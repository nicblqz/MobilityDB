#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

#include "BWC_DR.h"

bool check_next_window(BWC_DR *bwc, PPoint *ppoint){
    TimestampTz time = temporal_start_timestamptz(ppoint->point);
    TimestampTz comp = add_timestamptz_interval(bwc->start, bwc->window);
    if (time > comp){
        while (time > comp)
        {
            bwc->start = add_timestamptz_interval(bwc->start, bwc->window);
        }
        // finished windows + priority list
        return true;
    }
    return false;
}

bool add_point(BWC_DR *bwc, PPoint *ppoint){
    bool new_window = check_next_window(bwc, ppoint);
    bwc->total++;
    bwc->uncompressed_trips->size++;
    bwc->uncompressed_trips->trip[bwc->uncompressed_trips->size] = ppoint;

    for (int i = 0; i < bwc->total; i++){
        if (bwc->trips[i]->tid == ppoint->tid){
            bwc->trips[i]->size++;
            bwc->trips[i]->trip[bwc->trips[i]->size] = ppoint;
            ppoint->priority = evaluate_priority(bwc, ppoint);
        } else {
            bwc->trips[i]->size++;
            bwc->trips[i]->trip[ppoint->tid] = ppoint;
            ppoint->priority = INFINITY;
        }
    }

    bwc->priority_list->size++;
    bwc->priority_list->ppoints[bwc->priority_list->size] = ppoint;
    sorted_priority_list(bwc->priority_list);

    while (bwc->priority_list->size > bwc->limit){
        break;
    }
    return new_window;
}

void sorted_priority_list(priority_list* list) {
    for (int i = 0; i < list->size - 1; i++) {
        for (int j = i + 1; j < list->size; j++) {
            if (list->ppoints[i]->priority > list->ppoints[j]->priority) {
                PPoint *temp = list->ppoints[i];
                list->ppoints[i] = list->ppoints[j];
                list->ppoints[j] = temp;
            }
        }
    }
}

Temporal *get_expected_position(Trip *trip, PPoint *point){
    printf("trip size : %d\n", trip->size);
    int index = 0;
    for (int i = 0; i < trip->size; i++){
        if (trip->trip[i] == point){
            index = i;
            printf("index : %d\n", index);
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
        double distance = nad_tpoint_tpoint(expected_position,ppoint->point);
        return distance;
    } else {
        return INFINITY;
    } 
}
