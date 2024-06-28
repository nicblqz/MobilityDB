#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"
#include <math.h>
#include <string.h>

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
    PPoint *points[256];
} Trip;

typedef struct {
    int size;
    PPoint *ppoints[4];
} priority_list;

typedef struct {
    int total;
    int limit;
    const Interval *window;
    TimestampTz start;
    int number_of_trips;
    Trip* trips[128]; 
    Trip* uncompressed_trips[128];
    priority_list* priority_list;
    int finished_windows_count;
    priority_list* finished_windows[1024];
} BWC_DR;

void init_bwc(BWC_DR *bwc, int limit, char* start, char* interval);
bool check_next_window(BWC_DR *bwc, PPoint *ppoint);
bool add_point(BWC_DR *bwc, PPoint *ppoint);
void sorted_priority_list(priority_list* list);
Temporal *get_expected_position(Trip *trip, PPoint *point);
Temporal *get_position(PPoint *ppoint, TimestampTz time);
double evaluate_priority(BWC_DR *bwc, PPoint *ppoint);
void remove_point(BWC_DR *bwc);