#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

int main()
{
    Scheduler* scheduler = createScheduler();
    
    printf("FCFS Process Scheduler Simulator\n");
    printf("=================================\n\n");
    printf("Enter process details in format: <name> <pid> <burst_time> <io_start> <io_duration>\n");
    printf("For processes without I/O, use '-' for io_start and io_duration\n");
    printf("Enter KILL events in format: KILL <pid> <kill_time>\n");
    printf("Type 'RUN' to start the simulation\n\n");
    
    char input[200];
    
    while (1)
    {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) 
            break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "RUN") == 0 || strcmp(input, "run") == 0)
        {
            break;
        }
        
        char command[20];
        sscanf(input, "%s", command);
        
        if (strcmp(command, "KILL") == 0 || strcmp(command, "kill") == 0)
        {
            int pid, killTime;
            if (sscanf(input, "%*s %d %d", &pid, &killTime) == 2)
            {
                addKillEvent(scheduler, pid, killTime);
            }
        }
        else
        {
            char name[50];
            int pid, burstTime;
            char ioStartStr[10], ioDurationStr[10];
            
            if (sscanf(input, "%s %d %d %s %s", name, &pid, &burstTime, ioStartStr, ioDurationStr) == 5)
            {
                int ioStart = 0;
                int ioDuration = 0;
                
                if (strcmp(ioStartStr, "-") != 0)
                {
                    ioStart = atoi(ioStartStr);
                }
                
                if (strcmp(ioDurationStr, "-") != 0)
                {
                    ioDuration = atoi(ioDurationStr);
                }
                
                addProcess(scheduler, pid, name, burstTime, ioStart, ioDuration);
            }
        }
    }
    
    printf("\n========== Starting Scheduler ==========\n");
    runScheduler(scheduler);
    printResults(scheduler);
    
    freeScheduler(scheduler);
    
    return 0;
}
