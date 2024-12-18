#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>

boolean initWiFi(String ssid, String pkey)
{
  boolean connected = false;
  Serial.print("Disconnect");
  WiFi.disconnect();
  Serial.print("Soft AP disconnect");
  WiFi.softAPdisconnect(true);
  Serial.print("Set WLAN Mode");
  WiFi.mode(WIFI_STA);
  Serial.print("Verbindung zu ");
  Serial.print(ssid);
  Serial.println(" herstellen");

  // if we have a config we try to connect
  if (ssid != "")
  {
    WiFi.begin(ssid.c_str(), pkey.c_str());
    uint8_t cnt = 0;
    while ((WiFi.status() != WL_CONNECTED) && (cnt < 20))
    {
      delay(500);
      Serial.print(".");
      cnt++;
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("IP-Adresse = ");
      Serial.println(WiFi.localIP());
      connected = true;
    }
  }
  // if we have no connection /no config or wrong config)
  // we start an access point to allow configuration
  if (!connected)
  {
    Serial.println("Keine Verbindung! \nStarte Access-Point.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("webradioconf", "", 1);
  }
  return connected;
}
