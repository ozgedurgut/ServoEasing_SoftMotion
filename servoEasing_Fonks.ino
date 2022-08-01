#include <Arduino.h>
#include "ServoEasing.hpp"
#include "pinDef.h"

#define INFO // to see serial output of loop

ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;

int whichServo;
int speedValue = 20;

int step1, step2, step3, step4, step5, step6, step7;
String s_step1, s_step2, s_step3, s_step4, s_step5, s_step6;
float st1, st2, st3, st4, st5, st6;

int aci1, aci2, aci3, aci4, aci5, aci6, aci7;
String s_aci1, s_aci2, s_aci3, s_aci4, s_aci5, s_aci6;
float angle1, angle2, angle3, angle4, angle5, angle6;
String String_read;

#define START_DEGREE_VALUE  45 // The degree value written to the servo at time of attach.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
  delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_SERVO_EASING));

  Servo1.setEasingType(EASE_CUBIC_IN_OUT);
  Servo2.setEasingType(EASE_CUBIC_IN_OUT);
  Servo3.setEasingType(EASE_CUBIC_IN_OUT);

  Servo1.attach(SERVO1_PIN, START_DEGREE_VALUE);
  Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  Servo3.attach(SERVO3_PIN, START_DEGREE_VALUE);
  delay(500);

#ifdef INFO
  Serial.println(F("Ozge Hulya Durgut Project"));
#endif

}

void loop() {

  while (Serial.available()) {
    String_read = Serial.readString();
    Serial.println();
    Serial.print("Taken String : " + String_read);
  }

  if (!String_read.equals("")) {
    if (String_read.charAt(0) == '*')                              // AÇI İLE KONTROL
    {
      aci1 = String_read.indexOf("*");
      aci2 = String_read.indexOf(",", aci1 + 1);
      aci3 = String_read.indexOf(",", aci2 + 1);
      aci4 = String_read.indexOf(",", aci3 + 1);
      aci5 = String_read.indexOf(",", aci4 + 1);
      aci6 = String_read.indexOf(",", aci5 + 1);
      aci7 = String_read.indexOf(",", aci6 + 1);

      s_aci1 = String_read.substring(aci1 + 1, aci2);
      s_aci2 = String_read.substring(aci2 + 1, aci3);
      s_aci3 = String_read.substring(aci3 + 1, aci4);
      s_aci4 = String_read.substring(aci4 + 1, aci5);
      s_aci5 = String_read.substring(aci5 + 1, aci6);
      s_aci6 = String_read.substring(aci6 + 1, aci7);

      angle1 = s_aci1.toFloat();
      angle2 = s_aci2.toFloat();
      angle3 = s_aci3.toFloat();
      angle4 = s_aci4.toFloat();
      angle5 = s_aci5.toFloat();
      angle6 = s_aci6.toFloat();

      Serial.print(angle1);    Serial.print("/");
      Serial.print(angle2);    Serial.print("/");
      Serial.print(angle3);    Serial.print("/");
      Serial.print(angle4);    Serial.print("/");
      Serial.print(angle5);    Serial.print("/");
      Serial.print(angle6);    Serial.println();

      int ang1 = int(angle1);
      int ang2 = int(angle2);
      int ang3 = int(angle3);
      int ang4 = int(angle4);
      int ang5 = int(angle5);
      int ang6 = int(angle6);

      setSpeedForAllServos(speedValue);
      
      Servo1.setEaseTo(ang1);
      Servo2.setEaseTo(ang2);
      Servo3.setEaseTo(ang3);
      synchronizeAllServosStartAndWaitForAllServosToStop();

      while (ServoEasing::areInterruptsActive()) {
        ;
      }
      Serial.println("Movement Completed!");
      String_read = "";
    }

    else if (String_read.charAt(0) == '#') { //  #setSpeed:10        #(setSpeed) : (speed value)
      Serial.println("CHANGE MODE ACTIVATED " );

      step1 = String_read.indexOf(":", 0);
      step2 = String_read.indexOf(",", step1 + 1);

      s_step1 = String_read.substring(0 + 1, step1);
      s_step2 = String_read.substring(step1 + 1, step2);

      speedValue = s_step2.toInt();

      if (String_read.substring(1, 9) == "setSpeed") {
        Serial.println("SPEED CHANGE MODE ACTIVATED");

        setSpeedForAllServos(speedValue);

        Serial.print("Speed: ");
        Serial.println(speedValue);
      }
      String_read = "";
    }
  }
}
