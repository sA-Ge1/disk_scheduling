#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "disk_scheduling.h"

void* sstf_thread(void* arg) {
    DiskSchedulingData* data = (DiskSchedulingData*)arg;
    int *visited = (int *)calloc(data->n, sizeof(int));
    int current_head = data->head;
    data->total_movement = 0;

    if (visited == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    printf("\nSSTF Disk Scheduling\n");
    printf("Sequence of movement: %d", current_head);

    for (int i = 0; i < data->n; i++) {
        int min_distance = INT_MAX;
        int next_index = -1;

        for (int j = 0; j < data->n; j++) {
            if (!visited[j] && abs(current_head - data->requests[j]) < min_distance) {
                min_distance = abs(current_head - data->requests[j]);
                next_index = j;
            }
        }

        data->total_movement += min_distance;
        current_head = data->requests[next_index];
        visited[next_index] = 1;

        printf(" -> %d", current_head);
    }

    data->seek_time = data->total_movement * SEEK_TIME_PER_CYLINDER;
    printf("\nTotal head movement: %d\n", data->total_movement);
    printf("Seek time: %.2f ms\n", data->seek_time);

    free(visited);
    return NULL;
}
