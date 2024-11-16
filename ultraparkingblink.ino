#define BLYNK_TEMPLATE_ID "TMPL6UrwCOWDY"
#define BLYNK_TEMPLATE_NAME "ESP8266 PARKING"
#define BLYNK_AUTH_TOKEN "94kjwQNX18tWjy4dV574oB0P09hw0AkU"

// Include required libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Ultrasonic sensor pins
#define TRIG_PIN D1 // Ultrasonic Trig pin
#define ECHO_PIN D2 // Ultrasonic Echo pin
#define BUZZER_PIN D7 // Buzzer Signal pin

// WiFi credentials
char ssid[] = "Myduino Admin";       // Replace with your WiFi SSID
char pass[] = "InspireToInvent@2009";   // Replace with your WiFi Password

BlynkTimer timer;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Set up a timer to send distance data to Blynk every second
  timer.setInterval(1000L, sendDistanceToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();

  long duration, distance;

  // Send a 10us pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the echo signal
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If distance is less than 10 cm, sound the alarm with beeping
  if (distance > 0 && distance <= 25) {
    tone(BUZZER_PIN, 1000, 200); // Beep at 1000 Hz for 200 ms
    delay(300); // Short pause between beeps
  } else {
    noTone(BUZZER_PIN); // Turn off the buzzer
  }

  delay(200); // Delay to stabilize sensor readings
}

// Function to send distance data to Blynk
void sendDistanceToBlynk() {
  long duration, distance;

  // Send a 10us pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the echo signal
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Send distance to Blynk virtual pin V0
  Blynk.virtualWrite(V0, distance);
}
