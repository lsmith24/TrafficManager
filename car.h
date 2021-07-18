#ifndef _CAR_H
#define _CAR_H

#include <pthread.h>
#include <mqtt/async_client.h>
#include "world_clock.h"

#define MIN_TARGET_SPEED 10.0
#define MAX_TARGET_SPEED 20.0
#define MIN_WAIT_TIME 0
#define MAX_WAIT_TIME 3
#define SPEED_CHANGE_PERIOD 5
#define BUFFER_SPACING 2.0

class Road;
class Car {
public:
    int id;
    int active;
    Road* start_road;
    Car* forwardCar;
    
private:
    int time;
    WorldClock* clock;
    
    pthread_t thread;
    
    Road* current_road;
    
    float position;
    float prevPosition;
    
    float targetSpeed;
    float speed;
    float prevSpeed;
    
    int timeWaitedToEnter;
    
    pthread_mutex_t positionMutex;
    pthread_cond_t positionCond;
    
    pthread_mutex_t speedMutex;
    pthread_cond_t speedCond;
    
    mqtt::async_client* client;
    
public:
    Car(int id, Road* start_road, mqtt::async_client* client);
    void start();
    float getPosition(int time);
    float getSpeed(int time);
    void setSpeed(float s);
    
private:
    void cycle();
    void enterRoad();
    void waitEnterRoad();
    void drive();
    void enterIntersection();
    void move();
    void adjustSpeed(float speed);
    void changeRoad();
    
    static void* threadHelper(void* context);
};

#endif