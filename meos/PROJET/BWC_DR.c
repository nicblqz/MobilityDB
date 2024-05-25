#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

#include "BWC_DR.h"

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

Temporal *get_expected_position(Trip *trip, PPoint *point){
    int index = 0;
    for (int i = 0; i < trip->size; i++){
        if (trip->trip[i] == point){
            index = i;
        }
    }

    TimestampTz time = temporal_start_timestamptz(point->point);
    if (index == 0) return point->point;
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
    Trip *trip;

    for (int i = 0; i < bwc->total; i++){
        if (bwc->trips[i]->tid == tid) {
            trip =  bwc->trips[i];
            }
    }
    if (trip->size > 1){
        Temporal *expected_position = get_expected_position(trip, ppoint);
        Temporal *current_position = ppoint->point;
        double distance = nad_tpoint_tpoint(expected_position,current_position); //= distance2D(expected_position, current_position);
        return distance;
    } else {
        return INFINITY;
    } 
}
