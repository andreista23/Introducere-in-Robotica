
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

## Descrierea codului
  Programul are 2 ramuri principale, pentru cazul in care bateria se incarca, si pentru cazul in care bateria nu se incarca.
  
  Se trece in modul de incarcare atunci cand este apasat butonul de start (charging=1).
  
  Daca butonul de stop este apasat pentru mai mult de 1.5s, se opreste incarcarea (charging=0).
  
  Daca bateria se incarca(charging==1): exista 5 cazuri, tratate intr-un switch, in functie de nivelul bateriei (battery=0 -> bateria este sub 25%, battery=1 -> bateria este intre 25% si 50%, etc):
  - (battery == 0) Primul LED clipeste, restul sunt stinse
  - (battery == 1) Primul LED este aprins, al doilea clipeste, restul sunt stinse
  - (battery == 2) LED-urile 1 si 2 sunt aprinse, LED-ul 3 clipeste, celelalte sunt stinse
  - (battery == 3) LED-urile 1, 2 si 3 sunt aprinse, LED-ul 4 clipeste, celelalte sunt stinse
  - (battery == 4) toate LED-urile sunt aprinse
  LED-ul RGB este aprins, avand culoarea verde.
  La fiecare 3 secunde, battery++;

  Daca bateria nu se incarca, toate LED-urile sunt stinse, mai putin LED-ul RGB, care este aprins, avand culoarea rosie.
## Schema electrica

![Robotica tema 1](https://github.com/user-attachments/assets/567adeeb-7e75-4b09-b231-a1cbcdd29806)
![image](https://github.com/user-attachments/assets/32822976-5bc1-41e2-8c06-c66ebc56959f)



## Video cu functionalitatea montajului
