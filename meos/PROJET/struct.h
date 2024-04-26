#include "meos.h"

typedef struct {
    int tid;
    Temporal* point;
    double priority;
    double sog;
    double cog;
} PPoint;

typedef struct {
    int id;
    PPoint* ppoints;
} Trips;

typedef struct {
    PPoint *ppoint;
    double priority;
} priority_list;

typedef struct {
    int total;
    int limit;
    int window;
    int start;
    Trips* trips;
    Trips* uncompressed_trips;
    priority_list* priority_list;
} BWC_DR;