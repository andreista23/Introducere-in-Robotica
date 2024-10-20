#include <Arduino.h>
#define LED1 10
#define LED2 9
#define LED3 8
#define LED4 7
#define RED_LED 4
#define GREEN_LED 5
#define BLUE_LED 6
#define INTERVAL 2000
#define BTNSTART 3
#define BTNSTOP 2

int previousMillis = 0;
int battery=0;
int charging;
int buttonPressStartTime = 0;
void setup() {
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BTNSTART, INPUT_PULLUP);
  pinMode(BTNSTOP, INPUT_PULLUP);
}

void loop() {
  
  int buttonStart = digitalRead(BTNSTART);
  if(buttonStart == LOW){
    delay(20);
    if(charging==0){
      previousMillis = millis();
      charging=1;}
  }
  int buttonStop = digitalRead(BTNSTOP);
  if (buttonStop == LOW) {
    if (buttonPressStartTime == 0) {
      // Start the timer if button is first pressed
      buttonPressStartTime = millis();
    }
  if (millis() - buttonPressStartTime >= 1500) {
      // Debouncing delay to make sure it’s not a glitch
      delay(20);
      charging = 0;  // Reset the variable
    }
  } else {
    // Reset the timer if the button is released
    buttonPressStartTime = 0;
  }
  
  if(charging){
    
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    int currentMillis = millis();
    switch (battery){
      case 0:
        digitalWrite(LED1, LOW);
        delay(200);
        digitalWrite(LED1, HIGH);
        delay(200);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        break;
      case 1:
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2, LOW);
        delay(200);
        digitalWrite(LED2, HIGH);
        delay(200);
        break;
      case 2:
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3, LOW);
        delay(200);
        digitalWrite(LED3, HIGH);
        delay(200);
        break;
      case 3:
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4, LOW);
        delay(200);
        digitalWrite(LED4, HIGH);
        delay(200);
        break;
      case 4:
        digitalWrite(LED4,HIGH);
        break;
      default:
        break;
    }
   

    if(currentMillis - previousMillis >= INTERVAL) {
      previousMillis = currentMillis;
      battery++;
      if(battery>4){
        battery=0;
        charging=0;
      }
    }
    
  }
  else{
    //cand bateria nu se incarca, toate LED-urile sunt stinse, mai putin cel RGB, care arata ca incarcarea este disponibila
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    battery=0;
  }
  
  
}

