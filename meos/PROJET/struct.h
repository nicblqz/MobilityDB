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
    int total;
    int limit;
    int window;
    int start;
    Trips* trips;
    Trips* uncompressed_trips;
} BWC_DR;

/*class BWC_DR {
    public:
        int total;
        int limit;
        int window;
        int start;
        Trips *trips;
        // priority_list priority_list;
        Trips *uncompressed_trips;
        // priority_list* finished_windows;
        void set_limit(int limit){
            this->limit = limit;
        }
        void set_window(int window){
            this->window = window;
        }
        void set_start(int start){
            this->start = start;
        }
};*/