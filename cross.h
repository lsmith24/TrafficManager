#ifndef _CROSS_H
#define _CROSS_H

#include <pthread.h>
#include <mqtt/async_client.h>
#include <vector>
#include "world_clock.h"

class Cross {
	public:
        Cross(mqtt::async_client* client);
        void Start();
        void Cycle();
        static void* threadHelper(void* context);
        void terminate();
	private: 
		bool crossClear;
		int numOCars;
		int time; 
        int prioID;
        bool run;
		std::vector<Car*> cars;
		mqtt::async_client* client;
};

#endif