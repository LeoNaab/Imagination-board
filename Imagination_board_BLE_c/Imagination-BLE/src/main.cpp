#include<Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h> //esp32 ble library
#include <BLEServer.h>
const int READPIN = 46; //define pin here??
const char* SERVICE_UUID = "48d3c0fa-41ee-4494-a8d5-3a6658544e4f"; //how come this many bytes?
const char* VALUE_UUID = "ab947fd2-b81f-4e07-8228-bae3b68fca36";

class ServerCallbacks: public BLEServerCallbacks{
    void onConnect(BLEServer* pServer){
      Serial.println("Client connected."); //someone connected to our serveice when this prints, since onConnect will trigger this to print
    }
    void onDisconnect(BLEServer* pServer){
      Serial.println("Client disconnected.");
      BLEDevice::startAdvertising(); //to make sure our client sees the device even if we are not connected 
    }
  };
  class Data_charic_Callback: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic){
      //not sure what to put here???!!! we want the user to be able to read this
      int sensorValue = analogRead(READPIN);//define pin???
      
    }
  };

void setup() {
  pinMode(READPIN,OUTPUT);//sets up connection between the potentiometer on the board and the program
  BLEDevice::init("NUMBER TESTER"); //namespace with a freestanding function called emit
  BLEServer *pServer = BLEDevice::createServer(); //pointer because its an address t 

  
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *data_charic = pService->createCharacteristic(
    VALUE_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  
  pServer->setCallbacks(new ServerCallbacks()); //kind of declaring a function
  data_charic->setCallbacks(new Data_Charic()); //setting callbacks so we can access that part of the object to read it 
//the callbacks tells server what to do when specific events occur, 
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); //We don't know if these are correct for PC bluetooth
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  
} //created a characteristic for us to read when we connect on the phone, it is public so anyone 

  
  

void loop() {
  // put your main code here, to run repeatedly:

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}