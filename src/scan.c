#include <stdio.h>
#include <stdlib.h>
#include "disk_scheduling.h"
int scan(int requests[], int n, int head, int disk_size, int direction) {
    int i, j, total_movement = 0, index = 0;
    int *requests_sorted = malloc((n + 2) * sizeof(int));
    if (requests_sorted == NULL) {
        return 0;  // Handle allocation failure
    }

    for (i = 0; i < n; i++) requests_sorted[i] = requests[i];
    requests_sorted[n] = 0;
    requests_sorted[n + 1] = disk_size - 1;

    for (i = 0; i < n + 2; i++) {
        for (j = 0; j < n + 1 - i; j++) {
            int temp;
            if (requests_sorted[j] > requests_sorted[j + 1]) {
                temp = requests_sorted[j];
                requests_sorted[j] = requests_sorted[j + 1];
                requests_sorted[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < n + 2; i++) {
        if (requests_sorted[i] >= head) {
            index = i;
            break;
        }
    }

    printf("Sequence of movement: %d", head);

    if (direction == 1) {
        for (i = index; i < n + 2; i++) {
            total_movement += abs(head - requests_sorted[i]);
            head = requests_sorted[i];
            printf(" -> %d", head);
        }
        for (i = index - 1; i >= 0; i--) {
            total_movement += abs(head - requests_sorted[i]);
            head = requests_sorted[i];
            printf(" -> %d", head);
        }
    } else {
        for (i = index - 1; i >= 0; i--) {
            total_movement += abs(head - requests_sorted[i]);
            head = requests_sorted[i];
            printf(" -> %d", head);
        }
        for (i = index; i < n + 2; i++) {
            total_movement += abs(head - requests_sorted[i]);
            head = requests_sorted[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal head movement: %d\n", total_movement);

    free(requests_sorted);
    return total_movement;
}

void* scan_thread(void* arg) {
    DiskSchedulingData* data = (DiskSchedulingData*)arg;
    int total_movement = 0;
    
    // Modify the scan function to return total_movement
    total_movement = scan(data->requests, data->n, data->head, data->disk_size, data->direction);
    
    // Store the results in the data structure
    data->total_movement = total_movement;
    data->seek_time = total_movement * SEEK_TIME_PER_CYLINDER;
    
    return NULL;
}
