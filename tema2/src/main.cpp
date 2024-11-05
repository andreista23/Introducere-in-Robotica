#include <Arduino.h>
#include <string.h>

#define RED_LED 6
#define GREEN_LED 5
#define BLUE_LED 4
#define START_BUTTON 3
#define DIFF_BUTTON 2
#define DICTIONARY_SIZE 15

//variabile globale
bool gameStarted = false;
bool btnStart = 1;
bool btnStop = 1;
int wordInterval = 10000; //intervalul de timp in care trebuie ghicit cuvantul
bool roundStarted = false;
int currentMillis = millis();
int currentWord = 0; //indexul cuvantului curent
int previousMillis = currentMillis; 
int previousWordMillis = currentMillis; //timpul la care a fost afisat ultimul cuvant
bool mistake; 
int difficulty = 1; //1-easy, 2-medium, 3-hard
bool diffChanged = false; //daca s-a schimbat dificultatea
unsigned int literaCurenta = 0;
int score = 0;
bool selectDifficulty = false; //daca s-a selectat dificultatea
const char* dictionar[DICTIONARY_SIZE] = {
        "acordat", "balansat", "claritate", "dorinta", "eforturi",
        "fericire", "ganditor", "hotarat", "imagine", "judecata",
        "legatura", "motivare", "nevoie", "oferire", "placere"
    };
// functia ChangeStartButton este apelata din intrerupere atunci cand butonul de start este apasat; schimba valoarea variabilei butonului de start
void changeStartButton();
// functia changeDifficulty este apelata din intrerupere atunci cand butonul de schimbare a dificultatii este apasat; creste dificultatea
void changeDifficulty();
// functia startGame este apelata atunci cand jocul incepe; amesteca cuvintele si seteaza variabilele pentru inceperea jocului
void startGame();
// functia round este apelata in loop atunci cand jocul este in desfasurare; afiseaza cuvintele si verifica daca literele introduse sunt corecte
void round();


void setup() {
  // seteaza pinii
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(DIFF_BUTTON, INPUT_PULLUP);
  // seteaza comunicatia seriala
  Serial.begin(28800);
  // seteaza intreruperile
  attachInterrupt(digitalPinToInterrupt(START_BUTTON),changeStartButton,FALLING);
  attachInterrupt(digitalPinToInterrupt(DIFF_BUTTON),changeDifficulty,FALLING);
}

void loop() {
  currentMillis = millis();
  if(gameStarted){
    //daca butonul de start este apasat cand jocul este pornit, jocul se opreste
    if(btnStart == LOW){
      Serial.println("Stop button pressed");
      btnStart = 1;
      delay(50);
      gameStarted = false;
      roundStarted = false;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      delay(500);
     
    }
   
    if(roundStarted)
      round();
    if(mistake) { 
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
      }
      else {
        mistake = false;
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
      }
  }
  else {
    if(!selectDifficulty) {
     Serial.println("Select difficulty: ");
      selectDifficulty = true;
    }
    
    
    if(diffChanged) {
      delay(200);
      diffChanged = false;
      switch (difficulty) {
        case 1:
          wordInterval = 10000;
          Serial.println("Easy");
          break;
        case 2:
          wordInterval = 5000;
          Serial.println("Medium");
          break;
        case 3:
          wordInterval = 3000;
          Serial.println("Hard");
          break;
      }
    }
    //daca butonul de start este apasat cand jocul nu este pornit, jocul incepe
    if(btnStart == LOW && !gameStarted) {
      Serial.println("Start button pressed");
      btnStart = 1;
      delay(50);
      startGame();
      delay(500);
    }
    
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
  }
}

void changeStartButton() {
  btnStart = !btnStart;
}
void changeDifficulty(){
  if(!diffChanged && !gameStarted){
  diffChanged = true;
  difficulty++;
  if(difficulty>3)
    difficulty = 1;
    }
}

void startGame(){
  if(!gameStarted){
    gameStarted = true;
    int currentMillis = millis();
    int previousMillis = currentMillis;
    int previousMillis2 = currentMillis;
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    char nr = '2';
    Serial.println("3\n");
    //animatia de la inceput
    while(currentMillis-previousMillis < 3000){
      currentMillis = millis();
      if(currentMillis - previousMillis2 >= 1000 && nr != '0'){
        Serial.print(nr);
        Serial.println("\n");
        nr--;
        previousMillis2 = currentMillis; 
        
      }
     
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
        digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
        delay(500);
    
  }

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  randomSeed(millis());
  // Amestecarea array-ului folosind o metodă simplă
  for (int i = DICTIONARY_SIZE - 1; i > 0; i--) {
        // Generăm un index aleator
        int j = random(i + 1); // Folosim random() pentru a obține un număr aleator
        // Schimbăm elementele
        const char* temp = dictionar[i];
        dictionar[i] = dictionar[j];
        dictionar[j] = temp;
    }
    // Setăm variabilele pentru începerea jocului
    currentWord = 0;
    score=0;
    mistake = false;
    previousMillis = currentMillis;
    previousWordMillis = currentMillis;
    roundStarted = true;
  }
}
void round(){
  if(roundStarted){
    if(currentMillis-previousMillis < 30000){
      currentMillis = millis();
      bool skipWord = false;
       if (Serial.available() > 0) {
                char litera = Serial.read(); // Citim caracterul din serial
                // Verificăm dacă caracterul este o literă sau backspace
                if (litera == 8) { // ASCII code for backspace
                    mistake = false;
                    Serial.println("Backspace pressed");
                }
                else
                  // Verificăm dacă litera introdusă este corectă
                  if (litera == dictionar[currentWord-1][literaCurenta] && !mistake) {
                      Serial.println("Correct letter: " + String(litera));
                      literaCurenta++; // Move to the next letter

                      // Verificăm dacă cuvântul a fost completat
                      if (literaCurenta == strlen(dictionar[currentWord-1])) {
                          Serial.println("Word complete: " + String(dictionar[currentWord-1]));
                          score++;
                          skipWord = true;
                          literaCurenta = 0;
                          currentWord++; 
                          if (currentWord >= DICTIONARY_SIZE) {
                              currentWord = 1; // Daca am ajuns la sfarsitul dictionarului, mergem inapoi la primul cuvant
                          }
                          previousWordMillis = currentMillis; 
                      }
                  } 
                  else {
                      Serial.println("Wrong letter: " + String(litera) + " Expected: " + String(dictionar[currentWord-1][literaCurenta]));
                      mistake = true;
                    }
                  }
      if(currentWord==0) {
        Serial.println(dictionar[currentWord]);
        currentWord++;
        previousWordMillis = currentMillis;
        }
      else if(currentMillis-previousWordMillis >= wordInterval || skipWord) {
        Serial.println(dictionar[currentWord]);
        previousWordMillis = currentMillis;
        currentWord++;
        literaCurenta = 0;
        mistake = false;
    }
    }
    else {
      roundStarted = false;
      mistake = false;
      Serial.println("Round ended");
      Serial.println("Score: " + String(score));
      gameStarted = false;
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      previousWordMillis = currentMillis;
    }
  }
}
