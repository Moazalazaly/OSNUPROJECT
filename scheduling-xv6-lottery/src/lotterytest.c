#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int i;
    int pid;
    // Ticket counts for the three child processes
    int tickets[] = {10, 20, 30};

    for (i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            // Set the lottery tickets for this process
            // Note: Ensure 'settickets' matches your custom syscall name in user.h
            settickets(tickets[i]);

            // Record start time in ticks
            int start = uptime();

            // CPU-bound loop to force the scheduler to run
            // 'volatile' prevents the compiler from optimizing this loop away
            volatile int dummy = 0;
            for (int j = 0; j < 200000000; j++) {
                dummy += j;
            }

            // Record end time
            int end = uptime();
            int runtime = end - start;

            // Print data in a strict parseable format:
            // FORMAT: RESULT: pid=<id> tickets=<count> runtime=<ticks>
            printf(1, "RESULT: pid=%d tickets=%d runtime=%d\n", getpid(), tickets[i], runtime);

            exit();
        }
    }

    // Parent waits for all children to finish
    while (wait() != -1);
    printf(1, "Test Complete\n");
    exit();
}
