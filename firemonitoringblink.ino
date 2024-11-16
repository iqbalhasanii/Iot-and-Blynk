#define BLYNK_TEMPLATE_ID "TMPL6hF1EGzfd"
#define BLYNK_TEMPLATE_NAME "Fire Monitoring"
#define BLYNK_AUTH_TOKEN "lYXsD41PvD1Penj5iW0w8EKTdQuO9VLf"

#include <BlynkSimpleEsp8266.h>

#define FLAME_SENSOR_PIN D1
#define BUZZER_PIN D2

char ssid[] = "Myduino Team";
char pass[] = "InspireToInvent@2009";

BlynkTimer timer;

void setup() {
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
  
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Fire Monitoring System Initialized");

  // Timer to check the flame sensor periodically
  timer.setInterval(1000L, checkFlameSensor);
}

void checkFlameSensor() {
  int flameState = digitalRead(FLAME_SENSOR_PIN);

  if (flameState == LOW) {  // Flame detected
    Serial.println("Flame detected! Alarm triggered.");
    Blynk.virtualWrite(V0, 1);  // Update flame status on V0 in Blynk
    // Warning tone on the buzzer
    tone(BUZZER_PIN, 1000);
    delay(200);
    tone(BUZZER_PIN, 1500);
    delay(200);
  } else {
    Blynk.virtualWrite(V0, 0);  // Safe status on V0 in Blynk
    noTone(BUZZER_PIN);
  }
}

BLYNK_WRITE(V1) {  // Control buzzer via V1 in Blynk app
  int buzzerState = param.asInt();
  if (buzzerState == 1) {
    tone(BUZZER_PIN, 1000);  // Turn on buzzer remotely
  } else {
    noTone(BUZZER_PIN);      // Turn off buzzer remotely
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
