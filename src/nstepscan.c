#include <stdio.h>
#include <stdlib.h>
#include "disk_scheduling.h"

void* nstep_scan_thread(void* arg) {
    DiskSchedulingData* data = (DiskSchedulingData*)arg;
    int current_head = data->head;
    data->total_movement = 0;
    int steps = (data->n + data->step_size - 1) / data->step_size;
    int current_direction = data->direction;
    
    int *sorted_requests = malloc(data->n * sizeof(int));
    if (sorted_requests == NULL) {
        return NULL;
    }

    for (int i = 0; i < data->n; i++) 
        sorted_requests[i] = data->requests[i];

    // Sort the requests
    for (int i = 0; i < data->n; i++) {
        for (int j = 0; j < data->n - 1; j++) {
            if (sorted_requests[j] > sorted_requests[j + 1]) {
                int temp = sorted_requests[j];
                sorted_requests[j] = sorted_requests[j + 1];
                sorted_requests[j + 1] = temp;
            }
        }
    }

    printf("\nN-Step-SCAN Disk Scheduling\n");
    printf("Sequence of movement: %d", current_head);

    for (int k = 0; k < steps; k++) {  
        int start_index = k * data->step_size;

        if (current_direction == 1) {  // Right direction
            for (int i = start_index; i < start_index + data->step_size && i < data->n; i++) {
                data->total_movement += abs(current_head - sorted_requests[i]);
                current_head = sorted_requests[i];
                printf(" -> %d", current_head);
            }
        } else {  // Left direction
            for (int i = start_index + data->step_size - 1; i >= start_index && i < data->n; i--) {
                data->total_movement += abs(current_head - sorted_requests[i]);
                current_head = sorted_requests[i];
                printf(" -> %d", current_head);
            }
        }
        current_direction = 1 - current_direction;  // Switch direction after each step
    }

    printf("\nTotal head movement: %d\n", data->total_movement);
    
    // Add this line to calculate seek time
    data->seek_time = data->total_movement * SEEK_TIME_PER_CYLINDER;

    free(sorted_requests);
    return NULL;
}
