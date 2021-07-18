#include "car.h"
#include "road.h"
#include "cross.h"

#include <string> 
#include <vector>
// Tokenizing a string using stringstream 
#include <bits/stdc++.h> 
//Function for when 

using namespace std;

Cross::Cross(mqtt::async_client* client){
	crossClear = false;
	numOCars = 0;
	time = 0;
    prioID = 0;
	this->client = client;
    this->run = false;
}

void Cross::Cycle(){
	while (run) {
	//Wait to recieve messages
   
    auto msg = client->consume_message();
    printf("%s", msg->to_string());
    if (msg){
 		string msgTemp = msg->to_string();

        vector <string> tokens; 
      
        // stringstream class check1 
        stringstream check1(msgTemp); 
      
        string intermediate; 
      
        while(getline(check1, intermediate, ',')) 
        { 
            tokens.push_back(intermediate); 
        } 
        
        vector <vector<float>> info;
        
        
        //Fill Vector Vecor
        for(unsigned int i = 0; i < tokens.size(); i++){
            vector<float> temp;
            for(int t = 0; t < 5; t++) {
                temp.push_back(0.0);
            }
            info.push_back(temp);
        }
        
        for(unsigned int t = 0; t < tokens.size(); t++) {
            cout << tokens[t] << endl;
        }
        
        //Store Data
        for(unsigned int i = 0; i < tokens.size(); i++){
            sscanf(tokens[i].c_str(),"%f %f %f %f %f", &info[i][0], &info[i][1], &info[i][2], &info[i][3], &info[i][4]);
        }
        
        
        //Update Prio ID
        for(unsigned int j = 0; j < tokens.size(); j++){
            if(info[prioID][3] >= 56 || (info[j][3] >= info[prioID][3] && info[j][3] <= 56)){
                prioID = info[j][1];
            }
        }
        
        //Assign New Speed Values
        for(unsigned int k = 0; k < tokens.size(); k++){
            //If Priorty Keep goin
            if (info[k][1] != prioID){
                if (info[k][3] + info[k][4] > 48){
                    info[k][4] = 48 - info[k][3];
                }
            }
        }

	//Send new car speeds
    stringstream location;
    location << "{";
	for(unsigned int i = 0; i < tokens.size(); i++){
		location << "\"Group#8Car" << info[i][1] << "\": {";
            location << "\"speed\" : " << info[i][4]<< "}";
	} 
    location << "}";
    
    auto msg = mqtt::make_message("v1/gateway/attributes", location.str(), 1, false);
    client->publish(msg);
	
	//Done
	}
    }
}

void Cross::Start(){
    pthread_t* crossThread = new pthread_t();
    run = true;
	pthread_create(crossThread, NULL, threadHelper, this);
}

void* Cross::threadHelper(void* context) {
    Cross* cross = (Cross*) context;
    cross->Cycle();
    
    return NULL;
}

void Cross::terminate(){
    this->run = false;
}

	