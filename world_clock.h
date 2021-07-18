#ifndef _WORLD_CLOCK_H
#define _WORLD_CLOCK_H

#include <pthread.h>

class WorldClock {
private:
    int time;
    
    int externalEvents;
    int externalEventsTotal;
    
    pthread_t thread;
    
    pthread_cond_t tickCond;
    pthread_mutex_t tickMutex;
    
    pthread_cond_t externalCond;
    pthread_mutex_t externalMutex;
    
    bool shouldRun;
    
public:
    WorldClock();
    void start();
    void registerExternalEvent();
    void notifyExternalEvent();
    void unregisterExternalEvent();
    int waitForTick(int existingTime);
    int getTime();
    void terminate();
    
private:
    void cycle();
    
    static void* threadHelper(void* context);
};


#endif