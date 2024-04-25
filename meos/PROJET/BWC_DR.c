#include "MobilityDB/meos/include/meos.h"
#include <utils/timestamp.h>
#include "general/temporal.h"
#include "math.h"

typedef struct{
    double x;
    double y;
    TimestampTz time;
} Point;

typedef struct {
    int tid;
    Point point; // Temporal voir example
    double priority;
    double sog;
    double cog;
} PriorityPoint;

typedef struct {
    int id;
    PriorityPoint* points;
} trips;

typedef struct {
    //nys ??? -> plan
    int limit;
    int window;
    int start;
    int total = 0;
    trips trips;
    priority_list priority_list;
    // i, finalized_trips
    trips uncompressed_trips;
    priority_list* finished_windows; // Queue ?
} BWC_DR;

typedef struct {
    PriorityPoint ppoint;
    struct priority_list_node* next;
} priority_list_node;

typedef struct {
    priority_list_node* head;
} priority_list;

void sorted_priority_list(priority_list* list, PriorityPoint* ppoint) {
    priority_list_node* new_node = (priority_list_node*) malloc(sizeof(priority_list_node));
    new_node->ppoint = ppoint;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        priority_list_node* current = list->head;
        priority_list_node* previous = NULL;
        while (current != NULL && current->ppoint->priority < ppoint->priority) {
            previous = current;
            current = current->next;
        }
        if (previous == NULL) {
            new_node->next = list->head;
            list->head = new_node;
        } else {
            previous->next = new_node;
            new_node->next = current;
        }
    }
}

void init_priority_list(BWC_DR* bwc, PriorityPoint* ppoint) {
    bwc->priority_list = sorted_priority_list(bwc->priority_list, ppoint);
}

int check_next_window(BWC_DR *bwc, PriorityPoint* ppoint) {
    TimestampTz time = ppoint->point.time;
    if (time > bwc->start + bwc->window) {
        while (time > bwc->start + bwc->window)
            bwc->start += bwc->window;
        bwc->finished_window = bwc->priority_list;
        bwc->priority_list = sorted_priority_list(bwc->priority_list, ppoint);
        return 1;
    }
    return 0;
}

int add_point(BWC_DR *bwc, PriorityPoint* ppoint) {
    int new_window = check_next_window(bwc, ppoint);
    bwc->total += 1;
    if (ppoint->tid not in bwc->trips) { // (not in) pas en c
        bwc->trips[ppoint->tid] = (PriorityPoint*) malloc(sizeof(PriorityPoint));
        bwc->trips[ppoint->tid] = ppoint;
        ppoint->priority = HUGE_VAL;
    } else {
        bwc->trips[ppoint->tid] = ppoint;
        ppoint->priority = 0; //evaluate priority
    }
    return new_window;
}