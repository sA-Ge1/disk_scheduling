#include <stdio.h>
#include <stdlib.h>
#include "disk_scheduling.h"

void* fcfs_thread(void* arg) {
    DiskSchedulingData* data = (DiskSchedulingData*)arg;
    int current_head = data->head;
    data->total_movement = 0;

    printf("\nFCFS Disk Scheduling\n");
    printf("Sequence of movement: %d", current_head);

    for (int i = 0; i < data->n; i++) {
        data->total_movement += abs(current_head - data->requests[i]);
        current_head = data->requests[i];
        printf(" -> %d", current_head);
    }

    data->seek_time = data->total_movement * SEEK_TIME_PER_CYLINDER;
    printf("\nTotal head movement: %d\n", data->total_movement);
    printf("Seek time: %.2f ms\n", data->seek_time);

    return NULL;
}
