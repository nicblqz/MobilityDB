#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>

#define MAX_INSTANTS 50000

typedef struct {
    int tid;
    Temporal* point;
    double priority;
    double sog;
    double cog;
} PPoint;

typedef struct {
    int tid;
    int size;
    PPoint *trip[MAX_INSTANTS];
} Trip;

typedef struct {
    int size;
    PPoint *ppoints[MAX_INSTANTS];
} priority_list;

typedef struct {
    int total;
    int limit;
    const Interval *window;
    TimestampTz start;
    int number_of_trips;
    Trip* trips[MAX_INSTANTS];
    Trip* uncompressed_trips;
    priority_list* priority_list;
    int finished_windows_size;
    priority_list* finished_windows[MAX_INSTANTS];
} BWC_DR;

bool check_next_window(BWC_DR *bwc, PPoint *ppoint);
bool add_point(BWC_DR *bwc, PPoint *ppoint);
void sorted_priority_list(priority_list* list);
Temporal *get_expected_position(Trip *trip, PPoint *point);
Temporal *get_position(PPoint *ppoint, TimestampTz time);
double evaluate_priority(BWC_DR *bwc, PPoint *ppoint);
void remove_point(BWC_DR *bwc);