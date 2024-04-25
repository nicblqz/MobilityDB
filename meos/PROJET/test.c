#include "meos.h"
#include "stdio.h"
#include "stdlib.h"
#include "struct.h"

int main()
{
    meos_initialize(NULL, NULL);
    char *inst_wkt = "POINT(1 1)@2000-01-01";
    PPoint *ppoint = (PPoint *) malloc(sizeof(PPoint));
    ppoint->tid = 1;
    ppoint->point = tgeompoint_in(inst_wkt);
    BWC_DR *bwc;
    bwc = (BWC_DR *) malloc(sizeof(BWC_DR));
    bwc->trips = (Trips *) malloc(sizeof(Trips));
    bwc->trips->id = ppoint->tid;
    bwc->trips->ppoints = ppoint;
    char *inst_mfjson = temporal_as_mfjson(bwc->trips->ppoints->point, true, 3, 6, "EPSG:4326");
    printf("\n"
    "--------------------\n"
    "| Temporal Instant |\n"
    "--------------------\n\n"
    "WKT:\n"
    "----\n%s\n\n"
    "MF-JSON:\n"
    "--------\n%s\n", inst_wkt, inst_mfjson);
    free(bwc);
    free(bwc->trips);
    free(ppoint->point);
    meos_finalize();
    return 0;
}