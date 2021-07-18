#include "road.h"

#include <mqtt/async_client.h>
#include <iostream>
#include <sstream>

using namespace std;


//  const char *usernames[10] = { "dEbAFpM2yBy9dIadlole", "pWQz4F2GdINRECQQOhh1", "BPkATfb0SZMiFPcV7W52", "Q00AndXXEuB2j8KLRZVb", "AEk60S1AO9mQa5VxfUk0", 
//                              "hDJGprUOHITMbX3axxvt", "owrq16RdiUjlVPpVmrCU", "8Fhe5DMXkDPZc0oJ1ae8", "dnOnfnaylBP2Gw77k59g", "CLBwIHWZrMajQA2mUBrr"};

Road::Road(int numCars, float endPosition, WorldClock* clock, mqtt::async_client* client, RoadIdentifier id, int rd_num, Cross* cross) {
    this->numCars = numCars;
    this->endPosition = endPosition;
    this->clock = clock;
    this->client = client;
    this->id = id;
    this->cross = cross;
    this->rd_num = rd_num;
    
    lastEntered = NULL;
    time = -1;
    entranceOpen = true;
    intersectionOpen = true;
    
    pthread_mutex_init(&entranceMutex, NULL);
    pthread_mutex_init(&turnMutex, NULL);
}

void Road::start(Road* rd, mqtt::async_client* carClient) {
    if (rd_num == 1) {
        for(int i=0; i<numCars; i++) {
          cars.push_back(new Car(i, this, carClient));
        }
    }
    if (rd_num == 2) {
        for(int i=0; i<numCars; i++) {   
        cars.push_back(new Car(i+5, this, carClient));
    }
    }
    
    for(int i=0; i<numCars; i++) {
        auto msg = mqtt::make_message("v1/gateway/connect","{\"device\":\"Group#8Car"+to_string(cars[i]->id)+"\",\"type\":\"ProjectGroup#8\"}",1,false);
        client->publish(msg)->wait();
    }
    
    // Zero out location
    stringstream initialLocation;
    initialLocation << "{";
    for(int i=0; i<numCars; i++) {
        initialLocation << "\"Group#8Car" << cars[i]->id << "\": {";
        initialLocation << "\"latitude\": 0.0,";
        initialLocation << "\"longitude\":0.0,";
        initialLocation << "\"speed\":0.0}";
        
        if(i<numCars-1) {
            initialLocation << ",";
        }
    }
    initialLocation << "}";
    
    for(int i=0; i<numCars; i++) {
        cars[i]->start();
    }
}

bool Road::tryEnterRoad(int time) {
    bool success = false;
    pthread_mutex_lock(&entranceMutex);
    
    if(time > this->time) {
        entranceOpen = true;
        for(int i=0; i<numCars; i++) {
            if(cars[i]->active == 1) {
                //printf("ROAD CHECKING POSITION OF CAR %d at time %d\n", cars[i]->id, time);
                float position = cars[i]->getPosition(time);
                if(position < BUFFER_SPACING) {
                    entranceOpen = false;
                    break;
                }
            }
        }
        
        this->time = time;
        success = entranceOpen;
        entranceOpen = false;
    } else {
        success = false;
    }
    
    pthread_mutex_unlock(&entranceMutex);
    return success;
}

int Road::getCarNum() {
    return this->numCars;
}

int Road::getRdID() {
    return this->id;
}

void Road::setCarNum(int n) {
    this->numCars = n;
}

mqtt::async_client* Road::getClient() {
    return this->client;
}

bool Road::print(int time) {
    int notFinished = numCars;
    
    printf("\n\n==============================================\n");
    printf("ROAD STATUS FOR TIME %d\n", time);
    for(int i=0; i<cars.size(); i++) {
        if(cars[i]->active == 0) {
            notFinished -= 1;
        } else {
            float position = cars[i]->getPosition(time);
            float speed = cars[i]->getSpeed(time);
            printf("CAR %d IS AT POS %f ON ROAD %d GOING %fm/s\n", cars[i]->id, position, cars[i]->start_road->id, speed);
        }
    }
    
    return notFinished == 0;
}

void Road::visualize(int time) {
    stringstream location;
    location << "{";
    bool started = false;
    for(int i=0; i < cars.size(); i++) {
        location << "\"Group#8Car" << cars[i]->id << "\": {";
        if(cars[i]->getPosition(time) <= 100) {
            float position = cars[i]->getPosition(time);
            auto coord = GetLatLon(position, cars[i]->start_road->id);
            if(position >= 30.0 && position < 56.0){
//                 if(!started){
//                     printf("Running Cross Cycle");
//                     cross->Start();
//                     started = true;
//                 }
                //cr = true;
                location << "\"inCross\": " << true << ",";
            } else {
                location << "\"inCross\": " << false << ",";
            }
            location << "\"position\": " << cars[i]->getPosition(time) << ",";
            location << "\"speed\" : " << cars[i]->getSpeed(time) << ",";
            location << "\"latitude\": "<< coord.lat << ",";
            location << "\"longitude\": " << coord.lon << "}";
        } else {
            location << "\"inCross\": " << false << ",";
            location << "\"position\": " << 0.0 << ",";
            location << "\"speed\" : " << 0.0<< ",";
            location << "\"latitude\": 0.0,";
            location << "\"longitude\":0.0}";
        }
        
        if(i<numCars-1) {
            location << ",";
        }
    }
    location << "}";
    
    auto msg = mqtt::make_message("v1/gateway/attributes", location.str(), 1, false);
    client->publish(msg);
    //cout << location.str() << endl;
    
//     if (cr) {
//         auto msg2 = mqtt::make_message("v1/gateway/cross", cross.str(), 1, false);
//         client->publish(msg2);
//     }
}
