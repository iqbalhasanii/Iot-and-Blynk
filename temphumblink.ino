// Blynk Template ID, Device Name, and Auth Token for Blynk 2.0
#define BLYNK_TEMPLATE_ID "TMPL6dMA5XTGo"
#define BLYNK_TEMPLATE_NAME "ESP8266 temp"
#define BLYNK_AUTH_TOKEN "qZw8L0A6Os3X3BA0Trr4rTIkMs-mCBnH"    // Replace with your Blynk Auth Token

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define DHT sensor type and pin
#define DHTPIN D4     // DHT11 data pin connected to D4 (GPIO2)
#define DHTTYPE DHT11 // DHT sensor type
DHT dht(DHTPIN, DHTTYPE);

// Define buzzer module pin
#define BUZZER_PIN D2 // Buzzer module signal pin connected to D2 (GPIO4)

// Buzzer tone frequencies (in Hz)
#define TONE_FREQUENCY 1000 // Frequency for the warning tone

// WiFi credentials
char ssid[] = "Myduino Admin";      // Replace with your WiFi SSID
char pass[] = "InspireToInvent@2009";  // Replace with your WiFi password

void setup() {
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize the buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
}

void loop() {
  Blynk.run();

  // Read temperature and humidity from DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again)
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to Blynk
  Blynk.virtualWrite(V0, temperature); // Send temperature to virtual pin V0
  Blynk.virtualWrite(V1, humidity);    // Send humidity to virtual pin V1

  // Print temperature and humidity values to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Activate buzzer and send Blynk notification if temperature > 29°C or humidity > 70%
  if (temperature > 29 || humidity > 80) {
    digitalWrite(BUZZER_PIN, HIGH);        // Turn buzzer on
    tone(BUZZER_PIN, TONE_FREQUENCY);      // Play warning tone
    //Serial.println("Warning: Temperature or Humidity exceeded limit!");
    //Blynk.notify("Warning: Temperature or Humidity exceeded limit!"); // Send notification
  } else {  
    digitalWrite(BUZZER_PIN, LOW); // Turn buzzer off
    noTone(BUZZER_PIN);            // Ensure the tone is off
  }

  // Wait 2 seconds before taking the next reading
  delay(2000);
}
