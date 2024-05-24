#include "meos.h"

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
    Temporal *trip[MAX_INSTANTS];
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
    Trip* trips;
    Trip* uncompressed_trips;
    priority_list* priority_list;
} BWC_DR;