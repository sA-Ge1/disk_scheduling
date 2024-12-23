#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "disk_scheduling.h"
#include<stdio.h>
#include<stdlib.h>

#define SEEK_TIME_PER_CYLINDER 0.1 // 0.1ms per cylinder movement

void print_array(int arr[], int n) {
    printf("Request sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    DiskSchedulingData data;
    pthread_t threads[5];
    DiskSchedulingData thread_data[5];
    
    printf("Enter the number of requests: ");
    scanf("%d", &data.n);

    data.requests = (int*)malloc(data.n * sizeof(int));
    printf("Enter the request sequence: ");
    for (int i = 0; i < data.n; i++) {
        scanf("%d", &data.requests[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &data.head);

    printf("Enter disk size: ");
    scanf("%d", &data.disk_size);

    printf("Enter direction for SCAN/N-Step-SCAN (1 for right, 0 for left): ");
    scanf("%d", &data.direction);

    printf("Enter step size for N-Step-SCAN: ");
    scanf("%d", &data.step_size);

    // Create copies of data for each thread
    for (int i = 0; i < 5; i++) {
        thread_data[i] = data;
        thread_data[i].requests = malloc(data.n * sizeof(int));
        memcpy(thread_data[i].requests, data.requests, data.n * sizeof(int));
    }

    // Create threads for each algorithm
    pthread_create(&threads[0], NULL, fcfs_thread, &thread_data[0]);
    pthread_create(&threads[1], NULL, sstf_thread, &thread_data[1]);
    pthread_create(&threads[2], NULL, scan_thread, &thread_data[2]);
    pthread_create(&threads[3], NULL, cscan_thread, &thread_data[3]);
    pthread_create(&threads[4], NULL, nstep_scan_thread, &thread_data[4]);

    // Wait for all threads to complete
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Compare results
    printf("\nComparison of Algorithms:\n");
    printf("%-15s %-20s %-15s\n", "Algorithm", "Total Movement", "Seek Time (ms)");
    printf("------------------------------------------------\n");
    printf("%-15s %-20d %-15.10f\n", "FCFS", thread_data[0].total_movement, thread_data[0].seek_time);
    printf("%-15s %-20d %-15.10f\n", "SSTF", thread_data[1].total_movement, thread_data[1].seek_time);
    printf("%-15s %-20d %-15.10f\n", "SCAN", thread_data[2].total_movement, thread_data[2].seek_time);
    printf("%-15s %-20d %-15.10f\n", "C-SCAN", thread_data[3].total_movement, thread_data[3].seek_time);
    printf("%-15s %-20d %-15.10f\n", "N-Step-SCAN", thread_data[4].total_movement, thread_data[4].seek_time);

    // Find algorithms with minimum movement and minimum seek time
    int min_movement_idx = 0;
    int min_seek_time_idx = 0;
    
    for (int i = 1; i < 5; i++) {
        // Skip invalid results (zero movement)
        if (thread_data[i].total_movement == 0) continue;
        
        // Update minimum movement
        if (thread_data[i].total_movement < thread_data[min_movement_idx].total_movement ||
            thread_data[min_movement_idx].total_movement == 0) {
            min_movement_idx = i;
        }
        
        // Update minimum seek time
        if (thread_data[i].seek_time < thread_data[min_seek_time_idx].seek_time ||
            thread_data[min_seek_time_idx].seek_time == 0) {
            min_seek_time_idx = i;
        }
    }

    // Display results
    printf("\nBest Algorithm(s):\n");
    if (min_movement_idx == min_seek_time_idx) {
        // Same algorithm is best for both metrics
        const char* algo_name = "";
        switch (min_movement_idx) {
            case 0: algo_name = "FCFS"; break;
            case 1: algo_name = "SSTF"; break;
            case 2: algo_name = "SCAN"; break;
            case 3: algo_name = "C-SCAN"; break;
            case 4: algo_name = "N-Step-SCAN"; break;
        }
        printf("%s is best overall with:\n", algo_name);
        printf("  - Least movement: %d cylinders\n", thread_data[min_movement_idx].total_movement);
        printf("  - Least seek time: %.2f ms\n", thread_data[min_movement_idx].seek_time);
    } else {
        // Different algorithms are best for different metrics
        const char* move_algo = "";
        const char* seek_algo = "";
        switch (min_movement_idx) {
            case 0: move_algo = "FCFS"; break;
            case 1: move_algo = "SSTF"; break;
            case 2: move_algo = "SCAN"; break;
            case 3: move_algo = "C-SCAN"; break;
            case 4: move_algo = "N-Step-SCAN"; break;
        }
        switch (min_seek_time_idx) {
            case 0: seek_algo = "FCFS"; break;
            case 1: seek_algo = "SSTF"; break;
            case 2: seek_algo = "SCAN"; break;
            case 3: seek_algo = "C-SCAN"; break;
            case 4: seek_algo = "N-Step-SCAN"; break;
        }
        printf("%s has least movement: %d cylinders\n", 
               move_algo, thread_data[min_movement_idx].total_movement);
        printf("%s has least seek time: %.2f ms\n", 
               seek_algo, thread_data[min_seek_time_idx].seek_time);
    }

    // Cleanup
    free(data.requests);
    for (int i = 0; i < 5; i++) {
        free(thread_data[i].requests);
    }
    return 0;
} 