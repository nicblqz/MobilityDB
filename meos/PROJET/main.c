#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>
#include <string.h>

#include "BWC_DR.h"

// gcc -Wall -g -I/usr/local/include -o main main.c BWC_DR.c -L/usr/local/lib -lmeos

int main()
{
    meos_initialize(NULL, NULL);

    FILE *file = fopen("stream.csv", "r");
    BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));

    if (! file)
    {
        printf("Error opening input file\n");
        return 1;
    }

    char timestamp_buffer[32];
    int tid = 0;
    char x[32];
    char y[32];
    double sog = 0.0;
    double cog = 0.0;
    char inst[128];
    int i = 0;
    int read = 0;
    PPoint *ppoint[3719];
    for (int i = 0; i < 3719; i++){
        ppoint[i] = (PPoint *) malloc(sizeof(PPoint));
    }
    bool new_window = false;

  do
  {
    read = fscanf(file, "%[^,],%d,%[^,],%[^,],%lf,%lf\n",
      timestamp_buffer, &tid, x, y, &sog, &cog);

    if (i == 0){
        init_bwc(bwc, 4, timestamp_buffer, "10 seconds");
    }
    sprintf(inst, "SRID=32632;POINT(%s %s)@%s", x, y, timestamp_buffer);
    printf("%s\n",inst);
    ppoint[i]->tid = tid;
    ppoint[i]->point = tgeompoint_in(inst);
    ppoint[i]->sog = sog;
    ppoint[i]->cog = cog;

    new_window = add_point(bwc, ppoint[i]);
    i++;
    if (ferror(file))
    {
      printf("Error reading input file\n");
      fclose(file);
      return 1;
    }
  } while (!feof(file)); 

  FILE *file1 = fopen("compressed_output.csv", "w");
  FILE *file2 = fopen("uncompressed_output.csv", "w");
  for (int i = 0; i < 70; i++){
      for (int j = 0; j < bwc->trips[i]->size; j++){
        Temporal *x = tpoint_get_x(bwc->trips[i]->points[j]->point);
        double xd = tfloat_start_value(x);
        Temporal *y = tpoint_get_y(bwc->trips[i]->points[j]->point);
        double yd = tfloat_start_value(y);
        fprintf(file1, "%s,%d,%f,%f,%f,%f\n", pg_timestamptz_out(temporal_start_timestamptz(bwc->trips[i]->points[j]->point)), bwc->trips[i]->tid, xd, yd, bwc->trips[i]->points[j]->sog, bwc->trips[i]->points[j]->cog);
      }
      for (int j = 0; j < bwc->uncompressed_trips[i]->size; j++){
        Temporal *x = tpoint_get_x(bwc->uncompressed_trips[i]->points[j]->point);
        double xd = tfloat_start_value(x);
        Temporal *y = tpoint_get_y(bwc->uncompressed_trips[i]->points[j]->point);
        double yd = tfloat_start_value(y);
        fprintf(file2, "%s,%d,%f,%f,%f,%f\n", pg_timestamptz_out(temporal_start_timestamptz(bwc->uncompressed_trips[i]->points[j]->point)), bwc->uncompressed_trips[i]->tid, xd, yd, bwc->uncompressed_trips[i]->points[j]->sog, bwc->uncompressed_trips[i]->points[j]->cog);
      }
  }

  fclose(file);
  fclose(file1);
  fclose(file2);

    free(bwc);
    free(bwc->priority_list);
    for (int i = 0; i < 4; i++){
        free(bwc->priority_list->ppoints[i]);
    }
    for (int i = 0; i < 128; i++){
        free(bwc->trips[i]);
        free(bwc->uncompressed_trips[i]);
        for (int j = 0; j < 256; j++){
            free(bwc->trips[i]->points[j]);
            free(bwc->uncompressed_trips[i]->points[j]);
        }
    }
    for (int i = 0; i < 1024; i++){
        free(bwc->finished_windows[i]);
        for (int j = 0; j < 4; j++){
            free(bwc->finished_windows[i]->ppoints[j]);
        }
    }
    for (int i = 0; i < 3719; i++){
        free(ppoint[i]);
    }
    meos_finalize();
    return 0;
}