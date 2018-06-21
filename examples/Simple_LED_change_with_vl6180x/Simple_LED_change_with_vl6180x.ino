#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();
boolean ledState = true;
boolean isNear = false;
int detectionRange = 50;

void setup() {
  Serial.begin(115200);

  // wait for serial port to open on native usb devices
  // this will cause the arduino to not work when serial monitor is closed
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
  float lux = vl.readLux(VL6180X_ALS_GAIN_5);

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

  //My part of the code to switch on and off the LED using hand gestures
  if(range <= detectionRange && isNear == false)
  {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    isNear = true;
  }

  if(range > detectionRange)
  {
    isNear = false;
  }
  delay(50);
}
