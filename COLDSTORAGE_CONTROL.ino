#include <ESP32Servo.h> 
#include "DHT.h"

/* -------- SERVO -------- */
Servo leftServo;
Servo rightServo;
#define LEFT_SERVO_PIN   18
#define RIGHT_SERVO_PIN  19

/* -------- DHT11 -------- */
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* -------- ULTRASONIC -------- */
#define TRIG_PIN 26
#define ECHO_PIN 27

void setup() {
  Serial.begin(115200);
  leftServo.attach(LEFT_SERVO_PIN);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.write(0);
  rightServo.write(0);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("ESP32 Ready");
}

void loop() {
  /* -------- SERIAL COMMANDS -------- */
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equalsIgnoreCase("l")) { leftServo.write(90); }
    else if (command.equalsIgnoreCase("r")) { rightServo.write(90); }
    else if (command.equalsIgnoreCase("b")) { leftServo.write(90); rightServo.write(90); }
    else if (command.equalsIgnoreCase("re")) { leftServo.write(0); rightServo.write(0); }
  }

  /* -------- SENSOR READINGS -------- */
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  /* -------- SENDING DATA TO REACT -------- */
  if (!isnan(temperature)) {
    Serial.print("DATA:");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(distance);
  }
  delay(1000); 
}