#include "car.h"
#include "road.h"

#include <iostream>
#include <ctime>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Car::Car(int id, Road* start_road, mqtt::async_client* client) {
    this->id = id;
    active = 0;
    
    time = 0;
    clock = start_road->clock;
    
    this->start_road = start_road;
    this->current_road = start_road;
    this->client = client;
    
    position = -1.0f;
    prevPosition = -1.0f;
    
    targetSpeed = 0.0f;
    prevSpeed = 0.0f;
    speed = 0.0f;
    
    timeWaitedToEnter = 0;
    
    forwardCar = NULL;
    
    pthread_mutex_init(&positionMutex, NULL);
    pthread_cond_init(&positionCond, NULL);
    
    pthread_mutex_init(&speedMutex, NULL);
    pthread_cond_init(&speedCond, NULL);
    
    clock->registerExternalEvent();
}

void Car::start() {
    active = 2;
    pthread_create(&thread, NULL, Car::threadHelper, this);
}


float Car::getPosition(int time) {
    float position = -1.0f;
    
    pthread_mutex_lock(&positionMutex);
    if(time == this->time) {
        position = this->position;
    } else if(time == this->time - 1) {
        position = this->prevPosition;
    } else if(time == this->time+1) {
        pthread_cond_wait(&positionCond, &positionMutex);
        position = this->position;
    } else{
        printf("This should never happen. Requested position for time %d but Car %d is at time %d\n", time, id, this->time);
    }
    pthread_mutex_unlock(&positionMutex);
    
    return position;
}

void Car::setSpeed(float s) {
    pthread_mutex_lock(&speedMutex);
    this->speed = s;
    pthread_mutex_unlock(&speedMutex);
}

float Car::getSpeed(int time) {
    float speed = -1.0f;
    
    pthread_mutex_lock(&speedMutex);
    if(time == this->time) {
        speed = this->speed;
    } else if(time == this->time - 1) {
        speed = this->prevSpeed;
    } else if(time == this->time+1) {
        pthread_cond_wait(&speedCond, &speedMutex);
        speed = this->speed;
    } else{
        printf("This should never happen. Requested speed for time %d but Car %d is at time %d\n", time, id, this->time);
    }
    pthread_mutex_unlock(&speedMutex);
    
    return speed;
}

void Car::cycle() {
    srand(std::time(NULL)); 
    
    time = clock->getTime();
    
    enterRoad();
    drive();
       
    clock->waitForTick(time);
    clock->unregisterExternalEvent();
    
    active = 0;
}

void Car::enterRoad() {
    while(active == 2) {
        waitEnterRoad();
        if(start_road->tryEnterRoad(time)) {
            position = 0.0f;
            forwardCar = start_road->lastEntered;
            start_road->lastEntered = this;
            active = 1;            
        }
    }
    clock->notifyExternalEvent();
}

void Car::changeRoad() {
  /*  current_road->setCarNum(current_road->getCarNum() - 1);
    (current_road->cars).erase(current_road->cars.begin()+this->id);
    forwardCar = dest_road->lastEntered;
    dest_road->lastEntered = this;
    (dest_road->cars).push_back(this);
    current_road = dest_road; 
    dest_road->setCarNum(current_road->getCarNum() + 1);*/
    
   // string t = to_string(this->id);
   // auto m = mqtt::make_message("v1/gateway/connect","{\"device\":\"Group#8Car"+t+"\",\"type\":\"ProjectGroup#8\"}",1,false);
   // current_road->getClient()->publish(m)->wait();
}

void Car::enterIntersection() {
 /*   while(active == 2) {
        if(dest_road->tryEnterIntersection(time)) {
            position = 0.0f;
            if (current_road->getRdID() != dest_road->getRdID()) {
                changeRoad();
            }           
            active = 1;            
        }
        waitEnterRoad();
    }
    clock->notifyExternalEvent();
    clock->waitForTick(time);*/
}

void Car::waitEnterRoad() {
    int waitTime = rand() % (MAX_WAIT_TIME - MIN_WAIT_TIME) + MIN_WAIT_TIME;
    while(waitTime > 0) {
        time++;
        
        clock->notifyExternalEvent();
        clock->waitForTick(time);
        timeWaitedToEnter += 1;
        waitTime -= 1;
        
    }
}


void Car::drive() {
    targetSpeed = MIN_TARGET_SPEED + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(MAX_TARGET_SPEED-MIN_TARGET_SPEED)));
    int timeCount = SPEED_CHANGE_PERIOD;
    
                        
    float speedUpdate = 0.0f;
    while(position < road->endPosition) {
        
        //Wait for clock then move
        clock->waitForTick(time);
        move();
        
        //Do speed calc for next loop
        
        if (position > 30.0 && position < 52.0) {
            //Wait for message from Cross Man
            //Decode Message
            //Set New Speed
            client->subscribe("v1/gateway/attributes", 1);
            client->start_consuming();
           
            //uto msg = client->consume_message();
            
//             if (msg){
//                 string msgTemp = msg->to_string();

//                 vector <string> tokens; 
      
//                 // stringstream class check1 
//                 stringstream check1(msgTemp); 
      
//                 string intermediate; 
      
//                 while(getline(check1, intermediate, ',')) 
//                 { 
//                     tokens.push_back(intermediate); 
//                 } 
        
//                 vector <vector<float>> info;
                
//                 for(unsigned int i = 0; i < tokens.size(); i++){
//                    vector<float> temp;
//                     for(int t = 0; t < 5 ; t++) {
//                         temp.push_back(0.0);
//                     }
//                    info.push_back(temp);
//                 }
        
//                 //Store Data
//                 for(unsigned int i = 0; i < tokens.size(); i++){
//                     sscanf(tokens[i].c_str(),"%f %f %f %f %f", &info[i][0], &info[i][1], &info[i][2], &info[i][3], &info[i][4]);
//                 }
                
//                 targetSpeed = info[this->id][4];
//             }
    
        } else {
            //Free Drive
            //Randomly change speed every SPEED_CHANGE_PERIOD
            timeCount -= 1;
        
            if(timeCount <= 0) {
                targetSpeed = MIN_TARGET_SPEED + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(MAX_TARGET_SPEED-MIN_TARGET_SPEED)));
                timeCount = SPEED_CHANGE_PERIOD;
            }
        }
        
        speedUpdate = targetSpeed;
		//Check if car is too close
        if(forwardCar != NULL && forwardCar->active) {
            float forwardPosition = forwardCar->getPosition(time);
            float distance = forwardPosition - position - BUFFER_SPACING;
            
            if(distance < speedUpdate) {
                //printf("CAR %d SLOWING DOWN\n", this->id);
                speedUpdate = distance;
            }
        }
        
		//Change speed
        adjustSpeed(speedUpdate);
        //printf("CAR %d POSITION %f SPEED %f at time %d\n", this->id, this->position, this->speed, time);
        time++;

        clock->notifyExternalEvent();
    }
}

void Car::move() {
    pthread_mutex_lock(&positionMutex);
    time = clock->getTime();
    prevPosition = position;
    position += speed;
    pthread_mutex_unlock(&positionMutex);
    pthread_cond_broadcast(&positionCond);
}

void Car::adjustSpeed(float speed) {
    pthread_mutex_lock(&speedMutex);
    prevSpeed = this->speed;
    this->speed = speed;
    pthread_mutex_unlock(&speedMutex);
    pthread_cond_broadcast(&speedCond);
}

void* Car::threadHelper(void* context) {
    Car* car = (Car*) context;
    car->cycle();
    
    return NULL;
}