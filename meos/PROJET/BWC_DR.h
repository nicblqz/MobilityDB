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
    TInstant *instants[MAX_INSTANTS];
    PPoint *trip[MAX_INSTANTS];
} Trip;

typedef struct {
    PPoint *ppoint;
    double priority;
} priority_list;

typedef struct {
    int total;
    int limit;
    int window;
    int start;
    Trip* trips[5];
    Trip* uncompressed_trips;
    priority_list* priority_list;
} BWC_DR;

void sorted_priority_list(priority_list* list, int size);
Temporal *get_expected_position(Trip *trip, PPoint *point);
Temporal *get_position(PPoint *ppoint, TimestampTz time);
double evaluate_priority(BWC_DR *bwc, PPoint *ppoint);