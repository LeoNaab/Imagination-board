#include<Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h> 
#include <BLEServer.h> 

const int READPIN = A0; //define analog pin for reading sensor values (potentiometer in our case)
const char* SERVICE_UUID = "48d3c0fa-41ee-4494-a8d5-3a6658544e4f"; //from uuid generator online
const char* VALUE_UUID = "ab947fd2-b81f-4e07-8228-bae3b68fca36"; //from uuid characteristic 

class ServerCallbacks: public BLEServerCallbacks{
    void onConnect(BLEServer* pServer){ 
       // define code you want when a central device is connected to the service 
    }
    void onDisconnect(BLEServer* pServer){
      BLEDevice::startAdvertising(); //advertise our server for any central device as discoverable
    }
  };
  class Data_charic_Callback: public BLECharacteristicCallbacks {  //data charic the characteristic inherits the BLECharacteristic class
    void onRead(BLECharacteristic *pCharacteristic){ 
      int sensorValue = analogRead(READPIN);//send reading value to sensorValue 
      pCharacteristic->setValue(sensorValue); //sets value of characteristic to be SensorValue read from above
    }
  };

void setup() {
  pinMode(READPIN,OUTPUT);//sets up connection between the potentiometer on the board and the program
  BLEDevice::init("NUMBER TESTER"); //sets the name the periphel device is advertised as
  BLEServer *pServer = BLEDevice::createServer(); //creates server using BLE library 
  
  
  BLEService *pService = pServer->createService(SERVICE_UUID); // pService is created by creatService, a part of the pServer method
  BLECharacteristic *data_charic = pService->createCharacteristic( 
    VALUE_UUID, 
    BLECharacteristic::PROPERTY_READ
  ); //create characteristic and set its UUID as well as the properties it will have (can be read, notify, etc.)
  
  pServer->setCallbacks(new ServerCallbacks()); //Pserver uses setcallbacks function to instantiate a ServerCallbacks object and return a pointer to the instantiation
  data_charic->setCallbacks(new Data_charic_Callback()); //setting callbacks so we can access that part of the object to read it 
//the callbacks tells server what to do when specific events occur, 
  pService->start(); //sets up a new callback thread 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); //defines a pointer to get to advertising
  pAdvertising->addServiceUUID(SERVICE_UUID); //creates a service with the uuid we set
  pAdvertising->setScanResponse(true); //makes our service scanable?
  pAdvertising->setMinPreferred(0x06); //help with iPhone connection issues
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising(); //begins advertising our server

  
} // public to anyone 


void loop() {
  // put any of your other main code here, to run repeatedly:
  
}
