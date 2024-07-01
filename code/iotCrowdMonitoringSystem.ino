#include "thingProperties.h"
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <Servo.h>

#define In_Servo_Pin 9
#define In_Sensor_Pin 7
#define Out_Servo_Pin 10
#define Out_Sensor_Pin 8

ArduinoLEDMatrix matrix;

Servo inServo;
Servo outServo;

int personCount = 0;
int gateOpenDelay = 3000;


const uint32_t happy[] = {
	0x19819,
	0x80000001,
	0x81f8000
};


void setup() {

  Serial.begin(9600);

  delay(1500); 

   pinMode(In_Sensor_Pin, INPUT);
  pinMode(Out_Sensor_Pin, INPUT);

  inServo.attach(In_Servo_Pin);
  outServo.attach(Out_Servo_Pin);

 
  matrix.begin();

  matrix.loadFrame(happy);
  delay(1000);

  // Initialize the display with the initial count
  updateDisplay();

   // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();


}

void loop() {
  ArduinoCloud.update();

  // Check for incoming person
  if (digitalRead(In_Sensor_Pin) == LOW) {
    openGate(inServo);
    personCount++;
    updateDisplay();
    delay(gateOpenDelay); // Delay to allow the person to pass through
    closeGate(inServo);
    crowdCount = personCount; // crowdCount is the Cloud Variable created in Arduino Cloud
  }

  // Check for outgoing person
  if (digitalRead(Out_Sensor_Pin) == LOW) {
    openGate(outServo);
    personCount--;
    updateDisplay();
    delay(gateOpenDelay); // Delay to allow the person to pass through
    closeGate(outServo);
    crowdCount = personCount;// crowdCount is the Cloud Variable created in Arduino Cloud
  }
  
}

void openGate(Servo &servo) {
  servo.write(0); 
}

void closeGate(Servo &servo) {
  servo.write(90); 
}

void updateDisplay() {
  matrix.clear();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(55);

  // Display the count
  char text[20];
  // snprintf(text, sizeof(text), "Count: %d", personCount); // For Scrolling
  snprintf(text, sizeof(text), "%d", personCount);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(NO_SCROLL); //SCROLL_LEFT else NO_SCROLL can be used for not scrolling

  matrix.endDraw();
}



void onCrowdCountChange()  {
  // Add your code here to act upon CrowdCount change
}
