#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Update.h>

const char* ssid = "GITAM";
const char* password = "Gitam$$123";

// Change based on your org and device
const char* org_id = "Ahub";
const char* device_id = "1";
const char* current_version = "1.1";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

pinMode(2, OUTPUT);
digitalWrite(2, HIGH);
delay(1000);
digitalWrite(2, LOW);
delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


}

void loop(){

digitalWrite(2, HIGH);
delay(1000);
digitalWrite(2, LOW);
delay(1000);
    checkForUpdate();
}

void checkForUpdate() {
  String version_url = "https://raw.githubusercontent.com/your-org/esp-ota-repo/main/version/" + String(org_id) + "_" + String(device_id) + ".json";

  HTTPClient http;
  http.begin(version_url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String latest_version = doc["version"];
    String firmware_url = doc["url"];

    if (latest_version != current_version) {
      Serial.println("Update available: " + latest_version);
      http.end();
      performUpdate(firmware_url);
    } else {
      Serial.println("Device is up to date.");
    }
  } else {
    Serial.printf("Failed to check version. HTTP code: %d\n", httpCode);
  }
  http.end();
}

void performUpdate(String firmware_url) {
  WiFiClient client;
  t_httpUpdate_return ret = httpUpdate.update(client, firmware_url);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No updates available");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("Update OK. Device will reboot.");
      break;
  }
}
