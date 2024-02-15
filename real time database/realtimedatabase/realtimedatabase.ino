#include <ESP8266WiFi.h>
//#include <FirebaseESP8266.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Pixel";
const char* password = "87654321";

// Firebase project details
const char* firebaseHost = "smart-farm-98f69-default-rtdb.asia-southeast1.firebasedatabase.app";
// For test mode, the firebaseAuth can be an empty string
const char* firebaseAuth = "qRANFlp641ZkIut89J6rQRvUhqD9kWjy4syVO4l0";

// DHT sensor configuration
#define DHTPIN D4     // GPIO pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11 type
DHT dht(DHTPIN, DHTTYPE);

// Firebase configuration
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuthData;
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Firebase configuration setup
  firebaseConfig.host = firebaseHost;
  firebaseConfig.signer.tokens.legacy_token = firebaseAuth;
  Firebase.begin(&firebaseConfig, &firebaseAuthData);
}

void loop() {
  delay(2000); // Wait between measurements

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");

  // Log humidity to Firebase
  if (!Firebase.RTDB.setFloat(&firebaseData, "/humidity", humidity)) {
    Serial.println("Failed to write humidity to Firebase");
    Serial.println(firebaseData.errorReason());
  } else {
    Serial.println("Humidity logged to Firebase");
  }

  // Log temperature to Firebase
  if (!Firebase.RTDB.setFloat(&firebaseData, "/temperature", temperature)) {
    Serial.println("Failed to write temperature to Firebase");
    Serial.println(firebaseData.errorReason());
  } else {
    Serial.println("Temperature logged to Firebase");
  }
}
