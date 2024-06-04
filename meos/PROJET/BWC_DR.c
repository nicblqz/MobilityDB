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
            comp = add_timestamptz_interval(bwc->start, bwc->window);
        }
        bwc->finished_windows[bwc->finished_windows_size] = bwc->priority_list;
        bwc->finished_windows_size++;

        priority_list *new_list = (priority_list *) malloc(sizeof(priority_list));
        new_list->size = 0;
        bwc->priority_list = new_list;
        return true;
    }
    return false;
}

bool add_point(BWC_DR *bwc, PPoint *ppoint){
    bool new_window = check_next_window(bwc, ppoint);
    bwc->total++;

    bwc->uncompressed_trips->trip[bwc->uncompressed_trips->size] = ppoint;
    bwc->uncompressed_trips->size++;

    for (int i = 0; i < bwc->number_of_trips; i++){
        if (bwc->trips[i]->tid == ppoint->tid){
            bwc->trips[i]->trip[bwc->trips[i]->size] = ppoint;
            bwc->trips[i]->size++;
            ppoint->priority = evaluate_priority(bwc, ppoint);
        } else {
            bwc->trips[i+1] = (Trip *) malloc(sizeof(Trip));
            bwc->trips[i+1]->size = 1;
            bwc->trips[i+1]->tid = ppoint->tid;
            bwc->trips[i+1]->trip[0] = ppoint;
            ppoint->priority = INFINITY; // it doesnt work I dont know why, because ppoint->tid is updated and priority is not
            bwc->number_of_trips++;
        }
    }

    bwc->priority_list->ppoints[bwc->priority_list->size] = ppoint;
    bwc->priority_list->size++;
    sorted_priority_list(bwc->priority_list);

    while (bwc->priority_list->size > bwc->limit){
        remove_point(bwc);
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
    TimestampTz deltat = time - ts;
    double delta = deltat / 1000000; // from microseconds to seconds

    double speed = (ppoint->sog) * 1852 / 3600;  // from knots to m/s
    double angle = ((int)(ppoint->cog)%360) * M_PI / 180; // from degrees to radians

    Temporal *start_x = tpoint_get_x(ppoint->point);
    Temporal *start_y = tpoint_get_y(ppoint->point);
    double x = tfloat_start_value(start_x) + speed * delta * cos(angle);
    double y = tfloat_start_value(start_y) + speed * delta * sin(angle);

    char inst[100];
    sprintf(inst, "SRID=4326;POINT(%f %f)@%s", x, y, pg_timestamptz_out(time));
    return tgeompoint_in(inst);
} 

double evaluate_priority(BWC_DR *bwc, PPoint *ppoint)
{
    int tid = ppoint->tid;
    Trip *trip;

    for (int i = 0; i < bwc->number_of_trips; i++){
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

void remove_point(BWC_DR *bwc){
    PPoint *to_remove = bwc->priority_list->ppoints[0];
    for (int i = 0; i < bwc->priority_list->size; i++) {
        bwc->priority_list->ppoints[i] = bwc->priority_list->ppoints[i+1];
    }
    bwc->priority_list->ppoints[bwc->priority_list->size] = NULL;
    bwc->priority_list->size--;

    Trip *trip = (Trip *) malloc(sizeof(Trip));
    int trip_index = 0;
    for (int i = 0; i < bwc->number_of_trips; i++){
        if (bwc->trips[i]->tid == to_remove->tid){
            trip_index = i;
            trip = bwc->trips[i];
        }
    }

    int to_remove_index = 0;
    for (int i = 0; i < trip->size; i++){
        if (trip->trip[i] == to_remove){
            to_remove_index = i;
            break;
        }   
    }

    for (int i = to_remove_index; i < trip->size; i++) {
        if (i == trip->size - 1) {
            trip->trip[i] = NULL;
        }
        else {
        trip->trip[i] = trip->trip[i+1];
        }
    }
    trip->size--;

    for (int i = to_remove_index; i<trip->size; i++){
        PPoint *to_update = trip->trip[i];
        to_update->priority = evaluate_priority(bwc, to_update);
        for (int j = 0; j < bwc->priority_list->size; j++){
            if (bwc->priority_list->ppoints[j] == to_update){
                bwc->priority_list->ppoints[j]->priority = to_update->priority;
            }
        }
    } 

    bwc->trips[trip_index] = trip;
    sorted_priority_list(bwc->priority_list);
    bwc->total--;
}
