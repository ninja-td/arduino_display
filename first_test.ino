#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


#define BAR_HEIGHT 5
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define BAR_COLOR ST7735_BLACK
#define MAX_READ_BUF_SIZE 40
#define BACK_COLOR ST7735_WHITE

float p = 3.1415926;


char read_buffer[MAX_READ_BUF_SIZE];

void setup(void) {
  Serial.begin(9600);
  //Serial.print("Hello! ST7735 TFT Test");

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(1);
  // Use this initializer (uncomment) if you're using a 1.44" TFT
  //tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

  //Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  //Serial.println(time, DEC);
  delay(100);

  // large block of text
  tft.fillScreen(ST7735_BLACK);
  drawText("tesing 123", ST7735_WHITE);
  //drawText("tesing 456", ST7735_WHITE, 100,100);
  delay(100);

  drawHorizontalBar(5, 20, 40, 30, ST7735_GREEN);

  // a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, ST7735_GREEN);
  delay(100);

  //testdrawrects(ST7735_GREEN);
  //delay(500);


  //Serial.println("done");
  delay(100);
}

void loop() {
  if (ReadData()) {
    ProcessCommand();
  }
}

void ProcessCommand() {
  if (read_buffer[0] == 'b') {
   if(ReadData()) {
      uint16_t x = atoi(read_buffer);
      //Serial.println("read x");
      if (ReadData()) {
        uint16_t y = atoi(read_buffer);
        //Serial.println("read y");
        if (ReadData()) {
          uint16_t width = atoi(read_buffer);
          //Serial.println("read width");
          if (ReadData()) {
            uint16_t fwidth = atoi(read_buffer);
            //Serial.println("read fwidth");
            if (ReadData()) {
              uint16_t color = atoi(read_buffer);
              //Serial.println("read color");
              drawHorizontalBar(x, y, width, fwidth, color);
            }
          }
        }
      }
   }
  } else if (read_buffer[0] == 's') {
    if(ReadData()) {
      uint16_t x = atoi(read_buffer);
      //Serial.println("read x");
      if (ReadData()) {
        uint16_t y = atoi(read_buffer);
        if (ReadData()) {
          uint16_t width = atoi(read_buffer);
          //Serial.println("read y");
          if (ReadData()) {
            uint16_t color = atoi(read_buffer);
            //Serial.println("read color");
            if (ReadData()) {
              tft.fillRect(x, y, width*CHAR_WIDTH, CHAR_HEIGHT, BACK_COLOR);
              drawText(read_buffer, color, x, y);
            }
          }
        }
      }
    }            
  } else if (read_buffer[0] == 'c') {
    cls();
  } else {
    Serial.println("invalid command");
  }
}
              

boolean ReadData() {
  for (byte i = 0; i < MAX_READ_BUF_SIZE; ++i) {
    byte wait = 0;
    while (Serial.available() <= 0) {
      delay(5);
      wait++;
      if (wait > 100) {
        return false;
      }
    }
    read_buffer[i] = Serial.read();
    if (read_buffer[i] == '\0' || read_buffer[i] == '\n') {
      read_buffer[i] = '\0';
      break;
    }
  }
  read_buffer[MAX_READ_BUF_SIZE-1] = '\0';
  return true;
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void drawText(char *text, uint16_t color, uint16_t x, uint16_t y) {
  tft.setCursor(x, y);
  drawText(text, color);
}

void drawText(char *text, uint16_t color) {
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void cls() {
  tft.fillScreen(BACK_COLOR);
}

void drawHorizontalBar(uint16_t x, uint16_t y, uint16_t width, uint16_t fill_width, uint16_t color) {
  tft.fillRect(x, y, width, BAR_HEIGHT, BACK_COLOR);
  tft.drawRect(x, y, width, BAR_HEIGHT, BAR_COLOR);
  tft.fillRect(x+1, y+1, fill_width-1, BAR_HEIGHT-2, color);
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void drawFillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  tft.fillRect(x, y, width, height, color);
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}
