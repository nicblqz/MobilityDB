#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

void sorted_priority_list(priority_list* list, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (list[i].priority > list[j].priority) {
                priority_list temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}

Temporal *get_expected_position(Trip *trip, Temporal *point){
    int index = 0;
    for (int i = 0; i < trip->size; i++){
        if (trip->trip[i] == point){
            index = i;
        }
    }
    TimestampTz time = temporal_start_timestamptz(point);
    if (index == 0) return point;
    else {
        return get_position(trip->trip[index-1], time);
    }
}

Temporal *get_position(PPoint *ppoint, TimestampTz time)
{
    TimestampTz ts = temporal_start_timestamptz(ppoint->point);
    double speed = (ppoint->sog) * 1852 / 3600;  // from knots to m/s
    double angle = ((int)(ppoint->cog)%360) * M_PI / 180; // from degrees to radians
    TimestampTz deltat = time - ts;
    double delta = deltat / 1000000; // from microseconds to seconds
    Temporal *start_x = tpoint_get_x(ppoint->point);
    Temporal *start_y = tpoint_get_y(ppoint->point);
    double x = tfloat_start_value(start_x) + speed * delta * cos(angle);
    double y = tfloat_start_value(start_y) + speed * delta * sin(angle);
    char inst[100];
    sprintf(inst, "SRID=4326;POINT(%f %f)@%s", x, y, pg_timestamptz_out(time));
    return tgeompoint_in(inst);
} // ok

double evaluate_priority(BWC_DR *bwc, PPoint *ppoint)
{
    int tid = ppoint->tid;
    if (bwc->trips[tid].size > 1){
        Temporal *expected_position = get_expected_position(bwc->trips[tid].trip, ppoint->point);
        Temporal *current_position = ppoint->point;
        double distance = nad_tpoint_tpoint(expected_position,current_position); //= distance2D(expected_position, current_position);
        return distance;
    } else {
        return INFINITY;
    } 
}

int main()
{
    meos_initialize(NULL, NULL);

    /*BWC_DR *bwc;
    bwc = (BWC_DR *) malloc(sizeof(BWC_DR));*/

    char *inst_1 = "SRID=4326;POINT(1 1)@2000-01-01 00:00:00+01";
    char *inst_2 = "SRID=4326;POINT(2 2)@2000-01-02 00:00:00+01";
    char *inst_3 = "POINT(3 3)@2000-01-03";
    //bwc->total = 3;

    PPoint *ppoint1 = (PPoint *) malloc(sizeof(PPoint));
    ppoint1->tid = 1;
    ppoint1->point = tgeompoint_in(inst_1);
    ppoint1->sog = 10;
    ppoint1->cog = 45;
    PPoint *ppoint2 = (PPoint *) malloc(sizeof(PPoint));
    ppoint2->tid = 1;
    ppoint2->point = tgeompoint_in(inst_2);
    ppoint2->priority = 3;
    PPoint *ppoint3 = (PPoint *) malloc(sizeof(PPoint));
    ppoint3->tid = 1;
    ppoint3->point = tgeompoint_in(inst_3);
    ppoint3->priority = 2;

    // test get_expected_position

    /*Temporal *x = tpoint_get_x(ppoint3->point);
    double x_str = tfloat_start_value(x);
    printf("x: %f\n", x_str); test ok*/

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


    /*Temporal *pos = get_position(ppoint1, t2);
    char *pos_str = temporal_as_mfjson(pos, true, 3, 6, "EPSG:4326");
    printf("position: %s\n", pos_str); test ok*/

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
    /*printf("point 1: %s\n", inst_mfjson1);
    printf("point 2: %s\n", inst_mfjson2);
    printf("point 3: %s\n", inst_mfjson3);*/
    //free(bwc);
    //free(trip1);
    free(ppoint1);
    free(ppoint2);
    meos_finalize();
    return 0;
}