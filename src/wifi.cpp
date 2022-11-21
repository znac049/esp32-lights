#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "Settings.h"
#include "lights.h"

bool setupWifi()
{
    int wifiOk;
    int wifiTries = 0;

    // Connect to Wi-Fi
    WiFi.begin(Settings::get("ssid").c_str(), Settings::get("wifiPassword").c_str());
    
    wifiOk = WiFi.status();
    while ((wifiOk != WL_CONNECTED) && (wifiTries < 10)) {
        delay(1000);
        wifiTries++;
        Serial.print("Connecting to WiFi... attempt ");
        Serial.println(wifiTries);
        wifiOk = WiFi.status();
    }

    if (wifiOk == WL_CONNECTED) {
        if(!MDNS.begin(Settings::get("deviceName").c_str())) {
            Serial.println("Error starting mDNS");
            return false;
        }

        // Print ESP Local IP Address
        Serial.println(WiFi.localIP());
    }
    else {
        // Set myself up as an access point
        Serial.println("Creating local access point...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP((Settings::get("ssid") + ".ap").c_str(), "12345678");
        Serial.println(WiFi.softAPIP());
    }

    Serial.println(WiFi.macAddress());

    return true;
}