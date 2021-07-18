#ifndef _ROAD_H
#define _ROAD_H

#include <pthread.h>
#include <vector>

#include <mqtt/async_client.h>


#include "car.h"
#include "world_clock.h"
#include "vc2gps.h"
#include "cross.h"

class Road {
public:
    Car* lastEntered;
    WorldClock* clock;
    float endPosition;
    std::vector<Car*> cars;
    Cross* cross;
    
private:
    int numCars;
    int time;
    bool entranceOpen;
    bool intersectionOpen;
    pthread_mutex_t entranceMutex;
    pthread_mutex_t turnMutex;
    
    mqtt::async_client* client;
    RoadIdentifier id;
    int rd_num;
    
public:
    Road(int numCars, float endPosition, WorldClock* clock, mqtt::async_client* client, RoadIdentifier id, int rd_num, Cross* cross);
    void start(Road* rd, mqtt::async_client* carClient);
    bool tryEnterRoad(int time);
    bool print(int time);
    void visualize(int time);
    int getCarNum();
    void setCarNum(int n);
    mqtt::async_client* getClient();
    int getRdID();
};

#endif