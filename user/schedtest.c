#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// CPU-bound task
void cpu_task(int duration) {
    printf("CPU task %d starting\n", getpid());
    int start = uptime();
    volatile int x = 0;
    
    while(uptime() - start < duration) {
        for(int i = 0; i < 100000; i++) {
            x = x + 1;
        }
    }
    printf("CPU task %d finished\n", getpid());
}

// I/O-bound task simulation
void io_task(int duration) {
    printf("I/O task %d starting\n", getpid());
    int start = uptime();
    
    while(uptime() - start < duration) {
        sleep(2); // Simulate I/O wait
        // Small CPU burst
        volatile int sum = 0;
        for(int i = 0; i < 1000; i++) {
            sum += i; // Actually use the variable to prevent optimization
        }
        // Use sum to prevent compiler from optimizing away the loop
        if(sum < 0) printf("Debug: %d\n", sum); // This will never execute but prevents warning
    }
    printf("I/O task %d finished\n", getpid());
}

int main(int argc, char *argv[]) {
    printf("Scheduler Test Starting...\n");
    
    // Test with current scheduler
    printf("Creating test processes...\n");
    
    for(int i = 0; i < 3; i++) {
        int pid = fork();
        if(pid == 0) {
            // Child process
            if(i == 0) {
                setpriority(getpid(), 5);  // High priority
                cpu_task(10);
            } else if(i == 1) {
                setpriority(getpid(), 15); // Medium priority
                io_task(15);
            } else {
                setpriority(getpid(), 25); // Low priority
                cpu_task(8);
            }
            exit(0);
        } else if(pid < 0) {
            printf("Fork failed\n");
        } else {
            printf("Created process %d\n", pid);
        }
    }
    
    // Wait for all children
    for(int i = 0; i < 3; i++) {
        int status;
        wait(&status);
    }
    
    printf("All test processes completed\n");
    
    // Show final statistics
    struct schedstats stats;
    if(schedstats(&stats) == 0) {
        printf("\n=== Final Statistics ===\n");
        printf("Total context switches: %d\n", (int)stats.total_context_switches);
        printf("Scheduler calls: %d\n", (int)stats.scheduler_calls);
    }
    
    exit(0);
}
