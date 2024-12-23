#include <stdio.h>
#include <stdlib.h>
#include "disk_scheduling.h"
void* cscan_thread(void* arg) {
    DiskSchedulingData* data = (DiskSchedulingData*)arg;
    int current_head = data->head;
    data->total_movement = 0;
    int index = 0;
    
    int *requests_sorted = malloc((data->n + 2) * sizeof(int));
    if (requests_sorted == NULL) {
        return NULL;
    }

    for (int i = 0; i < data->n; i++) 
        requests_sorted[i] = data->requests[i];
    requests_sorted[data->n] = 0;
    requests_sorted[data->n + 1] = data->disk_size - 1;

    // Sort the requests
    for (int i = 0; i < data->n + 2; i++) {
        for (int j = 0; j < data->n + 1 - i; j++) {
            if (requests_sorted[j] > requests_sorted[j + 1]) {
                int temp = requests_sorted[j];
                requests_sorted[j] = requests_sorted[j + 1];
                requests_sorted[j + 1] = temp;
            }
        }
    }

    // Find the index where head is located
    for (int i = 0; i < data->n + 2; i++) {
        if (requests_sorted[i] >= current_head) {
            index = i;
            break;
        }
    }

    printf("\nC-SCAN Disk Scheduling\n");
    printf("Sequence of movement: %d", current_head);

    for (int i = index; i < data->n + 2; i++) {
        data->total_movement += abs(current_head - requests_sorted[i]);
        current_head = requests_sorted[i];
        printf(" -> %d", current_head);
    }

    current_head = 0;
    printf(" -> %d", current_head);
    data->total_movement += abs(requests_sorted[data->n + 1] - 0);

    for (int i = 0; i < index; i++) {
        data->total_movement += abs(current_head - requests_sorted[i]);
        current_head = requests_sorted[i];
        printf(" -> %d", current_head);
    }

    printf("\nTotal head movement: %d\n", data->total_movement);

    data->seek_time = data->total_movement * SEEK_TIME_PER_CYLINDER;

    free(requests_sorted);
    return NULL;
}
