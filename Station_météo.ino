#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

Servo myServo;


DHT dht(DHTPIN, DHTTYPE);

// Configuration de l'écran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

/*const int soundPin = A0;
int soundLevel = 0;
const int redPin = 11, greenPin = 12, bluePin = 13;*/
int sensorValue = 0;
int ldrValue = 0;
const int flamePin = 8;
int flameDetected = 0;
const int humiditePin = A0;
float valeurHumidite = 0;
float valeurTemperature = 0;
float valeurTemp = 0;
float hi = 0;
float valeurIndicedeChaleur = 0;
const int ledPin = 3;
const int led1Pin = 9;
const int buzzerPin = 10;
const float c1= -42.379;
const float c2= 2.04901523;
const float c3= 10.14333127;
const float c4= -0.22475541;
const float c5= -0.00683783;
const float c6= -0.05481717;
const float c7= 0.00122874;
const float c8= 0.00085282;
const float c9= -0.00000199;


void setup() {
  //initialisation du servomoteur
  myServo.attach(5);
  myServo.write(90); //Positionner à 90 degrés
  
  //Initialisation du moniteur série
  Serial.begin(9600);
  
  // Initialisation de l'écran LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  //Initialisation du capteur DHT11
  dht.begin();

  pinMode(humiditePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(flamePin, INPUT);
  /*pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);*/
}

void loop() {
  sensorValue = analogRead(A1);
  int potValue = map(sensorValue, 0, 1023, 0, 180);
 
  myServo.write(potValue); //Tourner selon les données du potentiomètre
  delay(1000);
  /*soundLevel = analogRead(soundPin);
  if (soundLevel>0){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);

    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
  }
  else{
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }*/
  ldrValue = analogRead(A0);
  flameDetected = digitalRead(flamePin);
  valeurHumidite = dht.readHumidity(); //Lit la valeur de l'humidité receuillie par le capteur
  valeurTemp = dht.readTemperature(); //Lit la valeur de température receuillie par le capteur
  valeurTemperature = (valeurTemp*(9/5))+32;
  valeurIndicedeChaleur = c1+(c2*valeurTemperature)+(c3*valeurHumidite)+(c4*valeurTemperature*valeurHumidite)+(c5*(valeurTemperature*valeurTemperature))+(c6*(valeurHumidite*valeurHumidite))+(c7*(valeurTemperature*valeurTemperature)*valeurHumidite)+(c8*(valeurHumidite*valeurHumidite)*valeurTemperature)+(c9*(valeurTemperature*valeurTemperature)*(valeurHumidite*valeurHumidite));                                           
  hi = (valeurIndicedeChaleur-32)*(5/9);
  Serial.print("Humidite: ");
  Serial.print(valeurHumidite);
  Serial.print("% | ");
  Serial.print("Temperature: ");
  Serial.print(valeurTemp);
  Serial.println(" °C");
  Serial.print("Indice de chaleur: ");
  Serial.print(hi);
  Serial.print(" °C | ");
  if(flameDetected == HIGH){
    Serial.println(" Flamme détectée");
  }
  else{
    Serial.println(" Pas de flamme");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidite=");
  lcd.print(valeurHumidite);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp=");
  lcd.print(valeurTemp);
  lcd.print(" degC");
  if(valeurTemp <=25 || valeurHumidite<30 || valeurHumidite>70 || flameDetected == HIGH){
    tone(buzzerPin, 5000);
    digitalWrite(ledPin, LOW);
    digitalWrite(led1Pin, LOW);
  }
  else{
    noTone(buzzerPin);
    digitalWrite(ledPin, HIGH);
    digitalWrite(led1Pin, HIGH);
  }
  Serial.print("Luminosité = ");
  Serial.println(ldrValue);
  delay(500);
}
