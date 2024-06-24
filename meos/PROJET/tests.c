#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

#include "BWC_DR.h"

// gcc -Wall -g -I/usr/local/include -o test test.c BWC_DR.c -L/usr/local/lib -lmeos

int main()
{
    meos_initialize(NULL, NULL);

    char *inst_1 = "SRID=4326;POINT(14.376078333333334 40.63037666666666)@2024-01-26 11:47:39.790272+01";
    char *inst_2 = "SRID=4326;POINT(14.37608 40.63037666666666)@2024-01-26 11:47:43.067087+01";
    char *inst_3 = "SRID=4326;POINT(14.376081666666666 40.630379999999995)@2024-01-26 11:48:57.512489+01";
    char *inst_4 = "SRID=4326;POINT(14.376081666666666 40.63037666666666)@2024-01-26 11:50:10.115083+01";
    char *inst_5 = "SRID=4326;POINT(14.376081666666666 40.63037666666666)@2024-01-26 11:50:12.777446+01";
    char *inst_6 = "SRID=4326;POINT(14.376085 40.63036666666667)@2024-01-26 11:51:13.910862+01";
    char *inst_7 = "SRID=4326;POINT(14.37609 40.63037666666666)@2024-01-26 11:52:13.200907+01";



    PPoint *ppoint1 = (PPoint *) malloc(sizeof(PPoint));
    ppoint1->tid = 247070900;
    ppoint1->point = tgeompoint_in(inst_1);
    ppoint1->sog = 0;
    ppoint1->cog = 348.5;
    PPoint *ppoint2 = (PPoint *) malloc(sizeof(PPoint));
    ppoint2->tid = 247070900;
    ppoint2->sog = 0;
    ppoint2->cog = 354.4;
    ppoint2->point = tgeompoint_in(inst_2);
    PPoint *ppoint3 = (PPoint *) malloc(sizeof(PPoint));
    ppoint3->tid = 247070900;
    ppoint3->sog = 0;
    ppoint3->cog = 335.3;
    ppoint3->point = tgeompoint_in(inst_3);
    PPoint *ppoint4 = (PPoint *) malloc(sizeof(PPoint));
    ppoint4->tid = 247070900;
    ppoint4->sog = 0;
    ppoint4->cog = 330.5;
    ppoint4->point = tgeompoint_in(inst_4);
    PPoint *ppoint5 = (PPoint *) malloc(sizeof(PPoint));
    ppoint5->tid = 247070901;
    ppoint5->sog = 0;
    ppoint5->cog = 333.1;
    ppoint5->point = tgeompoint_in(inst_5);
    PPoint *ppoint6 = (PPoint *) malloc(sizeof(PPoint));
    ppoint6->tid = 247070900;
    ppoint6->sog = 0;
    ppoint6->cog = 327.4;
    ppoint6->point = tgeompoint_in(inst_6);
    PPoint *ppoint7 = (PPoint *) malloc(sizeof(PPoint));
    ppoint7->tid = 247070900;
    ppoint7->sog = 0;
    ppoint7->cog = 322.6;
    ppoint7->point = tgeompoint_in(inst_7);
    PPoint *list = (PPoint *) malloc(7*sizeof(PPoint));
    list[0] = *ppoint1;
    list[1] = *ppoint2;
    list[2] = *ppoint3;
    list[3] = *ppoint4;
    list[4] = *ppoint5;
    list[5] = *ppoint6;
    list[6] = *ppoint7;

    // tests on timestamptz type OK
    /*char* date = "2000-01-01 00:00:00+01";
    char* date2 = "2000-01-01 00:00:01+01";
    char* date3 = "2000-01-01 00:01:00+01";
    char* date4 = "2000-01-01 01:00:00+01";
    char* date5 = "2000-01-02 00:00:00+01";
    char* date6 = "2000-02-01 00:00:00+01";
    char* date7 = "2001-01-01 00:00:00+01";
    char* date8 = "2024-01-01 01:00:00+01";
    TimestampTz t = pg_timestamptz_in(date, -1);
    TimestampTz t2 = pg_timestamptz_in(date2, -1);
    TimestampTz t3 = pg_timestamptz_in(date3, -1);
    TimestampTz t4 = pg_timestamptz_in(date4, -1);
    TimestampTz t5 = pg_timestamptz_in(date5, -1);
    TimestampTz t6 = pg_timestamptz_in(date6, -1);
    TimestampTz t7 = pg_timestamptz_in(date7, -1);
    TimestampTz t8 = pg_timestamptz_in(date8, -1);
    printf("t1: %lld\n", t);
    printf("t2: %lld\n", t2);
    printf("t3: %lld\n", t3);
    printf("t4: %lld\n", t4);
    printf("t5: %lld\n", t5);
    printf("t6: %lld\n", t6);
    printf("t7: %lld\n", t7);
    printf("t8: %lld\n", t8);*/

    // test get x OK
    /*Temporal *x = tpoint_get_x(ppoint3->point);
    double x_str = tfloat_start_value(x);
    printf("x: %f\n", x_str);*/

    // test distance between points OK
    /*TInstant *instants[2];
    instants[0] = (TInstant*) ppoint1->point;
    instants[1] = (TInstant*) ppoint2->point;
    Temporal * seq;
    seq = (Temporal *) tsequence_make((const TInstant **) instants, 2,
    true, true, LINEAR, true);
    printf("\nNumber of instants: %d, Distance : %lf\n",
    temporal_num_instants(seq), tpoint_length(seq));
    double distance = nad_tpoint_tpoint(ppoint1->point, ppoint2->point); 
    printf("distance: %lf\n", distance); test ok*/

    // test compute interval OK
    /*TimestampTz t1 = temporal_start_timestamptz(ppoint1->point);
    char *time_str = pg_timestamptz_out(t1);
    printf("start time: %s\n", time_str);
    TimestampTz t2 = temporal_start_timestamptz(ppoint2->point);
    char *time_str2 = pg_timestamptz_out(t2);
    printf("start time: %s\n", time_str2);
    TimestampTz delta_time = t2 - t1; 
    double inte = delta_time / 1000000;
    Interval *delta = minus_timestamptz_timestamptz(t2, t1);
    char *interv = pg_interval_out(delta);
    char *time_str3 = pg_timestamptz_out(delta);
    printf("delta: %lld\n", delta_time);
    printf("delta: %f\n", inte); test ok*/

    // test get_expected_position OK
    /*Trip *trip1 = (Trip *) malloc(sizeof(Trip));
    trip1->size = 2;
    trip1->trip[0] = ppoint1;
    trip1->trip[1] = ppoint2;
    Temporal *expected_position = get_expected_position(trip1, ppoint2);
    char *expected_position_str = temporal_as_mfjson(expected_position, true, 3, 6, "EPSG:4326");
    printf("expected position: %s\n", expected_position_str);*/

    // test get_position OK
    /*Temporal *pos = get_position(ppoint1, t2);
    char *pos_str = temporal_as_mfjson(pos, true, 3, 6, "EPSG:4326");
    printf("position: %s\n", pos_str); test ok*/

    // test evaluate_priority OK
    /*BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    Trip *trip1 = (Trip *) malloc(sizeof(Trip));
    trip1->size = 2;
    trip1->tid = 1;
    trip1->trip[0] = ppoint1;
    trip1->trip[1] = ppoint2;
    bwc->total = 1;
    bwc->trips[0] = trip1;
    ppoint2->priority = evaluate_priority(bwc,ppoint2);
    printf("Point 2 priority : %f", ppoint2->priority);*/
    
    // test priority list OK
    /*BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    Trip *trip1 = (Trip *) malloc(sizeof(Trip));
    priority_list *p_list = (priority_list *) malloc(sizeof(priority_list));
    trip1->size = 3;
    trip1->tid = 1;
    trip1->trip[0] = ppoint1;
    trip1->trip[1] = ppoint2;
    trip1->trip[2] = ppoint3;
    bwc->total = 1;
    bwc->trips[0] = trip1;
    //ppoint1->priority = evaluate_priority(bwc,ppoint1);
    //ppoint2->priority = evaluate_priority(bwc,ppoint2);
    //ppoint3->priority = evaluate_priority(bwc,ppoint3);
    ppoint1->priority = 3;
    ppoint2->priority = 2;
    ppoint3->priority = 1;
    p_list->size = 3;
    p_list->ppoints[0] = ppoint1;
    p_list->ppoints[1] = ppoint2;
    p_list->ppoints[2] = ppoint3;
    printf("Before sorting\n");
    sorted_priority_list(p_list);
    for (int i = 0; i < p_list->size; i++){
        printf("Point %d priority : %f\n", i, p_list->ppoints[i]->priority);
    }*/

    //test check_next_window OK
    /*char *inst_4 = "SRID=4326;POINT(1 1)@2000-01-03 00:00:00+01";
    Temporal *point = tgeompoint_in(inst_4);
    PPoint *ppoint = (PPoint *) malloc(sizeof(PPoint));
    ppoint->point = point;
    BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    const Interval *window = (const Interval *) malloc(sizeof(Interval));
    window = pg_interval_in("1 day", -1);
    bwc->window = window;
    bwc->start = pg_timestamptz_in("2000-01-01 00:00:00+01", -1);
    bool new_window = check_next_window(bwc, ppoint);
    printf(pg_timestamp_out(bwc->start));*/

    // test add_point OK
    BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    BWC_DR *bwc2 = (BWC_DR *) malloc(sizeof(BWC_DR));
    init_bwc(bwc, 3, "2024-01-26 11:47:39.790272+01", "10 seconds");
    init_bwc(bwc2, 3, "2024-01-26 11:47:39.790272+01", "1 day");
    bool new_window = add_point(bwc, ppoint1);
    for (int k = 1; k < 7; k++){
      for (int i = 0; i < bwc->number_of_trips; i++){
        for (int j = 0; j < bwc->trips[i]->size; j++){
            printf("\ntrip %d : %f\n", bwc->trips[i]->tid, bwc->trips[i]->trip[j]->priority);
        }
      }
    if (new_window){
      for (int i = 0; i < bwc->finished_windows_count; i++){
        for (int j = 0; j < bwc->finished_windows[i]->size; j++){
            printf("finished window %d : %f\n", i, bwc->finished_windows[i]->ppoints[j]->priority);
        }
    }
    } else {
        for (int i = 0; i < bwc->priority_list->size; i++){
            printf("priority %d : %f\n", i, bwc->priority_list->ppoints[i]->priority);
        }
    }
    printf("------------------\n");
    new_window = add_point(bwc, &list[k]);
    }
          for (int i = 0; i < bwc->number_of_trips; i++){
        for (int j = 0; j < bwc->trips[i]->size; j++){
            printf("trip %d : %f\n", bwc->trips[i]->tid, bwc->trips[i]->trip[j]->priority);
        }
      }
    if (new_window){
      for (int i = 0; i < bwc->finished_windows_count; i++){
        for (int j = 0; j < bwc->finished_windows[i]->size; j++){
            printf("finished window %d : %f\n", i, bwc->finished_windows[i]->ppoints[j]->priority);
        }
    }
    } else {
        for (int i = 0; i < bwc->priority_list->size; i++){
            printf("priority %d : %f\n", i, bwc->priority_list->ppoints[i]->priority);
        }
    }
    printf("------------------\n");
    new_window = add_point(bwc2, ppoint1);
    for (int k = 1; k < 7; k++){
      for (int i = 0; i < bwc2->number_of_trips; i++){
        for (int j = 0; j < bwc2->trips[i]->size; j++){
            printf("trip %d : %f\n", bwc2->trips[i]->tid, bwc2->trips[i]->trip[j]->priority);
        }
      }
    if (new_window){
      for (int i = 0; i < bwc2->finished_windows_count; i++){
        for (int j = 0; j < bwc2->finished_windows[i]->size; j++){
            printf("finished window %d : %f\n", i, bwc2->finished_windows[i]->ppoints[j]->priority);
        }
    }
    } else {
        for (int i = 0; i < bwc2->priority_list->size; i++){
            printf("priority %d : %f\n", i, bwc2->priority_list->ppoints[i]->priority);
        }
    }
    printf("------------------\n");
    new_window = add_point(bwc2, &list[k]);
    }
          for (int i = 0; i < bwc2->number_of_trips; i++){
        for (int j = 0; j < bwc2->trips[i]->size; j++){
            printf("trip %d : %f\n", bwc2->trips[i]->tid, bwc2->trips[i]->trip[j]->priority);
        }
      }
    if (new_window){
      for (int i = 0; i < bwc2->finished_windows_count; i++){
        for (int j = 0; j < bwc2->finished_windows[i]->size; j++){
            printf("finished window %d : %f\n", i, bwc2->finished_windows[i]->ppoints[j]->priority);
        }
    }
    } else {
        for (int i = 0; i < bwc2->priority_list->size; i++){
            printf("priority %d : %f\n", i, bwc2->priority_list->ppoints[i]->priority);
        }
    }
    printf("------------------\n");

    // test finak OK
    /*BWC_DR *bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    init_bwc(bwc, 3, "2000-01-01 00:00:00+01", "1 day");
    bool new_window = add_point(bwc, ppoint1);
    new_window = add_point(bwc, ppoint2);
    new_window = add_point(bwc, ppoint3);
    printf("trip 0 : %f\n", bwc->trips[0]->trip[0]->priority);
    printf("trip 1 : %f\n", bwc->trips[0]->trip[1]->priority);
    printf("trip 2 : %f\n", bwc->trips[0]->trip[2]->priority);
    //printf("finished windows 0 : %f\n", bwc->finished_windows[0]->ppoints[0]->priority); 
    //printf("finished windows 1 : %f\n", bwc->finished_windows[0]->ppoints[1]->priority);
    printf("priority 0 : %f\n", bwc->priority_list->ppoints[0]->priority);
    printf("priority 1 : %f\n", bwc->priority_list->ppoints[1]->priority);
    printf("priority 2 : %f\n", bwc->priority_list->ppoints[2]->priority);
    
    remove_point(bwc);
    printf("------------------\n"); 
    printf("trip 0 : %f\n", bwc->trips[0]->trip[0]->priority);
    printf("trip 1 : %f\n", bwc->trips[0]->trip[1]->priority);
    printf("priority 0 : %f\n", bwc->priority_list->ppoints[0]->priority);
    printf("priority 1 : %f\n", bwc->priority_list->ppoints[1]->priority);
    remove_point(bwc);
    printf("------------------\n");
    printf("trip 0 : %f\n", bwc->trips[0]->trip[0]->priority);
    printf("priority 0 : %f\n", bwc->priority_list->ppoints[0]->priority);*/

    FILE *file = fopen("stream.csv", "r");
    BWC_DR *bwc1 = (BWC_DR *) malloc(sizeof(BWC_DR));


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
    int i = 0;
    int no_records = 0;
    int no_nulls = 0;
    char inst[128];
    PPoint *ppoint = (PPoint *) malloc(sizeof(PPoint));

  do
  {
    int read = fscanf(file, "%[^,],%d,%[^,],%[^,],%lf,%lf\n",
      timestamp_buffer, &tid, x, y, &sog, &cog);

    if (i == 0){
        init_bwc(bwc1, 4, timestamp_buffer, "10 seconds");
    }
    i++;
    sprintf(inst, "SRID=4326;POINT(%s %s)@%s+01", x, y, timestamp_buffer);
    ppoint->tid = tid;
    ppoint->point = tgeompoint_in(inst);
    ppoint->sog = sog;
    ppoint->cog = cog;

    add_point(bwc1, ppoint);

    if (read == 6)
      no_records++;

    if (read != 6 && !feof(file))
    {
      printf("Record with missing values ignored\n");
      no_nulls++;
    }

    if (ferror(file))
    {
      printf("Error reading input file\n");
      fclose(file);
      return 1;
    }



  } //while (!feof(file)); 
    while (i < 14);

  printf("\n%d no_records read.\n%d incomplete records ignored.\n",
    no_records, no_nulls);

  printf("Number of trips: %d\n", bwc1->number_of_trips);
  for (int i = 0; i < bwc1->number_of_trips; i++){
    printf("trip size %d : %d\n", i, bwc1->trips[i]->size);
    for (int j = 0; j < bwc1->trips[i]->size; j++){
        printf("trip %d : %f\n", bwc1->trips[i]->tid, bwc1->trips[i]->trip[j]->priority);
    }
  }
    for (int i = 0; i < bwc1->finished_windows_count; i++){
      for (int j = 0; j < bwc1->finished_windows[i]->size; j++){
            printf("finished window %d : %f\n", i, bwc1->finished_windows[i]->ppoints[j]->priority);
        }
    }
    if (bwc1->priority_list->size > 0){
    for (int i = 0; i < bwc1->priority_list->size; i++){
            printf("priority %d : %f\n", i, bwc1->priority_list->ppoints[i]->priority);
        }
    }

  fclose(file);

    free(bwc);
    free(bwc1);
    //free(trip1);
    //free(ppoint);
    free(ppoint1);
    free(ppoint2);
    free(ppoint3);
    meos_finalize();
    return 0;
}