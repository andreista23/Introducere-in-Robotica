#include <Arduino.h>
#define LED1 10
#define LED2 9
#define LED3 8
#define LED4 7
#define RED_LED 6
#define GREEN_LED 5
#define BLUE_LED 4
#define INTERVAL 3000
#define BTNSTART 3
#define BTNSTOP 2
#define BTNTIME 1500

int previousMillis = 0; //timpul l-a care s-a inceput intervalul de 3 secunde de incarcare
int battery=0; //nivelul de incarcare al bateriei
int charging; //daca bateria se incarca sau nu
int buttonPressStartTime = 0; //timpul la care a fost apasat butonul de stop
void setup() {
  //pinii corespunzatori LED-urilor sunt setati ca OUTPUT, iar pinii corespunzatori butoanelor sunt setati ca INPUT_PULLUP
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
  
  int currentMillis = millis();
  int buttonStart = digitalRead(BTNSTART);
  //daca butonul de start este apasat, se incepe incarcarea bateriei
  if(buttonStart == LOW){
    delay(20);
    if(charging==0){
      previousMillis = currentMillis;
      charging=1;
      }
  }
  int buttonStop = digitalRead(BTNSTOP);
  if (buttonStop == LOW) {
    if (buttonPressStartTime == 0) {
      //Se inregistreaza momentul cand butonul a fost apasat
      buttonPressStartTime = currentMillis;
    }
    else
      if (currentMillis - buttonPressStartTime >= BTNTIME) {
          //delay pentru debouncing
          delay(20);
          charging = 0;  //se opreste incarcarea
          buttonPressStartTime = 0; //se reseteaza timpul de apasare
        }
  } 
  else if(buttonPressStartTime > 0){
    //daca butonul a fost apasat pentru mai putin 1.5s, se reseteaza timpul de apasare
    buttonPressStartTime = 0;
  }
  
  if(charging){
    //LED-ul RGB este aprins, avand culoarea rosie, pentru a arata ca bateria se incarca
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    
    //battery = 0 -> bateria este < 25%,  battery = 1 -> bateria este < 50%,  battery = 2 -> bateria este < 75%
    // battery = 3 -> bateria este < 100%, battery = 4 -> bateria este 100%
    switch (battery){
      case 0:
        //LED-ul 1 clipeste, celelalte sunt stinse
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
        //LED-ul 1 este aprins, LED-ul 2 clipeste, celelalte sunt stinse
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2, LOW);
        delay(200);
        digitalWrite(LED2, HIGH);
        delay(200);
        break;
      case 2:
        //LED-ul 1 si 2 sunt aprinse, LED-ul 3 clipeste, celelalte sunt stinse
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3, LOW);
        delay(200);
        digitalWrite(LED3, HIGH);
        delay(200);
        break;
      case 3:
        //LED-ul 1, 2 si 3 sunt aprinse, LED-ul 4 clipeste, celelalte sunt stinse
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4, LOW);
        delay(200);
        digitalWrite(LED4, HIGH);
        delay(200);
        break;
      case 4:
        //toate LED-urile sunt aprinse
        digitalWrite(LED4,HIGH);
        break;
      default:
        break;
    }
    //daca au trecut 3 secunde de la inceputul intervalului de incarcare, nivelul bateriei creste cu 1
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




