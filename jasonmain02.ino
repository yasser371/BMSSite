#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "yasser";
const char* pass = "13601981";
const char* serverName = "https://ysm01.ir/switch_control.php";

String userid = "123456789012"; 

const int gpioPins[] = {12, 14, 26, 27}; // Define GPIO pins
String macAddress = WiFi.macAddress();  // Get ESP32 MAC Address

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);
    Serial.println("Connecting...");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Serial.println("\nConnected to WiFi");
    Serial.println(WiFi.localIP());
    Serial.println("MAC Address: " + macAddress);

    for (int pin : gpioPins) {
        pinMode(pin, OUTPUT);
    }
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
WiFiClientSecure* client = new WiFiClientSecure;
client->setInsecure();    //client->setInsecure(); // Keep this line

HTTPClient http;
http.begin(*client, "https://ysm01.ir/switch_control.php");
http.addHeader("Content-Type", "application/json");


// Read temperature
int adc = analogRead(35);
float temp = (adc * 3.3) / 40.96; // Adjust calculation as needed

// Create JSON payload
JSONVar payload;
payload["userid"] = userid;
payload["mac"] = macAddress;
payload["switch1"] = digitalRead(12);
payload["switch2"] = digitalRead(14);
payload["temp"] = temp; // Send temperature

String jsonString = JSON.stringify(payload);
Serial.println("Sending JSON: " + jsonString);

int httpResponseCode = http.POST(jsonString);
String response = http.getString();
Serial.println("Server Response: " + response);
http.end();

        
        delete client;
    } else {
        Serial.println("WiFi disconnected");
    }

    delay(5000);
}
