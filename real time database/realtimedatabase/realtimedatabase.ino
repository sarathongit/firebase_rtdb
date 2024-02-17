#include <ESP8266WiFi.h>
//#include <FirebaseESP8266.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Pixel";
const char* password = "87654321";

// Firebase project details
const char* firebaseHost = "smart-farm-98f69-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* firebaseAuth = "qRANFlp641ZkIut89J6rQRvUhqD9kWjy4syVO4l0";


// pin conf
int relay1 = D5;
int relay2 = D6;
int relay3 = D7;


// DHT sensor configuration
#define DHTPIN D4     
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);



FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuthData;
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(led,OUTPUT);


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
  Firebase.reconnectWiFi(true);
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

  // Log humidity
  if (!Firebase.RTDB.setFloat(&firebaseData, "/smart farm/humidity", humidity)) {
    Serial.println("Failed to write humidity to Firebase");
    Serial.println(firebaseData.errorReason());
  } else {
    Serial.println("Humidity logged to Firebase");
  }

  // Log temperature 
  if (!Firebase.RTDB.setFloat(&firebaseData, "/smart farm/temperature", temperature)) {
    Serial.println("Failed to write temperature to Firebase");
    Serial.println(firebaseData.errorReason());
  } else {
    Serial.println("Temperature logged to Firebase");
  }


  if (Firebase.getString(ledData, "/smart farm/relay1")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led, LOW);
    }
  }


   if (Firebase.getString(ledData, "/smart farm/relay2")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led, LOW);
    }
  }

   if (Firebase.getString(ledData, "/smart farm/relay3")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led, LOW);
    }
  }
  delay(100);
}
