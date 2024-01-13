#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "HUAWEIP40"
#define WIFI_PASSWORD ""
#define API_KEY "AIzaSyCn75QqUymZshV0Ewg1qRgPJBJWbU7Qfro"
#define DATABASE_URL "https://iot-plastenik-default-rtdb.europe-west1.firebasedatabase.app/"

#include "DHT.h"

#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int status = 0;
void setup() {

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(D1, OUTPUT);
  dht.begin();

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUP OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setInt(&fbdo, "Senzor/temperatura", temperature);
    Firebase.RTDB.setInt(&fbdo, "Senzor/vlaznost", humidity);
  }
  if (temperature > 25) {
    Serial.println("Trebala bi se upaliti led dioda");
    digitalWrite(D1, HIGH);
  }
  if (temperature < 25) {
    Serial.println("Ne bi se upaliti led dioda");
    digitalWrite(D1, LOW);
  }
}