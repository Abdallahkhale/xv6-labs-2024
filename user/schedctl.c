#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_usage() {
    printf("Usage: schedctl [command] [args]\n");
    printf("Commands:\n");
    printf("  set <policy>     - Set scheduling policy (0=RR, 1=FCFS, 2=SJF, 3=PRIORITY)\n");
    printf("  get              - Get current scheduling policy\n");
    printf("  priority <pid> <prio> - Set process priority (0-31)\n");
    printf("  stats            - Show scheduler statistics\n");
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        print_usage();
        exit(1);
    }
    
    if(strcmp(argv[1], "set") == 0) {
        if(argc != 3) {
            print_usage();
            exit(1);
        }
        int policy = atoi(argv[2]);
        if(setscheduler(policy) < 0) {
            printf("Failed to set scheduler policy\n");
            exit(1);
        }
        printf("Scheduler policy set successfully\n");
    }
    else if(strcmp(argv[1], "get") == 0) {
        int policy = getscheduler();
        char *names[] = {"Round Robin", "FCFS", "SJF", "Priority", "MLFQ"};
        if(policy >= 0 && policy <= 4) {
            printf("Current policy: %s (%d)\n", names[policy], policy);
        } else {
            printf("Unknown policy: %d\n", policy);
        }
    }
    else if(strcmp(argv[1], "priority") == 0) {
        if(argc != 4) {
            print_usage();
            exit(1);
        }
        int pid = atoi(argv[2]);
        int priority = atoi(argv[3]);
        if(setpriority(pid, priority) < 0) {
            printf("Failed to set priority\n");
            exit(1);
        }
        printf("Priority set successfully\n");
    }
    else if(strcmp(argv[1], "stats") == 0) {
        struct schedstats stats;
        if(schedstats(&stats) < 0) {
            printf("Failed to get scheduler statistics\n");
            exit(1);
        }
        
        printf("=== Scheduler Statistics ===\n");
        printf("Total processes: %d\n", (int)stats.total_processes);
        printf("Active processes: %d\n", stats.active_processes);
        printf("Runnable processes: %d\n", stats.runnable_processes);
        printf("Context switches: %d\n", (int)stats.total_context_switches);
        printf("Scheduler calls: %d\n", (int)stats.scheduler_calls);
        
        char *names[] = {"Round Robin", "FCFS", "SJF", "Priority", "MLFQ"};
        int policy = stats.current_policy;
        if(policy >= 0 && policy <= 4) {
            printf("Current policy: %s\n", names[policy]);
        }
    }
    else {
        print_usage();
        exit(1);
    }
    
    exit(0);
}
