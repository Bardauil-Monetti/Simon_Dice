#include <Arduino.h>

int LED[4] = {10, 2, 3, 4};
int boton[4] = {5, 7, 8, 9};
volatile bool flag[4] = {0, 0, 0, 0};
int tActual, tPrevio = 0, cont;
int tActual2, tPrevio2 = 0, tDelay2;
int tActual3, tPrevio3 = 0, tDelay3;
int LEDon[10], lvl = 1; //hasta 10 rondas
bool estado = false, win = true, show = true;
void IRAM_ATTR ISR1(){ flag[0] = true; }
void IRAM_ATTR ISR2(){ flag[1] = true; }
void IRAM_ATTR ISR3(){ flag[2] = true; }
void IRAM_ATTR ISR4(){ flag[3] = true; }

void setup(){
  for(int i = 0; i < 4; i++){
    pinMode(LED[i], OUTPUT);
    pinMode(boton[i], INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(boton[0]), ISR1, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton[1]), ISR2, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton[2]), ISR3, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton[3]), ISR4, FALLING);
  //creo la secuencia
  for(int i = 0; i < 10; i++){
    LEDon[i] = random(0, 4);
  }
}
void loop(){
  //prender la secuencia de LEDs
  if(show){
    cont = lvl;
    tDelay2 = random(2000, 6001);
    tDelay3 = random(700, 1001);
    while(cont){ //lo hago (nivel alcanzado) veces
      tActual2 = millis();
      if(tActual2 - tPrevio2 >= tDelay2){ //espero entre 2 y 6 segundos para prender el LED 
        digitalWrite(LED[LEDon[cont - 1]], HIGH); //lo prendo
        tActual3 = millis();
        if(tActual3 - tPrevio3 >= tDelay3){ //espero entre 0.7 y 1 segundo
          digitalWrite(LED[LEDon[cont - 1]], LOW); // lo apago
          tPrevio3 = tActual3;
          cont--;
        }
        tPrevio2 = tActual2;
      }
    }
    show = false;
  }
  //escanear botones
  cont = lvl;
  win = true;
  while(cont && win){ // chequeo todas las etapas de la secuencia
    for(int j = 0; j < 4; j++){
      if(flag[j]){ // escaneo los botones
        if(j != LEDon[cont - 1]){
          //si se aprieta uno mal
          lvl = 1; // se reinicia el nivel
          //parapadeo todos los leds 4 veces cada 500ms
          int cont2 = 4;
          while(cont2){
            tActual = millis();
            if(tActual - tPrevio >= 500){
              if(estado){estado = false;}
              else{estado = true;}
              cont2--;
            }
            for(int k = 0; k<4; k++){
              digitalWrite(LED[k], estado);
            }
          }
          win = false;
          //cambio la secuencia
          for(int k = 0; k < 10; k++){
            LEDon[k] = random(0, 4);
          }
        }
        flag[j] = false;
        cont--;
      }
    }
  }
  //si se pasaron todas las etapas sin perder
  if(win){
    lvl++; // pasamos de nivel
    show = true; 
  }
}
