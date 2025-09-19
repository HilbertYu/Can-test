#include "LedControl.h"
#include <Adafruit_NeoPixel.h>
#include "gear_font.h"

#define PIN 6        // WS2812B 的 DIN 接 Arduino D6
#define NUMPIXELS 8  // 8 顆燈


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
String inputString = "";
bool newData = false;
bool ledState = false;
unsigned long lastToggle = 0; 

void setup_rpm()
{
  pixels.begin();
  pixels.clear();
  pixels.show();    
}

void blt(void)
{
unsigned long now = millis();
    if (now - lastToggle >= 300) {
      ledState = !ledState;
      lastToggle = now;

      if (ledState) {
        pixels.fill(pixels.Color(0, 0, 80));
      } else {
        pixels.clear();
      }
      pixels.show();
    }
}

void loop_rpm(int cur_rpm)
{
  //cur_rpm = 1250;
  //const int rpm_c[4] = {1800, 3000, 4500, 6000};
  const int rpm_c[4] = {3000, 3700, 4500, 5500};
  const int rpm_nled[3] = {3, 6, 8};

  int rpm_range = rpm_c[3] - rpm_c[0];
  double rpm_blk = (double)rpm_range/NUMPIXELS;

  int cur_rpm_idx = (cur_rpm - rpm_c[0])/rpm_blk;

  if (cur_rpm_idx <= 0)
  {
    pixels.clear();
    pixels.show();
    return;
  }

  if (cur_rpm_idx >= 7)
  {
    cur_rpm_idx = 7;
  }


  if (cur_rpm >rpm_c[3])
    blt();
  else
  {
    pixels.clear();
    for (int i = 0; i <= cur_rpm_idx; i++)
    {
      if (i <= rpm_nled[0])
        pixels.setPixelColor(i, pixels.Color(0, 40, 0)); // G
      else if (i <= rpm_nled[1])
        pixels.setPixelColor(i, pixels.Color(40, 40, 0)); //Y
      else
        pixels.setPixelColor(i, pixels.Color(40, 0, 0)); //Y  
    }
  }

  pixels.show();
}


LedControl lc = LedControl(12, 11, 10, 1);  // DIN, CLK, CS, n_dev



void showChar(int index) {
  if (index < 0 || index > 7) return;
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, font[index][row]);
  }
}



struct InputInfo
{
  int gear;
  int rpm;
};

InputInfo g_input_ctx = {0};

void recvWithEndMarker()
{
  String line = Serial.readStringUntil('\n');
  line.trim();
  Serial.println(line);

  int rpm = 0;
  int gear = -1;
  int ret = sscanf(line.c_str(), "%d,%d", &rpm, &gear);

  g_input_ctx.rpm = rpm;
  g_input_ctx.gear = gear;
}

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  showChar(0);  // 預設顯示 N

  setup_rpm();
}

void loop() 
{
  recvWithEndMarker();
    
  loop_rpm(g_input_ctx.rpm);
  showChar(g_input_ctx.gear);
  newData = false;
  inputString = ""; // 清空字串
}
