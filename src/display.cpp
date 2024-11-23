/****************************************************************************************
 * 1,8" TFT LCD SPI Display 128x160 Px with ST7735 on ESP32 using the Arduino_GFX_Library
 * https://www.az-delivery.de/collections/displays/products/1-8-zoll-spi-tft-display
 *
 *  TFT <-> ESP32 wiring:
 *  1  VCC    V5       red       5V powering >>> works also on 3V3 <<<
 *  2  GND    GND      darkblue  Ground
 *  3  CS     GPIO05   orange    SPI: CS Chip Select (Slave Select)
 *  4  RES    EN       black     Reset
 *  5  A0     GPIO17   yellow    Data Select
 *  6  SDA    GPIO23   blue      SPI: MOSI (Master Out Slave In)
 *  7  SCK    GPIO18   green     SPI: SCK (Serial Clock)
 *  8  LED    3.3V     red       3,3V Background Ilumination (or 3V3)
 *(           GPIO19   -         SPI: MISO Master In Slave Out - not used for TFT )
 *
 ****************************************************************************************/
#include <Arduino.h>
#include <U8g2lib.h>             // -> U8g2 font list: https://github.com/olikraus/u8g2/wiki/fntlistall
                                 //    U8g2 Unifont list: https://github.com/olikraus/u8g2/wiki/fntgrpunifont
#include <Arduino_GFX_Library.h> // -> https://github.com/moononournation/Arduino_GFX

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "global.h"
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
// Arduino_DataBus *bus = new Arduino_SWSPI(17 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 *gfx = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
// // Arduino_GFX *gfx = new Arduino_ST7735(
// //     bus, -1 /* RST */, 3 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */,
// //     0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */,
// //     false /* BGR */); // for 1.8" BLACKTAB 128x160; Soft-SPI. Rotation: 3 -> pins left / 1 -> pins right

/* Assign human-readable names to some common 16-bit color values:
 * Format: 5-6-5 bits für R-G-B  -> 1 1 1 1.1 1 1 1.1 1 1 1.1 1 1 1
 *                                  R R R R R|G G G G G G|B B B B B
 * #define BLACK       0x0000  ///<   0,   0,   0
 * #define NAVY        0x000F  ///<   0,   0, 123
 * #define DARKGREEN   0x03E0  ///<   0, 125,   0
 * #define DARKCYAN    0x03EF  ///<   0, 125, 123
 * #define MAROON      0x7800  ///< 123,   0,   0
 * #define PURPLE      0x780F  ///< 123,   0, 123
 * #define OLIVE       0x7BE0  ///< 123, 125,   0
 * #define LIGHTGREY   0xC618  ///< 198, 195, 198
 * #define DARKGREY    0x2082  ///<  32,  32,  32
 * #define BLUE        0x001F  ///<   0,   0, 255
 * #define GREEN       0x07E0  ///<   0, 255,   0
 * #define CYAN        0x07FF  ///<   0, 255, 255
 * #define RED         0xF800  ///< 255,   0,   0
 * #define MAGENTA     0xF81F  ///< 255,   0, 255
 * #define YELLOW      0xFFE0  ///< 255, 255,   0
 * #define WHITE       0xFFFF  ///< 255, 255, 255
 * #define ORANGE      0xFD20  ///< 255, 165,   0
 * #define GREENYELLOW 0xAFE5  ///< 173, 255,  41
 * #define PINK        0xFC18  ///< 255, 130, 198
 * #define GREY        0xCF39  ///< 0xD75A  //0xDF7B
 */

void setup_display(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!gfx->begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  gfx->fillScreen(BLACK);
  // gfx->setUTF8Print(true); // enable UTF8 support
  gfx->setTextColor(WHITE);
  gfx->display();
}

// clear one line
void clearLine(uint8_t line)
{
  uint8_t L = (line * LS) + 8;
  gfx->fillRect(0, L, SCREEN_WIDTH, LS - 1, BLACK); // coordinates upper left corner x, y; Breite; Höhe
  gfx->display();
}

// clear the whole display
void displayClear()
{
  gfx->fillScreen(BLACK);
  gfx->display();
}

// show a one line message
void displayMessage(uint8_t line, String msg)
{
  clearLine(line);
  // gfx->setFont(u8g2_font_DigitalDisco_tf);
  // gfx->setTextColor(WHITE);
  gfx->setTextWrap(false);
  gfx->setCursor(2, ((line + 1) * LS));
  // massage is limited to 20 chars
  gfx->print(msg.substring(0, 20));
  gfx->display();
}

// show a two OR MORE line message with line wrap
void displayMessage2(uint8_t line, String msg)
{
  // delete 3 compressed lines (real line high 15)
  uint8_t L = (line * LS) + 9;
  // gfx->fillRect(0, L, 160, (LS + 15 + 15 -3), BLACK);       // coordinates upper left corner x, y; Breite; Höhe
  gfx->fillRect(0, L, SCREEN_WIDTH, SCREEN_HEIGHT - L, BLACK);
  // gfx->setFont(u8g2_font_DigitalDiscoThin_tf);
  //  gfx->setTextColor(CYAN);
  gfx->setCursor(2, ((line + 1) * LS));
  gfx->setTextWrap(true);
  gfx->print(msg);
  gfx->display();
}

void displayGain()
{
  uint8_t g = 5 * log(curGain / 2);

  const uint8_t VOLUME_BLOCK = 100;
  auto v = map(g, 0, 19, 0, VOLUME_BLOCK);
  gfx->fillRect(0, 0, SCREEN_WIDTH, LS - 1, SSD1306_BLACK);
  gfx->drawRect(17, 2, VOLUME_BLOCK, 5, SSD1306_WHITE);
  gfx->fillRect(17, 3, v, 4, SSD1306_WHITE);
  gfx->setCursor(0, 0);
  gfx->printf("%02d", g);

  gfx->display();
}

// show name of current station on TFT display
void showStation()
{
  clearLine(1);
  gfx->setTextWrap(false);
  gfx->setCursor(2, (LS * 2)); // 2nd line!
  String name = String(stationlist[curStation].name);
  if (curStation == actStation)
  {
    gfx->print(name); // limit length to 19 chars
  }
  else
  {
    gfx->print("? ");
    gfx->print(name); // limit length to 17 chars + "?"
  }
  gfx->drawFastHLine(0, LS + 8, SCREEN_WIDTH, WHITE); // Coordinate Beginn x, y; Länge
  gfx->drawFastHLine(0, (2 * LS) + 8, SCREEN_WIDTH, WHITE);
  gfx->display();
}

// show date, time and loudness in the first line
struct tm displayDateTime()
{

  Serial.println("Show Time");
  char sttime[21];
  gfx->fillRect(0, 8, SCREEN_WIDTH, 16, BLACK);
  //  gfx->setFont(u8g2_font_DigitalDiscoThin_tf);
  // gfx->setTextColor(GREEN);
  gfx->setCursor(2, LS); // 1st line
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    // get date and time as a string
    strftime(sttime, sizeof(sttime), "%d. %b %Y", &timeinfo);
    // MyLcdPrint(String(sttime));
    gfx->print(String(sttime));
    strftime(sttime, sizeof(sttime), " %H:%M", &timeinfo);
    // MyLcdPrint(String(sttime));
    gfx->setTextColor(WHITE);
    gfx->print(String(sttime));
  }
  else
  {
    gfx->print("??. ??? ???? ??:??");
  } // if no result from RTC -> "??.???" is displayed
  // display loudness
  displayGain();
  gfx->display();
  return timeinfo;
}
