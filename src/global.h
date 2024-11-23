#pragma once
#include <Arduino.h>
#include <stdint.h>
// esp32 library to save preferences in flash
#include <Preferences.h>
#include <Adafruit_SSD1306.h>

extern Preferences pref;
extern Preferences sender;

typedef struct
{
    char url[150];   // stream url
    char name[32];   // stations name
    uint8_t enabled; // flag to activate the station
} Station;

#define STATIONS 30 // number of stations in the list

#define STASSID "sdfsfsdfsdf"
#define STAPSK "dfgdfgdfg"
// gloabal variables
extern Station stationlist[STATIONS];
extern String ssid;
extern String pkey;
extern String ntp;
extern boolean connected;
extern uint8_t curStation;  // index for current selected station in stationlist
extern uint8_t curGain;     // current loudness
extern uint8_t actStation;  // index for current station in station list used for streaming
extern uint32_t lastchange; // time of last selection change
extern char title[128];     // character array to hold meta data message
extern bool newTitle;
extern uint32_t tick;   // last tick-counter value to trigger timed event
extern uint32_t discon; // tick-counter value to calculate disconnected time

//// audio.cpp
void setGain();
bool startUrl(String url);
// callback function will be called if meta data were found in input stream
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
// this function checks if decoder runs
void audio_loop();
// function to do all the required setup
void setup_audio();

///// gain
void setup_gain();
void gain_loop();

///// display
void displayGain();
void showStation();
struct tm displayDateTime();
// show a two OR MORE line message with line wrap
void displayMessage2(uint8_t line, String msg);
// show a one line message
void displayMessage(uint8_t line, String msg);
// clear one line
void clearLine(uint8_t line);
// clear the whole display
void displayClear();
void setup_display(void);
extern Adafruit_SSD1306 *gfx;
#define LS 12 // 23 // line spacing

// stations
void setup_senderList();
void reorder(uint8_t oldpos, uint8_t newpos);
void saveList();
void restore();

///// wlan.cpp
boolean initWiFi(String ssid, String pkey);

///// webserver
void setup_webserver();
void webserver_loop();

//// ota
// prepare OTA
void setup_ota();

///// rotary
void rotary_loop();
void setup_rotary();
