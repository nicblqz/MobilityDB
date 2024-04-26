#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"

int main()
{
    meos_initialize(NULL, NULL);

    BWC_DR *bwc;
    bwc = (BWC_DR *) malloc(sizeof(BWC_DR));

    char *inst_1 = "POINT(1 1)@2000-01-01";
    char *inst_2 = "POINT(2 2)@2000-01-02";
    bwc->total = 2;

    PPoint *ppoint1 = (PPoint *) malloc(sizeof(PPoint));
    ppoint1->tid = 1;
    ppoint1->point = tgeompoint_in(inst_1);
    ppoint1->priority = 1;
    PPoint *ppoint2 = (PPoint *) malloc(sizeof(PPoint));
    ppoint2->tid = 2;
    ppoint2->point = tgeompoint_in(inst_2);
    ppoint2->priority = 2;

    Trips *trip1 = (Trips *) malloc(sizeof(Trips));
    PPoint ppoints[bwc->total];
    if (ppoint1->priority < ppoint2->priority)
    {
        ppoints[0] = *ppoint2;
        ppoints[1] = *ppoint1;
    }
    else
    {
        ppoints[0] = *ppoint1;
        ppoints[1] = *ppoint2;
    }
    trip1->id = 1;
    trip1->ppoints = ppoints;
    bwc->trips = trip1;

    char *inst_mfjson1 = temporal_as_mfjson(bwc->trips->ppoints[0].point, true, 3, 6, "EPSG:4326");
    char *inst_mfjson2 = temporal_as_mfjson(bwc->trips->ppoints[1].point, true, 3, 6, "EPSG:4326");
    printf("\n-----------\nfirst point\n-----------\n\n");
    printf("point %d: \n %s\n\n", bwc->trips->ppoints[0].tid, inst_mfjson1);
    printf("-----------\nsecond point\n-----------\n\n");
    printf("point %d: \n %s\n\n", bwc->trips->ppoints[1].tid, inst_mfjson2);
    free(bwc);
    free(trip1);
    free(ppoint1);
    free(ppoint2);
    meos_finalize();
    return 0;
}