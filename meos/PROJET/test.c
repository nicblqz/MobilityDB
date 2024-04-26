#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"

void sorted_priority_list(priority_list* list, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (list[i].priority > list[j].priority) {
                priority_list temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}

int main()
{
    meos_initialize(NULL, NULL);

    BWC_DR *bwc;
    bwc = (BWC_DR *) malloc(sizeof(BWC_DR));

    char *inst_1 = "POINT(1 1)@2000-01-01";
    char *inst_2 = "POINT(2 2)@2000-01-02";
    char *inst_3 = "POINT(3 3)@2000-01-03";
    bwc->total = 3;

    PPoint *ppoint1 = (PPoint *) malloc(sizeof(PPoint));
    ppoint1->tid = 1;
    ppoint1->point = tgeompoint_in(inst_1);
    ppoint1->priority = 1;
    PPoint *ppoint2 = (PPoint *) malloc(sizeof(PPoint));
    ppoint2->tid = 2;
    ppoint2->point = tgeompoint_in(inst_2);
    ppoint2->priority = 3;
    PPoint *ppoint3 = (PPoint *) malloc(sizeof(PPoint));
    ppoint3->tid = 3;
    ppoint3->point = tgeompoint_in(inst_3);
    ppoint3->priority = 2;

    Trips *trip1 = (Trips *) malloc(sizeof(Trips));
    PPoint ppoints[bwc->total];
    priority_list p_list[] = {{ppoint1, ppoint1->priority}, {ppoint2, ppoint2->priority}, {ppoint3, ppoint3->priority}};
    sorted_priority_list(p_list, bwc->total);
    for (int i = 0; i < bwc->total; i++)
    {
        ppoints[i] = *p_list[i].ppoint;
    }
    trip1->id = 1;
    trip1->ppoints = ppoints;
    bwc->trips = trip1;

    char *inst_mfjson1 = temporal_as_mfjson(bwc->trips->ppoints[0].point, true, 3, 6, "EPSG:4326");
    char *inst_mfjson2 = temporal_as_mfjson(bwc->trips->ppoints[1].point, true, 3, 6, "EPSG:4326");
    char *inst_mfjson3 = temporal_as_mfjson(bwc->trips->ppoints[2].point, true, 3, 6, "EPSG:4326");
    for (int i = 0; i < bwc->total; i++)
    {
        printf("point %d \n", bwc->trips->ppoints[i].tid);
    }
    /*printf("point 1: %s\n", inst_mfjson1);
    printf("point 2: %s\n", inst_mfjson2);
    printf("point 3: %s\n", inst_mfjson3);*/
    free(bwc);
    free(trip1);
    free(ppoint1);
    free(ppoint2);
    meos_finalize();
    return 0;
}