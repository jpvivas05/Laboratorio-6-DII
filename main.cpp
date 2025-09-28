//******************************************/
// Universidad del Valle de Guatemala
// BE3029 - Electronica Digital 2
// Pablo Mazariegos
// 21/07/2025
// Ejemplo Hola Mundo
// MCU: ESP32 dev kit 1.0
//******************************************/
//******************************************/
// Librerias
//******************************************/
#include <Arduino.h>
#include <stdint.h>
#include "driver/ledc.h"
//******************************************/
// Definiciones
//******************************************/
#define pot1 33
#define pot2 32
#define ledA 27
#define ledR 25
#define ledV 26

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
uint8_t contadorAzul = 0; // contador de 8 bits Azul
//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
//******************************************/
// Configuracion
//******************************************/
void setup() {
  Serial.begin(115200);
  initleds();
  initPWM();
}
//******************************************/
// Loop Principal
//******************************************/
void loop() {
  Serial.print("Potenciómetro 1:");
  leerADC(pot1, canalR);
  delay(250);
  Serial.print("Potenciómetro 2:");
  leerADC(pot2, canalV);
  delay(250);
  Serial.print("Contador = ");
  leerSerial();
  Serial.println(contadorAzul);
  delay(250);
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
void leerADC(int pot, int canal){
  uint8_t val = analogRead(pot);
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