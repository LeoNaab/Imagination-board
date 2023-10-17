#include <Arduino.h>


// put function declarations here:
const int LED = 14;
const int LED_SHIFT = 103;
// const int LED_LOW = 103;
// const int LED_HIGH = 104;
const int S_OK  = 0xaa;
const int MOTOR_LOW = 0;
const int MOTOR_HIGH = 100;
const int MOTOR_CLK=101;
const int MOTOR_COUNTERCLK=102;
const int MOTOR_1A = 3;
const int MOTOR_1B = 5; 
const int S_ERR = 0xff;
int i = 0;

void on_receive(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) { 
  // read one byte
  int state = USBSerial.read();
  int speed = 0;
  bool clockwise;

  if(state >= MOTOR_LOW && state <= MOTOR_HIGH){
  //  if(clockwise == true)
    speed = state;
  //  if(clockwise == false)
  //   //invert speed 
  //   speed = 100 - state;
    USBSerial.write(S_OK);
  }
  else if(state == MOTOR_CLK){
    clockwise = true; //telling us for later that we are currently moving clockwise so we can use "speed" directly
    digitalWrite(MOTOR_1B,LOW);
    USBSerial.write(S_OK);
  } 
  else if(state == MOTOR_COUNTERCLK){
    clockwise = false;
    digitalWrite(MOTOR_1B, HIGH); 
    USBSerial.write(S_OK);
  }
  // guard byte is valid LED state
  else if ((state == LED_SHIFT || state == LED_SHIFT + HIGH)) {
    // update LED with valid state
    digitalWrite(LED, state - LED_SHIFT); //since our byte code is different now, we want 0 to be off and 1 to be on?
    USBSerial.write(S_OK);
  }
  else {
    // invalid byte received
    // what else should we do?
    USBSerial.write(S_ERR);
  }
  
  //PWM, make sure to use the 
  if(clockwise){
    analogWrite(MOTOR_1A, speed);
  }
  else{
    analogWrite(MOTOR_1A, MOTOR_HIGH - speed);
  }
  
  
  return;
 }

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(MOTOR_1A, OUTPUT);
  pinMode(MOTOR_1B, OUTPUT);
  
  USBSerial.begin(9600);

  // register "on_receive" as callback for RX event
  USBSerial.onEvent(ARDUINO_HW_CDC_RX_EVENT, on_receive);
  USBSerial.begin(9600);
}


void loop() {

}
