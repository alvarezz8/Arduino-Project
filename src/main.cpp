#include <avr/io.h>
#include <util/delay.h>
#include "../hd44780/HD44780.hpp"
#include "../hd44780/HD44780.cpp"
#include "../lib/LiquidCrystal.h"
#include "../lib/LiquidCrystal.cpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int buttonPin = A0;
const int buzzerPin = 8;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned long startTime;
unsigned long elapsedTime;
unsigned long targetTime = 10000; // tiempo  en milisegundos (6 seconds)
bool gameStarted = false;

void startGame();
void endGame();

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // esprar a que se presione el botón select para iniciar
  if (digitalRead(buttonPin) == HIGH && !gameStarted) {
    lcd.clear();
    lcd.print("Press Select");
    delay(500);
    lcd.clear();
    lcd.print("Starting...");
    delay(1000);
    startGame();
  }

  if (gameStarted) {
    elapsedTime = millis() - startTime;  //tiempo transcurrido = millis() - 

    lcd.clear();
    lcd.print("Tiempo: ");
    lcd.print((targetTime - elapsedTime + 999) / 1000);  // para redondear hacia arriba
    lcd.print("s");

    // we check if the select button has been pressed to stop the stopwatch (cronometro)
    if (digitalRead(buttonPin) == LOW) {
      endGame();
    }

    // we check if time has run out
    if (elapsedTime >= targetTime) {
      endGame();
    }
  }
}

void startGame() {
  gameStarted = true;
  lcd.clear();
  lcd.print("Cuenta Atras");
  lcd.setCursor(0, 1);
  lcd.print("3 seconds");
  delay(1000);
  lcd.clear();
  lcd.print("Cuenta Atras");
  lcd.setCursor(0, 1);
  lcd.print("2 seconds");
  delay(1000);
  lcd.clear();
  lcd.print("Cuenta Atras");
  lcd.setCursor(0, 1);
  lcd.print("1 second");
  delay(1000);
  lcd.clear();
  lcd.print("¡GO!");
  startTime = millis();  // Start stopwatch after displaying GO
}

void endGame() {
  gameStarted = false;
  lcd.clear();
  lcd.print("Juego Terminado");

  // show the difference between the target time and the actual time.
  lcd.setCursor(0, 1);
  lcd.print("Diferencia: ");
  lcd.print(abs(targetTime - elapsedTime));
  lcd.print("ms");

  //activar el zumbador para indicar el final del juego
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
  
  // Esperar a que se libere el botón "Select"
  while (digitalRead(buttonPin) == HIGH) {
    delay(10);
  }

  // Esperar unos segundos antes de permitir que se inicie un nuevo juego
  delay(2000);
  lcd.clear();
  lcd.print("Presiona Select");
}

