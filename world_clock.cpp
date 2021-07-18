#include "world_clock.h"

WorldClock::WorldClock() {
    time = 0;
    
    externalEvents = 0;
    externalEventsTotal = 0;
    
    pthread_cond_init(&tickCond, NULL);
    pthread_mutex_init(&tickMutex, NULL);
    
    pthread_cond_init(&externalCond, NULL);
    pthread_mutex_init(&externalMutex, NULL);
    
    shouldRun = false;
}

void WorldClock::start() {
    shouldRun = true;
    pthread_create(&thread, NULL, WorldClock::threadHelper, this);
}

void WorldClock::registerExternalEvent() {
    pthread_mutex_lock(&externalMutex);
    
    externalEventsTotal += 1;
    externalEvents += 1;
    pthread_cond_broadcast(&externalCond);
    
    pthread_mutex_unlock(&externalMutex);
}

void WorldClock::notifyExternalEvent() {
    pthread_mutex_lock(&externalMutex);
    
    externalEvents -= 1;
    if( externalEvents <= 0 ) {
        pthread_cond_broadcast(&externalCond);
    }
    
    pthread_mutex_unlock(&externalMutex);
}

void WorldClock::unregisterExternalEvent() {
    pthread_mutex_lock(&externalMutex);
    
    externalEventsTotal -= 1;
    externalEvents -= 1;
    
    if(externalEvents <= 0) {
        pthread_cond_broadcast(&externalCond);
    }
    
    pthread_mutex_unlock(&externalMutex);
}

int WorldClock::waitForTick(int existingTime) {
    pthread_mutex_lock(&tickMutex);
    
    while( existingTime > time ) {
        pthread_cond_wait(&tickCond, &tickMutex);
    }
    
    int newTime = time;
    pthread_mutex_unlock(&tickMutex);
    
    return newTime;
}

int WorldClock::getTime() {
    return time;
}

void WorldClock::terminate() {
    shouldRun = false;
    
    pthread_mutex_lock(&externalMutex);
    externalEventsTotal = 0;
    externalEvents = 0;
    pthread_cond_signal(&externalCond);
    pthread_mutex_unlock(&externalMutex);
}



void WorldClock::cycle() {
    timespec waitTime;
    waitTime.tv_sec = 1;
    waitTime.tv_nsec = 0;
    
    clock_t startTime;
    float elapsedTime = 0.0;
    
    while(shouldRun) {
        nanosleep(&waitTime, NULL);
        startTime = clock();
        
        pthread_mutex_lock(&externalMutex);
        while( externalEvents > 0 ) {
            pthread_cond_wait(&externalCond, &externalMutex);
        }
        pthread_mutex_unlock(&externalMutex);
        
        time += 1;
        pthread_mutex_lock(&externalMutex);
        externalEvents = externalEventsTotal;
        pthread_mutex_unlock(&externalMutex);
        
        pthread_cond_broadcast(&tickCond);
        
        elapsedTime = ((float)clock() - startTime)/CLOCKS_PER_SEC;
        waitTime.tv_sec = 0;
        waitTime.tv_nsec = (1.0 - elapsedTime) * 1000000000;
    }
}

void* WorldClock::threadHelper(void* context) {
    WorldClock* clock = (WorldClock*) context;
    clock->cycle();
    
    return NULL;
}