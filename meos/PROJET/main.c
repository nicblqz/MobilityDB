#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

#include "BWC_DR.h"

// gcc -Wall -g -I/usr/local/include -o main main.c BWC_DR.c -L/usr/local/lib -lmeos

int main()
{
    meos_initialize(NULL, NULL);

    char *inst_1 = "SRID=4326;POINT(1 1)@2000-01-01 00:00:00+01";
    char *inst_2 = "SRID=4326;POINT(314295.822102 314295.822102)@2000-01-02 00:00:00+01";
    char *inst_3 = "SRID=4326;POINT(3 3)@2000-01-03 00:00:00+01";

    PPoint *ppoint1 = (PPoint *) malloc(sizeof(PPoint));
    ppoint1->tid = 1;
    ppoint1->point = tgeompoint_in(inst_1);
    ppoint1->sog = 10;
    ppoint1->cog = 45;
    PPoint *ppoint2 = (PPoint *) malloc(sizeof(PPoint));
    ppoint2->tid = 1;
    ppoint2->point = tgeompoint_in(inst_2);
    PPoint *ppoint3 = (PPoint *) malloc(sizeof(PPoint));
    ppoint3->tid = 1;
    ppoint3->point = tgeompoint_in(inst_3);

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

    // test get_expected_position OK
    /*Trip *trip1 = (Trip *) malloc(sizeof(Trip));
    trip1->size = 1;
    trip1->trip[0] = ppoint1;
    Temporal *expected_position = get_expected_position(trip1, ppoint1);
    char *expected_position_str = temporal_as_mfjson(expected_position, true, 3, 6, "EPSG:4326");
    printf("expected position: %s\n", expected_position_str);*/

    // test get x OK
    /*Temporal *x = tpoint_get_x(ppoint3->point);
    double x_str = tfloat_start_value(x);
    printf("x: %f\n", x_str);*/

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

    // test get_position OK
    /*Temporal *pos = get_position(ppoint1, t2);
    char *pos_str = temporal_as_mfjson(pos, true, 3, 6, "EPSG:4326");
    printf("position: %s\n", pos_str); test ok*/

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
    
    // test manipulate structures OK
    /*Trip *trip1 = (Trip *) malloc(sizeof(Trip));
    PPoint ppoints[bwc->total];
    priority_list p_list[] = {{ppoint1, ppoint1->priority}, {ppoint2, ppoint2->priority}, {ppoint3, ppoint3->priority}};
    sorted_priority_list(p_list, bwc->total);
    for (int i = 0; i < bwc->total; i++)
    {
        ppoints[i] = *p_list[i].ppoint;
    }
    trip1->tid = 1;
    trip1->size = bwc->total;
    trip1->ppoints = ppoints;
    bwc->trips = trip1;

    char *inst_mfjson1 = temporal_as_mfjson(bwc->trips->ppoints[0].point, true, 3, 6, "EPSG:4326");
    char *inst_mfjson2 = temporal_as_mfjson(bwc->trips->ppoints[1].point, true, 3, 6, "EPSG:4326");
    char *inst_mfjson3 = temporal_as_mfjson(bwc->trips->ppoints[2].point, true, 3, 6, "EPSG:4326");
    for (int i = 0; i < bwc->total; i++)
    {
        printf("point %d \n", bwc->trips->ppoints[i].tid);
    }
    printf("point 1: %s\n", inst_mfjson1);
    printf("point 2: %s\n", inst_mfjson2);
    printf("point 3: %s\n", inst_mfjson3);*/
    //free(bwc);
    //free(trip1);
    free(ppoint1);
    free(ppoint2);
    free(ppoint3);
    meos_finalize();
    return 0;
}