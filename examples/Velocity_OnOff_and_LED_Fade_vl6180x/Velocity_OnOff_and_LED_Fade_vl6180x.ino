#include <Wire.h>
#include "Adafruit_VL6180X.h"

/*
 * This code doesn't work too well yet because the velocity code isnt very good
 */

Adafruit_VL6180X vl = Adafruit_VL6180X();
/*
boolean ledState = true;
boolean isNear = false;
int detectionRange = 50;
*/
boolean ledState = true;
boolean isMoving = false;
const int dataSize = 10;
float rangeData[dataSize];
float velocityData[dataSize];
int delayTime = 50;
const float magnitude = 100; //1000 is mm/s. 100 is cm/s. 1 is m/s
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long timeDiff = 0;
int dutyCycle = 0;

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  /*
  while (!Serial) {
    delay(1);
  }
  */
  
  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");

  //my part of the code to setup the LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //gain 5 is standard, for brighter areas, use lower gain
  float lux = vl.readLux(VL6180X_ALS_GAIN_1_25);

  Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }


  //collect data on timing
  previousTime = currentTime;
  currentTime = millis();
  timeDiff = currentTime - previousTime;
  
  //moves all the previous range data back by 1, and updates the latest range
  for(int i = 0; i < dataSize - 1; i++)
  {
    rangeData[i + 1] = rangeData[i];
  }
  rangeData[0] = range;

  for(int j = 0; j < dataSize - 2; j++)
  {
    velocityData[j + 1] = (rangeData[j + 1] - rangeData[j + 2])/(timeDiff/magnitude);
  }
  velocityData[0] = (rangeData[0] - rangeData[1])/((timeDiff/magnitude)); //delayTime is in milliseconds, range is in millimeters

  Serial.print("Velocity: "); Serial.print(velocityData[0]); Serial.println("cm/s");

  //This is the fade function
  if(ledState == true)
  {
  //map the set of proximity to the duty cycle for analogWrite
  dutyCycle = map(range, 12, 100, 0, 255);
  
  //enable the LED fade function based on proximity
  analogWrite(LED_BUILTIN, dutyCycle);
  }

  //This is the on-off function
  if((velocityData[0] > 10 || velocityData[0] < -10) && isMoving == false)
  {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    isMoving = true;
  }

  if(velocityData[0] < 10 || velocityData[0] > -10)
  {
    isMoving = false;
  }
  
  delay(delayTime);
  
}
