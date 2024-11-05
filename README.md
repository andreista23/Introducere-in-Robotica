
#  Tema de casă 1 - ⁠Sistem de încărcare a unui vehicul electric


## Descrierea cerintelor

Trebuie sa simulam o statie de incarcare, folosind LED-uri pentru a arata nivelul de incarcare al bateriei, si butoane pentru a porni/opri procesul.
Procesul de incarcare porneste la apasarea scurta a butonului de start, si se opreste fortat la apasarea lunga a butonului de stop.
Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.
Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încărca prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.

## Componentele utilizate
- placa Arduino Uno R3, cu microcontroller ATmega328P
- 4x LED-uri (pentru a simula procentul de încărcare)
- 1x LED RGB (pentru starea de liber sau ocupat)
- 2x Butoane (pentru start încărcare și stop încărcare)
- 9x Rezistoare (7x 220/330ohm, 2x 1K)
- Breadboard
- Linii de legătură

## Setup fizic
<img src="https://github.com/user-attachments/assets/c252f743-cfb8-4d06-acf7-92829e6b1d15" alt="Image 1" width="400"/>
<img src="https://github.com/user-attachments/assets/f6aa9cf6-b6a6-4712-90a1-987756e526d1" alt="Image 2" width="400"/>

## Descrierea codului
  Programul are 2 ramuri principale, pentru cazul in care bateria se incarca, si pentru cazul in care bateria nu se incarca.
  
  Se trece in modul de incarcare atunci cand este apasat butonul de start (charging=1).
  ```
    if(buttonStart == LOW){
    delay(20);
    if(charging==0){
      previousMillis = millis();
      charging=1;
      }
  }
  ```
  Daca butonul de stop este apasat pentru mai mult de 1.5s, se opreste incarcarea (charging=0).
  ```
    if (buttonStop == LOW) {
    if (buttonPressStartTime == 0) {
      //Se inregistreaza momentul cand butonul a fost apasat
      buttonPressStartTime = millis();
    }
    if (millis() - buttonPressStartTime >= BTNTIME) {
        //delay pentru debouncing
        delay(20);
        charging = 0;  //se opreste incarcarea
      }
  } 
  ```
  Daca bateria se incarca(charging==1): exista 5 cazuri, tratate intr-un switch, in functie de nivelul bateriei (battery=0 -> bateria este sub 25%, battery=1 -> bateria este intre 25% si 50%, etc):
  - (battery == 0) Primul LED clipeste, restul sunt stinse
  - (battery == 1) Primul LED este aprins, al doilea clipeste, restul sunt stinse
  - (battery == 2) LED-urile 1 si 2 sunt aprinse, LED-ul 3 clipeste, celelalte sunt stinse
  - (battery == 3) LED-urile 1, 2 si 3 sunt aprinse, LED-ul 4 clipeste, celelalte sunt stinse
  - (battery == 4) toate LED-urile sunt aprinse

  ```
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
  ```
  LED-ul RGB este aprins, avand culoarea verde.
  La fiecare 3 secunde, battery++;
  ```
    if(currentMillis - previousMillis >= INTERVAL) {
      previousMillis = currentMillis;
      battery++;
      if(battery>4){
        battery=0;
        charging=0;
      }
    }
  ```

  Daca bateria nu se incarca, toate LED-urile sunt stinse, mai putin LED-ul RGB, care este aprins, avand culoarea rosie.
## Schema electrica

![Robotica tema 1](https://github.com/user-attachments/assets/567adeeb-7e75-4b09-b231-a1cbcdd29806)
![image](https://github.com/user-attachments/assets/32822976-5bc1-41e2-8c06-c66ebc56959f)
https://www.tinkercad.com/things/3c0BLhKdvTG-robotica-tema-1


## Video cu functionalitatea montajului
https://youtube.com/watch/Jnpg4jyJvUQ?feature=share

#  Tema de casă 2 - ⁠TypeRacer Game

## Descrierea cerintelor
Trebuie sa implementam un joc de tip TypeRacer, astfel: cand jocul este in repaus, Jocul este în repaus. LED-ul RGB are culoarea albă.

Se alege dificultatea jocului folosind butonul de dificultate, iar în terminal va apărea “Easy/Medium/Hard mode on!”.

Se apasă butonul de start/stop.

LED-ul clipește timp de 3 secunde, iar în terminal se va afișa numărătoarea înversă: 3, 2, 1.

LED-ul devine verde și încep să apară cuvinte de tastat.

La tastarea corectă, următorul cuvânt apare imediat. Dacă nu se tasteaza cuvântul în timpul stabilit de dificultate, va apărea un nou cuvânt.

O greșeală face LED-ul să devină roșu. Pentru a corecta cuvântul, se va folosi tasta BackSpace.

Dupa 30 de secunde, runda se termină, iar în terminal se va afișa scorul: numărul total de cuvinte scrise corect.

Jocul se poate opri oricând cu butonul de start/stop.
## Componentele utilizate
- placa Arduino Uno R3, cu microcontroller ATmega328P
- 1x LED RGB (pentru starea de liber sau ocupat)
- 2x Butoane (pentru start încărcare și stop încărcare)
- 5x Rezistoare (3x 330ohm, 2x 1K)
- Breadboard
- Linii de legătură

## Setup fizic
![20241104_230922](https://github.com/user-attachments/assets/b7cac9bb-da1a-40b9-8218-e8118e3a1510)
![20241104_230908](https://github.com/user-attachments/assets/87f657e1-059a-4c44-8837-a858c333c73b)

## Descrierea codului
In functia setup(), am setat modul pinilor (output pentru LED, input pentru butoane), am setat baud-rate-ul pentru serial, si am initializat intreruperile de tip Hardware pentru pinii 2 si 3.
```
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
  attachInterrupt
```
In functia startGame(), am implementat animatia de la inceputul jocului, am amestecat cuvintele care se vor afisa in timpul rundei si am initializat variabilele necesare pentru inceperea jocului
```
void startGame(){
  if(!gameStarted){
    gameStarted = true;
    int currentMillis = millis();
    int previousMillis = currentMillis;
    int previousMillis2 = currentMillis;
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    char nr='2';
    Serial.println("3\n");
    while(currentMillis-previousMillis < 3000){
      currentMillis = millis();
      if(currentMillis - previousMillis2 >= 1000 && nr!='0'){
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
```
In functia round(), este implementata functionalitatea jocului in sine: runda dureaza 30s, cuvintele se afiseaza la intervalul selectat, se verifica corectitudinea literelor si a cuvintelor introduse de la tastatura, este tinut scorul.
```
void round(){
  if(roundStarted){
    if(currentMillis-previousMillis < 30000){
      currentMillis = millis();
      bool skipWord = false;
       if (Serial.available() > 0) {
                char litera = Serial.read(); // Read the next character from serial
                // Compare the letter with the current word
                if (litera == 8) { // ASCII code for backspace
                    mistake = false;
                    Serial.println("Backspace pressed");
                }
                else
                  if (litera == dictionar[currentWord-1][literaCurenta] && !mistake) {
                      Serial.println("Correct letter: " + String(litera));
                      literaCurenta++; // Move to the next letter

                      // Check if the entire word has been guessed
                      if (literaCurenta == strlen(dictionar[currentWord-1])) {
                          Serial.println("Word complete: " + String(dictionar[currentWord-1]));
                          score++;
                          skipWord = true;
                          literaCurenta = 0; // Reset letter index
                          currentWord++; // Move to the next word
                          if (currentWord >= DICTIONARY_SIZE) {
                              currentWord = 0; // Loop back to the first word
                          }
                          // Update previous word millis for the next word
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
```
In loop(), codul se imparte pe cele 2 ramuri principale: cand jocul este pornit, respectiv oprit. Daca a fost apasat butonul de start, jocul porneste sau se opreste. Cand jocul este oprit, se seteaza dificultatea. Cand este pornit, se apeleaza functiile startGame si round.
```
void loop() {
  currentMillis = millis();
  if(gameStarted){
    //daca butonul de start este apasat cand jocul este pornit, jocul se opreste
    if(btnStart == LOW){
      Serial.println("Stop button pressed");
      btnStart = 1;
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
      
      startGame();
      delay(500);
    }
    
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
  }
}
```
## Schema electrica

## Video cu functionalitatea montajului 
https://www.youtube.com/watch/1ye6pVLRoCs
