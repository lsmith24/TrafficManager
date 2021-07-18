#include <iostream>

#include <mqtt/async_client.h>
#include <vector>

#include "world_clock.h"
#include "car.h"
#include "road.h"
#include "cross.h"

#define NUM_CARS 5
#define END_OF_ROAD 100

using namespace std;
int prioID1 = 0;
int prioID2 = 0;

void update(std::vector<Car*> &cars1, std::vector<Car*>& cars2, int time, Road *road1, Road *road2) {
    int max1 = 0;
    int max2 = 0;
    int flag = 0;
    Car *car1;
    Car *car2;
    for (int i = 0; i < cars1.size(); i++) {
        Car *c = cars1[i];
            if ((c->getPosition(time) + c->getSpeed(time) > max1) && (c->getPosition(time) < 52)) {
                max1 = c->getPosition(time) + c->getSpeed(time);
                prioID1 = c->id;
                car1 = c;
            }
            if (c->getPosition(time) > 30 && c->getPosition(time) < 56) {
                flag = 1;
            }
    }        
    for (int i = 0; i < cars2.size(); i++) {  
        Car *c = cars2[i];
        if ((c->getPosition(time) + c->getSpeed(time) > max2) && (c->getPosition(time) < 52)) {
            max2 = c->getPosition(time) + c->getSpeed(time);
            prioID2 = c->id;
            car2 = c;
         }
         if (c->getPosition(time) > 30.0 && c->getPosition(time) < 56.0) {
             flag = 1;
        }
    }
    cout << "PRINTTTTTTT" << endl;
    if (flag == 1) {
        int c1 = car1->getPosition(time);
        int c2 = car2->getPosition(time);
        if (c1 > c2) {
           if (c2 + car2->getSpeed(time) > 48.0) {
               car2->setSpeed(48.0 - c2);
           }
           if (c1 + car1->getSpeed(time) > 50.0) {
               //Prio car goes to middle
               car1->setSpeed(50.0 - c1);
           }
           if (c1 == 50.0 && (prioID1 % 2 == 0)) {
                cars1.erase(cars1.begin() + prioID1);
              //  Car *prevOld = cars1[prioID1 - 1];
               // prevOld->forwardCar = temp->forwardCar;
               // temp->forwardCar = cars2[prioID2]->forwardCar;
              //  Car *prev = cars2[prioID2];
              //  prev->forwardCar = temp;
                cars2.push_back(car1);
                car1->start_road = road2;
            }
        } else {
            if (c1 + car1->getSpeed(time) > 48.0) {
               car1->setSpeed(48.0 - c1);
           } 
            if (c2 + car2->getSpeed(time) > 50.0) {
               //Prio car goes to middle
               car2->setSpeed(50.0 - c2);
            }
            if (c2 == 50.0 && (prioID2 % 3 == 0)) {
                cars2.erase(cars2.begin() + prioID2);
              //  Car *prevOld = cars2[prioID2 - 1];
              //  prevOld->forwardCar = temp->forwardCar;
              //  temp->forwardCar = cars1[prioID1]->forwardCar;
              //  Car *prev = cars1[prioID1];
              //  prev->forwardCar = temp;
                cars1.push_back(car2);
                car2->start_road = road1;
            }
        }
        flag = 0;
    }
}

int main() {     
    const string tb_uri = {"ssl://tb.precise.seas.upenn.edu:8883"};
    string rd_client_id = {"road_client1"};
    string rd_client_id2 = {"road_client2"};
    string rd_client_id3 = {"road_client3"};
    string rd_client_id4 = {"road_client4"};
    string rd_client_id5 = {"road_client5"};
    string rd_client_id6 = {"road_client6"};
    string rd_client_id7 = {"road_client7"};
    string rd_client_id8 = {"road_client8"};
    string rd_client_id9 = {"road_client9"};
    string rd_client_id10 = {"road_client10"};
    string cross_client_id = {"cross_client"};
    string car_client_id = {"car_client"};
    string username1 = {"pSsG7Q85RGsMtxgiPuNc"};
    string username2 = {"66CADtFeihdqzp3x6IXm"};
    string username3 = {"0HUeEn80YWXiHB8pJzwa"};
    string username4 = {"lZQWkVV3ljypHr21KgNh"};
    string username5 = {"YFfyedQeNX51rmJSmrQy"};
    string username6 = {"GHTS1XyF71Dt88p7z3AH"};
    string username7 = {"m5LTzbv2PrysgAkVSuKt"};
    string username8 = {"CEmCaPzcdwIXTeRqTzL6"};
    string username9 = {"6qe9AmVMY44ULdeqX1P8"};
    string username10 = {"z827Gy7iSdGNtObKp6C3"};
    string username_cr = {"ZFXFaa3UdGxGUq130irz"};
    string username_car = {"Uj1PL5juOZtw9mDcmcfw"};
    string password = {""};
    
    // Init Client
    mqtt::async_client client1(tb_uri, rd_client_id);
    mqtt::async_client client2(tb_uri, rd_client_id2);
 //   mqtt::async_client client3(tb_uri, rd_client_id3);
  //  mqtt::async_client client4(tb_uri, rd_client_id4);
  //  mqtt::async_client client5(tb_uri, rd_client_id5);
  //  mqtt::async_client client6(tb_uri, rd_client_id6);
   // mqtt::async_client client7(tb_uri, rd_client_id7);
   // mqtt::async_client client8(tb_uri, rd_client_id8);
   // mqtt::async_client client9(tb_uri, rd_client_id9);
   // mqtt::async_client client10(tb_uri, rd_client_id10);
    mqtt::async_client client_cross(tb_uri, cross_client_id);
    mqtt::async_client client_car(tb_uri, car_client_id);
    
    // Username & Password
    mqtt::connect_options connopts(username1, password);
    mqtt::connect_options connopts2(username2, password);
   // mqtt::connect_options connopts3(username3, password);
  //  mqtt::connect_options connopts4(username4, password);
  //  mqtt::connect_options connopts5(username5, password);
  //  mqtt::connect_options connopts6(username6, password);
  //  mqtt::connect_options connopts7(username7, password);
   // mqtt::connect_options connopts8(username8, password);
  //  mqtt::connect_options connopts9(username9, password);
  //  mqtt::connect_options connopts10(username10, password);
    mqtt::connect_options connopts_cr(username_cr, password);
    mqtt::connect_options connopts_car(username_car, password);
    
    // Enable SSL
    mqtt::ssl_options sslopts;
    connopts.set_ssl(sslopts);
    connopts2.set_ssl(sslopts);
  //  connopts3.set_ssl(sslopts);
  //  connopts4.set_ssl(sslopts);
  //  connopts5.set_ssl(sslopts);
  //  connopts6.set_ssl(sslopts);
    //connopts7.set_ssl(sslopts);
   // connopts8.set_ssl(sslopts);
   // connopts9.set_ssl(sslopts);
   // connopts10.set_ssl(sslopts);
    connopts_cr.set_ssl(sslopts);
    connopts_car.set_ssl(sslopts);

    
    // Perform connection
    client1.connect(connopts)->wait();
    client2.connect(connopts2)->wait();
  //  client3.connect(connopts3)->wait();
  //  client4.connect(connopts4)->wait();
  //  client5.connect(connopts5)->wait();
  //  client6.connect(connopts6)->wait();
   // client7.connect(connopts7)->wait();
  //  client8.connect(connopts8)->wait();
  //  client9.connect(connopts9)->wait();
  //  client10.connect(connopts10)->wait();
    client_cross.connect(connopts_cr)->wait();
    client_car.connect(connopts_car)->wait();
    
    client_cross.start_consuming();
    client_cross.subscribe("v1/gateway/attributes", 1);
    client_car.start_consuming();
    client_car.subscribe("v1/gateway/attributes", 1);
    
    WorldClock clock;
    Cross cross(&client_cross);
    Road road1(NUM_CARS, END_OF_ROAD, &clock, &client1, RoadIdentifier::BC, 1, &cross);
    Road road2(NUM_CARS, END_OF_ROAD, &clock, &client2, RoadIdentifier::DE, 2, &cross);
  //  Road road3(NUM_CARS, END_OF_ROAD, &clock, &client3, RoadIdentifier::AB, 3, &cross);
  //  Road road4(NUM_CARS, END_OF_ROAD, &clock, &client4, RoadIdentifier::EF, 4, &cross);
  //  Road road5(NUM_CARS, END_OF_ROAD, &clock, &client5, RoadIdentifier::GH, 5, &cross);
  //  Road road6(NUM_CARS, END_OF_ROAD, &clock, &client6, RoadIdentifier::HI, 6, &cross);
  //  Road road7(NUM_CARS, END_OF_ROAD, &clock, &client7, RoadIdentifier::IH, 7, &cross);
  //  Road road8(NUM_CARS, END_OF_ROAD, &clock, &client8, RoadIdentifier::HG, 8, &cross);
  //  Road road9(NUM_CARS, END_OF_ROAD, &clock, &client9, RoadIdentifier::JK, 9, &cross);
  //  Road road10(NUM_CARS, END_OF_ROAD, &clock, &client10, RoadIdentifier::KL, 10, &cross);
    
    clock.registerExternalEvent();
    
    road1.start(&road2, &client_car);
    road2.start(&road1, &client_car);
  //  road3.start(&road1, &client_car);
  //  road4.start(&road5, &client_car);
  //  road5.start(&road4, &client_car);
  //  road6.start(&road6, &client_car);
  //  road7.start(&road9, &client_car);
  //  road8.start(&road4, &client_car);
  //  road9.start(&road7, &client_car);
  //  road10.start(&road6, &client_car);
    clock.start();
    cross.Start();
        
    int time = 0;
    while(!road1.print(time) && !road2.print(time)) {
       update(road1.cars, road2.cars, time, &road1, &road2);
    //    update(road3.cars, road1.cars, time, &road3, &road1);
    //    update(road4.cars, road5.cars, time, &road4, &road5);
       // update(road6.cars, road10.cars, time, &road6, &road10);
       // update(road7.cars, road9.cars, time, &road7, &road9);
      //  update(road8.cars, road4.cars, time, &road8, &road4);
        road1.visualize(time);
        road2.visualize(time);
      //  road3.visualize(time);
      //  road4.visualize(time);
      //  road5.visualize(time);
      //  road6.visualize(time);
       // road7.visualize(time);
      //  road8.visualize(time);
      //  road9.visualize(time);
      //  road10.visualize(time);
        clock.notifyExternalEvent();
        time = clock.waitForTick(time+1);
    }
    
    cross.terminate();
    clock.terminate();
    client1.disconnect()->wait();
    client2.disconnect()->wait();
  //  client3.disconnect()->wait();
  //  client4.disconnect()->wait();
  //  client5.disconnect()->wait();
  //  client6.disconnect()->wait();
   // client7.disconnect()->wait();
   // client8.disconnect()->wait();
  //  client9.disconnect()->wait();
  //  client10.disconnect()->wait();

    return 0;
}