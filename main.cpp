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
#define rs 32
#define en 25
#define d4 26
#define d5 27
#define d6 14
#define d7 13

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
void leerADC();
void leerADC2();
void initleds(void);
void initPWM(void);
//******************************************/
// Variables globales
//******************************************/
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
uint8_t val = 0;
uint8_t val2 = 0;
uint8_t contadorAzul = 255; // contador de 8 bits Azul
//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
//******************************************/
// Configuracion
//******************************************/
void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Todo bien!");
  delay(500);
  lcd.clear();
  initleds();
  initPWM();
}
//******************************************/
// Loop Principal
//******************************************/
void loop() {
  lcd.setCursor(0,0);
  lcd.print("Rojo  Verde Azul");
  Serial.print("Potenciómetro Rojo:");
  leerADC();
  lcd.setCursor(0,1);
  lcd.print(val);
  delay(10);
  Serial.print("Potenciómetro Verde:");
  leerADC2();
  lcd.setCursor(6,1);
  lcd.print(val2);
  delay(10);
  Serial.print("Contador Azul:");
  leerSerial();
  lcd.setCursor(12,1);
  lcd.print(contadorAzul);
  delay(100);
  lcd.clear();
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
  Serial.println(contadorAzul);
  ledcWrite(canalA, contadorAzul);
}
void leerADC() {
  const int N = 10;
  long suma = 0;

  for (int i = 0; i < N; i++) {
    suma += analogRead(pot1);
    delay(1);
  }
  int poten = suma / N;                // promedio
  val = map(poten, 0, 4095, 0, 255); // mapea a 8 bits

  ledcWrite(canalR, val);

  Serial.println(val);
}
void leerADC2() {
  const int N2 = 10;
  long suma2 = 0;

  for (int i = 0; i < N2; i++) {
    suma2 += analogRead(pot2);
    delay(1);
  }
  int poten2 = suma2 / N2;                // promedio
  val2 = map(poten2, 0, 4095, 0, 255); // mapea a 8 bits

  ledcWrite(canalV, val2);

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
