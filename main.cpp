//******************************************/
// Universidad del Valle de Guatemala
// BE3029 - Electronica Digital 2
// Pablo Mazariegos
// 27/09/2025
// Laboratorio 6
// MCU: ESP32 dev kit 1.0
//******************************************/
//******************************************/
// Librerias
//******************************************/
#include <Arduino.h>
#include <stdint.h>
#include "driver/ledc.h"
#include <LiquidCrystal.h>
//******************************************/
// Definiciones
//******************************************/
/*#define rs 14
#define en 12
#define d4 19
#define d5 18
#define d6 5
#define d7 17*/

#define pot1 36
#define pot2 39
#define ledA 18
#define ledR 21
#define ledV 19

#define canalR 0
#define canalV 1
#define canalA 3

#define freqPWM 100

#define resPWM 8
//******************************************/
// Prototipos de funciones
//******************************************/
void leerSerial(void);
void leerADC(int pot, int canal);
void initleds(void);
void initPWM(void);
//******************************************/
// Variables globales
//******************************************/
//LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

uint8_t contadorAzul = 255; // contador de 8 bits Azul
//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
//******************************************/
// Configuracion
//******************************************/
void setup() {
  Serial.begin(115200);

  /*lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Todo bien!");*/
  initleds();
  initPWM();
}
//******************************************/
// Loop Principal
//******************************************/
void loop() {
  Serial.print("Potenciómetro 1:");
  leerADC(pot1, canalR);
  delay(500);
  Serial.print("Potenciómetro 2:");
  leerADC(pot2, canalV);
  delay(500);
  Serial.print("Contador = ");
  leerSerial();
  Serial.println(contadorAzul);
  delay(500);
}
//******************************************/
// Otras funciones
//******************************************/
void leerSerial() {
  while (Serial.available() > 0) {
    char tecla = Serial.read();
    if (tecla == '-') {
      if (contadorAzul < 255) {
        contadorAzul++;
      }
    }
    else if (tecla == '+') {
      if (contadorAzul > 0) {
        contadorAzul--;
      }
    }
  }
  ledcWrite(canalA, contadorAzul);
}
void leerADC(int pot, int canal) {
  const int N = 10;
  long suma = 0;

  for (int i = 0; i < N; i++) {
    suma += analogRead(pot);
    delay(1);
  }
  int poten = suma / N;                // promedio (0..4095)
  uint8_t val = map(poten, 0, 4095, 0, 255); // mapea a 8 bits

  ledcWrite(canal, val);

  Serial.println(val);
}

void initPWM(void){
  //    Asignar canales
  ledcSetup(canalR, freqPWM, resPWM);
  ledcSetup(canalV, freqPWM, resPWM);
  ledcSetup(canalA, freqPWM, resPWM);
  //    Asignar GPIO
  ledcAttachPin(ledR, canalR);
  ledcAttachPin(ledV, canalV);
  ledcAttachPin(ledA, canalA);
}
void initleds(void){
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(ledA, OUTPUT);

  digitalWrite(ledR, HIGH);
  digitalWrite(ledV, HIGH);
  digitalWrite(ledA, HIGH);
}
