/*This is 7 Switches Working Code With INPUT_PULLUP When Switch Pressed TURN ON Relay*/

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// Switches & Relay Pin definition
#define S1 13
#define R1 23

#define S2 12
#define R2 22

#define S3 14
#define R3 21

#define S4 27
#define R4 19

#define S5 33
#define R5 18

#define S6 32
#define R6 5  

#define S7 15
#define R7 25

//#define S8 4
//#define R8 26



#define DEBUG_SW 0

int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;
int switch_ON_Flag5_previous_I = 0;
int switch_ON_Flag6_previous_I = 0;
int switch_ON_Flag7_previous_I = 0;



void setup() 
{
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch1_on;
extern "C" homekit_characteristic_t cha_switch2_on;
extern "C" homekit_characteristic_t cha_switch3_on;
extern "C" homekit_characteristic_t cha_switch4_on;
extern "C" homekit_characteristic_t cha_switch5_on;
extern "C" homekit_characteristic_t cha_switch6_on;
extern "C" homekit_characteristic_t cha_switch7_on;


static uint32_t next_heap_millis = 0;

//Called when the switch value is changed by iOS Home APP
void cha_switch1_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch1_on.value.bool_value = on;	//sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R1, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch2_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch2_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R2, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch3_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch3_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R3, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch4_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch4_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R4, on ? LOW : HIGH);
}

void cha_switch5_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch5_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R5, on ? LOW : HIGH);
}

void cha_switch6_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch6_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R6, on ? LOW : HIGH);
}

void cha_switch7_on_setter(const homekit_value_t value) 
{
  bool on = value.bool_value;
  cha_switch7_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R7, on ? LOW : HIGH);
}



void my_homekit_setup()
{
  pinMode(S1, INPUT_PULLUP);
  pinMode(R1, OUTPUT);

  pinMode(S2, INPUT_PULLUP);
  pinMode(R2, OUTPUT);

  pinMode(S3, INPUT_PULLUP);
  pinMode(R3, OUTPUT);

  pinMode(S4, INPUT_PULLUP);
  pinMode(R4, OUTPUT);

  pinMode(S5, INPUT_PULLUP);
  pinMode(R5, OUTPUT);

  pinMode(S6, INPUT_PULLUP);
  pinMode(R6, OUTPUT);

  pinMode(S7, INPUT_PULLUP);
  pinMode(R7, OUTPUT);

//  pinMode(S8, INPUT);
//  pinMode(R8, OUTPUT);

  //Add the .setter function to get the switch-event sent from iOS Home APP.
  //The .setter should be added before arduino_homekit_setup.
  //HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
  //Maybe this is a legacy design issue in the original esp-homekit library,
  //and I have no reason to modify this "feature".
  cha_switch1_on.setter = cha_switch1_on_setter;
  cha_switch2_on.setter = cha_switch2_on_setter;
  cha_switch3_on.setter = cha_switch3_on_setter;
  cha_switch4_on.setter = cha_switch4_on_setter;
  cha_switch5_on.setter = cha_switch5_on_setter;
  cha_switch6_on.setter = cha_switch6_on_setter;
  cha_switch7_on.setter = cha_switch7_on_setter;
  

  arduino_homekit_setup(&config);


}


void my_homekit_loop() 
{
  arduino_homekit_loop();
  switch_feedback_function();
  const uint32_t t = millis();
  if (t > next_heap_millis) 
  {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }

}

void switch_feedback_function()
{

  if (digitalRead(S1) == LOW)
  {
    if (switch_ON_Flag1_previous_I == 0 )
    {
      digitalWrite(R1, LOW);
      if (DEBUG_SW) Serial.println("Relay1- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch1_is_on = 1;
      cha_switch1_on.value.bool_value = switch1_is_on;
      homekit_characteristic_notify(&cha_switch1_on, cha_switch1_on.value);
      switch_ON_Flag1_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch1 -ON");

  }
  if (digitalRead(S1) == HIGH )
  {
    if (switch_ON_Flag1_previous_I == 1)
    {
      digitalWrite(R1, HIGH);
      if (DEBUG_SW) Serial.println("Relay1 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch1_is_on = 0;
      cha_switch1_on.value.bool_value = switch1_is_on;
      homekit_characteristic_notify(&cha_switch1_on, cha_switch1_on.value);

      switch_ON_Flag1_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch1 OFF");
  }


  if (digitalRead(S2) == LOW)
  {
    if (switch_ON_Flag2_previous_I == 0 )
    {
      digitalWrite(R2, LOW);
      if (DEBUG_SW)  Serial.println("Relay2- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch2_is_on = 1;
      cha_switch2_on.value.bool_value = switch2_is_on;
      homekit_characteristic_notify(&cha_switch2_on, cha_switch2_on.value);
      switch_ON_Flag2_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch2 -ON");

  }
  if (digitalRead(S2) == HIGH )
  {
    if (switch_ON_Flag2_previous_I == 1)
    {
      digitalWrite(R2, HIGH);
      if (DEBUG_SW) Serial.println("Relay2 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch2_is_on = 0;
      cha_switch2_on.value.bool_value = switch2_is_on;
      homekit_characteristic_notify(&cha_switch2_on, cha_switch2_on.value);
      switch_ON_Flag2_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch2 OFF");
    //delay(200);
  }

  if (digitalRead(S3) == LOW)
  {
    if (switch_ON_Flag3_previous_I == 0 )
    {
      digitalWrite(R3, LOW);
      if (DEBUG_SW) Serial.println("Relay3- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch3_is_on = 1;
      cha_switch3_on.value.bool_value = switch3_is_on;
      homekit_characteristic_notify(&cha_switch3_on, cha_switch3_on.value);
      switch_ON_Flag3_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch3 -ON");

  }
  if (digitalRead(S3) == HIGH )
  {
    if (switch_ON_Flag3_previous_I == 1)
    {
      digitalWrite(R3, HIGH);
      if (DEBUG_SW) Serial.println("Relay3 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch3_is_on = 0;
      cha_switch3_on.value.bool_value = switch3_is_on;
      homekit_characteristic_notify(&cha_switch3_on, cha_switch3_on.value);
      switch_ON_Flag3_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch3 OFF");
    //delay(200);
  }

  if (digitalRead(S4) == LOW)
  {
    if (switch_ON_Flag4_previous_I == 0 )
    {
      digitalWrite(R4, LOW);
      if (DEBUG_SW) Serial.println("Relay4- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch4_is_on = 1;
      cha_switch4_on.value.bool_value = switch4_is_on;
      homekit_characteristic_notify(&cha_switch4_on, cha_switch4_on.value);
      switch_ON_Flag4_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch4 -ON");

  }
  if (digitalRead(S4) == HIGH )
  {
    if (switch_ON_Flag4_previous_I == 1)
    {
      digitalWrite(R4, HIGH);
      if (DEBUG_SW) Serial.println("Relay4 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch4_is_on = 0;
      cha_switch4_on.value.bool_value = switch4_is_on;
      homekit_characteristic_notify(&cha_switch4_on, cha_switch4_on.value);
      switch_ON_Flag4_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch4 OFF");
    //delay(200);
  }

    if (digitalRead(S5) == LOW)
  {
    if (switch_ON_Flag5_previous_I == 0 )
    {
      digitalWrite(R5, LOW);
      if (DEBUG_SW) Serial.println("Relay5- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch5_is_on = 1;
      cha_switch5_on.value.bool_value = switch5_is_on;
      homekit_characteristic_notify(&cha_switch5_on, cha_switch5_on.value);
      switch_ON_Flag5_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch5 -ON");

  }
  if (digitalRead(S5) == HIGH )
  {
    if (switch_ON_Flag5_previous_I == 1)
    {
      digitalWrite(R5, HIGH);
      if (DEBUG_SW) Serial.println("Relay5 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch5_is_on = 0;
      cha_switch5_on.value.bool_value = switch5_is_on;
      homekit_characteristic_notify(&cha_switch5_on, cha_switch5_on.value);
      switch_ON_Flag5_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch5 OFF");
    //delay(200);
  }

    if (digitalRead(S6) == LOW)
  {
    if (switch_ON_Flag6_previous_I == 0 )
    {
      digitalWrite(R6, LOW);
      if (DEBUG_SW) Serial.println("Relay6- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch6_is_on = 1;
      cha_switch6_on.value.bool_value = switch6_is_on;
      homekit_characteristic_notify(&cha_switch6_on, cha_switch6_on.value);
      switch_ON_Flag6_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch6 -ON");

  }
  if (digitalRead(S6) == HIGH )
  {
    if (switch_ON_Flag6_previous_I == 1)
    {
      digitalWrite(R6, HIGH);
      if (DEBUG_SW) Serial.println("Relay6 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch6_is_on = 0;
      cha_switch6_on.value.bool_value = switch6_is_on;
      homekit_characteristic_notify(&cha_switch6_on, cha_switch6_on.value);
      switch_ON_Flag6_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch6 OFF");
    //delay(200);
  }

    if (digitalRead(S7) == LOW)
  {
    if (switch_ON_Flag7_previous_I == 0 )
    {
      digitalWrite(R7, LOW);
      if (DEBUG_SW) Serial.println("Relay7- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch7_is_on = 1;
      cha_switch7_on.value.bool_value = switch7_is_on;
      homekit_characteristic_notify(&cha_switch7_on, cha_switch7_on.value);
      switch_ON_Flag7_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch7 -ON");

  }
  if (digitalRead(S7) == HIGH )
  {
    if (switch_ON_Flag7_previous_I == 1)
    {
      digitalWrite(R7, HIGH);
      if (DEBUG_SW) Serial.println("Relay7 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch7_is_on = 0;
      cha_switch7_on.value.bool_value = switch7_is_on;
      homekit_characteristic_notify(&cha_switch7_on, cha_switch7_on.value);
      switch_ON_Flag7_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch7 OFF");
    //delay(200);
  }



}
