/* Example code for starting
 * 2 threads and controlling 
 * their run-time frequency.
 *
 * Author: Chris Womack
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <util/util.h>
#include <sys/time.h>

// declare threads (global space)
pthread_t thread1;
pthread_t thread2;

// forward declare thread mains
void thread1_critical_section(void);
void thread2_critical_section(void);

// sig handler for sig int to kill threads gracefully 
void sig_handler(int signum) {
    if (signum != SIGINT) {
        printf("Received invalid signum = %d in sig_handler()\n", signum);
        ASSERT(signum == SIGINT);
    }

    printf("Received SIGINT. Executing pthread_cancel on thread 1 & 2\n");

    // send cancel request to threads
    pthread_cancel(thread1);
    pthread_cancel(thread2);
}

int main(void) {
    pthread_attr_t attr; // attribute structure
    int status;
 
    signal(SIGINT, sig_handler); // handles SIGINT

    pthread_attr_init(&attr);   // initializes attribute structure

    // pthread has functions to set diff attr in attr struct
    pthread_attr_setstacksize(&attr, 1024*1024); // set stack size to 1MB
    
    // create thread 1
    printf("Creating thread1\n");
    status = pthread_create(&thread1, &attr, (void*)&thread1_critical_section, NULL);
    if (status != 0) {
        printf("Failed to create thread1 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    // create thread 2
    printf("Creating thread2\n");
    status = pthread_create(&thread2, &attr, (void*)&thread2_critical_section, NULL);
    if (status != 0) {
        printf("Failed to create thread2 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    // wait until thread 1 and 2 complete 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nReturned to main after pthread_join on thread 1 & 2\n"); 

    return 0;
}

void thread1_critical_section(void) {
    unsigned int run_cnt;
    unsigned int exec_period_usecs;
    struct timeval ts;    

    exec_period_usecs = 1000000; /*in micro-seconds*/
    printf("Thread 1 started. Execution period = %d uSecs\n", exec_period_usecs);

    run_cnt = 0;
    while(1) {
        usleep(exec_period_usecs);
        gettimeofday(&ts, NULL);
        printf("000[THREAD 1] Executing critical section of Thread1. ");
        printf("%06lu.%06lu: Thread1 run_cnt = %d\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_usec, run_cnt);
        run_cnt += 1;
    }
}


void thread2_critical_section(void) {
    unsigned int run_cnt;
    unsigned int exec_period_usecs;
    struct timeval ts;    

    exec_period_usecs = 5000000; /*in micro-seconds*/
    
    printf("Thread 2 started. Execution period = %d uSecs\n",\
                                           exec_period_usecs);
    run_cnt = 0;
    while(1) {
        usleep(exec_period_usecs);
        gettimeofday(&ts, NULL);
        printf("\nXXX [THREAD 2] Executing critical section of Thread2 ");
        printf("%06lu.%06lu: Thread2 run_cnt = %d\n\n", (unsigned int)ts.tv_sec, (unsigned int)ts.tv_usec, run_cnt);
        run_cnt += 1;
    }
}
