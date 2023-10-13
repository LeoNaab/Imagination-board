#include <Arduino.h>

// put function declarations here:
const int LED = 14;
int i = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  USBSerial.begin(9600);
}

void loop() {
  if(i<5){
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
    i++;
  }
  USBSerial.println("Hello, World! make way for the MTN climbers JK");
  delay(1000);
  // put your main code here, to run five times:
}
