#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>
#include <string.h>

#include "BWC_DR.h"

void init_bwc(BWC_DR *bwc, int limit, char* start, char* interval){
    bwc->priority_list = (priority_list *) malloc(sizeof(priority_list));
    for (int i = 0; i < 4; i++){
        bwc->priority_list->ppoints[i] = (PPoint *) malloc(sizeof(PPoint));
    }
    for (int i = 0; i < 128; i++){
        bwc->trips[i] = (Trip *) malloc(sizeof(Trip));
        bwc->uncompressed_trips[i] = (Trip *) malloc(sizeof(Trip));
        for (int j = 0; j < 256; j++){
            bwc->trips[i]->points[j] = (PPoint *) malloc(sizeof(PPoint));
            bwc->uncompressed_trips[i]->points[j] = (PPoint *) malloc(sizeof(PPoint));
        }
    }
    for (int i = 0; i < 1024; i++){
        bwc->finished_windows[i] = (priority_list *) malloc(sizeof(priority_list));
        for (int j = 0; j < 4; j++){
            bwc->finished_windows[i]->ppoints[j] = (PPoint *) malloc(sizeof(PPoint));
        }
    }
    bwc->total = 0;
    bwc->limit = limit;
    bwc->number_of_trips = 0;
    bwc->priority_list->size = 0;
    bwc->finished_windows[0]->size = 0;
    bwc->finished_windows_count = 0;
    bwc->start = pg_timestamptz_in(start, -1);
    bwc->window = pg_interval_in(interval, -1);
}

bool check_next_window(BWC_DR *bwc, PPoint *ppoint){
    TimestampTz time = temporal_start_timestamptz(ppoint->point);
    TimestampTz comp = add_timestamptz_interval(bwc->start, bwc->window);
    if (time > comp){
        while (time > comp)
        {
            bwc->start = add_timestamptz_interval(bwc->start, bwc->window);
            comp = add_timestamptz_interval(bwc->start, bwc->window);
        }
        bwc->finished_windows[bwc->finished_windows_count] = bwc->priority_list;
        bwc->finished_windows_count++;

        bwc->priority_list = (priority_list *) malloc(sizeof(priority_list));
        for (int i = 0; i < 4; i++){
            bwc->priority_list->ppoints[i] = (PPoint *) malloc(sizeof(PPoint));
        }
        bwc->priority_list->size = 0;
        return true;
    }
    return false;
}

bool add_point(BWC_DR *bwc, PPoint *ppoint){
    bool new_window = check_next_window(bwc, ppoint);
    bwc->total++;

    bool tid_found = false;
    for (int i = 0; i < bwc->number_of_trips; i++){
        if (bwc->trips[i]->tid == ppoint->tid){
            tid_found = true;
            bwc->uncompressed_trips[i]->points[bwc->uncompressed_trips[i]->size] = ppoint;
            bwc->trips[i]->points[bwc->trips[i]->size] = ppoint;
            bwc->uncompressed_trips[i]->size++;
            bwc->trips[i]->size++;
            ppoint->priority = evaluate_priority(bwc, ppoint);
        } 
    } if (!tid_found){
        bwc->uncompressed_trips[bwc->number_of_trips]->size = 1;
        bwc->trips[bwc->number_of_trips]->size = 1;
        bwc->uncompressed_trips[bwc->number_of_trips]->tid = ppoint->tid;
        bwc->trips[bwc->number_of_trips]->tid = ppoint->tid;
        ppoint->priority = INFINITY; 
        bwc->uncompressed_trips[bwc->number_of_trips]->points[0] = ppoint;
        bwc->trips[bwc->number_of_trips]->points[0] = ppoint;
        bwc->number_of_trips++;
        }
   // }

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
        if (trip->points[i] == point){
            index = i;
            break;
        }
    }

    TimestampTz time = temporal_start_timestamptz(point->point);

    if (index == 0) return point->point;
    else {
        return get_position(trip->points[index-1], time);
    }
}

Temporal *get_position(PPoint *ppoint, TimestampTz time)
{
    bool error = false;
    TimestampTz ts = temporal_start_timestamptz(ppoint->point);
    if (strcmp(pg_timestamp_out(ts),"2000-01-01 00:00:00") == 0){ 
        printf("Error: invalid timestamp\n");
        error = true;
    }
    TimestampTz deltat = (time - ts);
    double delta = (double)deltat / 1000000; // from microseconds to seconds

    double speed = (ppoint->sog) * 1852 / 3600;  // from knots to m/s
    double angle = ((int)(ppoint->cog)%360) * M_PI / 180; // from degrees to radians

    Temporal *start_x = tpoint_get_x(ppoint->point);
    Temporal *start_y = tpoint_get_y(ppoint->point);
    double x = tfloat_start_value(start_x) + speed * delta * cos(angle);
    double y = tfloat_start_value(start_y) + speed * delta * sin(angle);

    char inst[100];
    sprintf(inst, "SRID=32632;POINT(%f %f)@%s", x, y, pg_timestamptz_out(time));
    if (error) {
        return ppoint->point;
    } else {
    return tgeompoint_in(inst);
    }
} 

double evaluate_priority(BWC_DR *bwc, PPoint *ppoint)
{
    int tid = ppoint->tid;
    int index = 0; 

    for (int i = 0; i < bwc->number_of_trips; i++){
        if (bwc->trips[i]->tid == tid) {
            index = i;
            }
    }
    if (bwc->trips[index]->size > 1){
        Temporal *expected_position = get_expected_position(bwc->trips[index], ppoint);
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

    Trip *trip;
    int trip_index = 0;
    for (int i = 0; i < bwc->number_of_trips; i++){
        if (bwc->trips[i]->tid == to_remove->tid){
            trip_index = i;
            trip = bwc->trips[i];
        }
    }

    int to_remove_index = 0;
    for (int i = 0; i < trip->size; i++){
        if (trip->points[i] == to_remove){
            to_remove_index = i;
            break;
        }   
    }

    for (int i = to_remove_index; i < trip->size; i++) {
        if (i == trip->size - 1) {
            trip->points[i] = NULL;
        }
        else {
        trip->points[i] = trip->points[i+1];
        }
    }
    trip->size--;

    for (int i = to_remove_index; i<trip->size; i++){
        PPoint *to_update = trip->points[i];
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


