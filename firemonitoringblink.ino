#define BLYNK_TEMPLATE_ID "TMPL6XFNL3Mhh"
#define BLYNK_TEMPLATE_NAME "Fire Monitoring System"
#define BLYNK_AUTH_TOKEN "NzUt0gYX0C9VtYu_Xgr3L1JHRbc3w64s"

#include <BlynkSimpleEsp8266.h>  // Include the Blynk library for ESP8266

// Define hardware pins
#define FLAME_SENSOR_PIN D1  // Pin connected to the flame sensor's output
#define BUZZER_PIN D2        // Pin connected to the buzzer's signal input

// WiFi credentials (replace with your network details)
char ssid[] = "Myduino Team";      // Your WiFi network name
char pass[] = "InspireToInvent@2009";  // Your WiFi password

// Timer object for periodic tasks
BlynkTimer timer;

void setup() {
  // Initialize pins
  pinMode(FLAME_SENSOR_PIN, INPUT);  // Set flame sensor pin as input
  pinMode(BUZZER_PIN, OUTPUT);       // Set buzzer pin as output
  
  // Start Serial Monitor for debugging
  Serial.begin(115200);
  Serial.println("Initializing Fire Monitoring System...");

  // Initialize Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk. System is ready.");

  // Set up a timer to check the flame sensor every second
  timer.setInterval(1000L, checkFlameSensor);
}

/**
 * Function to check the flame sensor status.
 * Sends the flame status to Blynk and triggers the buzzer if a flame is detected.
 */
void checkFlameSensor() {
  int flameState = digitalRead(FLAME_SENSOR_PIN);  // Read the flame sensor's state (HIGH or LOW)

  if (flameState == LOW) {  // Flame detected (LOW = active state for flame sensors)
    Serial.println("Warning: Flame detected! Alarm triggered.");
    
    // Send flame status to Blynk (1 = Flame detected)
    Blynk.virtualWrite(V0, 1);
    
    // Activate buzzer with a warning sound pattern
    tone(BUZZER_PIN, 1000);  // Play a 1000 Hz tone
    delay(200);              // Wait 200 ms
    tone(BUZZER_PIN, 1500);  // Change to a 1500 Hz tone
    delay(200);              // Wait 200 ms
  } else {
    // No flame detected (HIGH = inactive state for flame sensors)
    Serial.println("Status: No flame detected. System is safe.");
    
    // Send safe status to Blynk (0 = No flame detected)
    Blynk.virtualWrite(V0, 0);
    
    // Turn off the buzzer
    noTone(BUZZER_PIN);
  }
}

/**
 * Function to control the buzzer remotely via Blynk.
 * Triggered when the button widget linked to V1 is pressed.
 */
BLYNK_WRITE(V1) {
  int buzzerState = param.asInt();  // Read the button state (0 = OFF, 1 = ON)
  
  if (buzzerState == 1) {
    Serial.println("Remote command: Buzzer activated.");
    tone(BUZZER_PIN, 1000);  // Turn on buzzer with a 1000 Hz tone
  } else {
    Serial.println("Remote command: Buzzer deactivated.");
    noTone(BUZZER_PIN);      // Turn off buzzer
  }
}

void loop() {
  Blynk.run();  // Process Blynk events
  timer.run();  // Run the timer to check flame sensor
}
