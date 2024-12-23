#ifndef DISK_SCHEDULING_H
#define DISK_SCHEDULING_H
#define SEEK_TIME_PER_CYLINDER 0.1
typedef struct {
    int* requests;
    int n;
    int head;
    int disk_size;
    int direction;
    int step_size;
    int total_movement;
    double seek_time;  // assuming seek time in milliseconds
} DiskSchedulingData;

void* fcfs_thread(void* arg);
void* sstf_thread(void* arg);
void* scan_thread(void* arg);
void* cscan_thread(void* arg);
void* nstep_scan_thread(void* arg);

#endif 