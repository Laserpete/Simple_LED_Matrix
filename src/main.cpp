#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>

#define LED_PIN 5
#define COLOR_ORDER GRB
#define CHIPSET WS2812B

#define MATRIX_WIDTH \
  18  // Set this negative if physical led 0 is opposite to where you want
      // logical 0
#define MATRIX_HEIGHT \
  18  // Set this negative if physical led 0 is opposite to where you want
      // logical 0
#define MATRIX_TYPE \
  HORIZONTAL_ZIGZAG_MATRIX  // See top of LEDMatrix.h for matrix wiring types

#define NUM_LEDS 324

#define FRAMES_PER_SECOND 120

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
uint8_t angle = 0;
uint8_t x, y;
uint8_t gHue, gCurrentPatternNumber = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(128);
  FastLED.clear(true);
  // delay(500);
  // FastLED.showColor(CRGB::Red);
  // delay(1000);
  // FastLED.showColor(CRGB::Lime);
  // delay(1000);
  // FastLED.showColor(CRGB::Blue);
  // delay(1000);
  // FastLED.showColor(CRGB::White);
  // delay(1000);
  // FastLED.clear(true);

  // // Scottish Flag
  // leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1,
  //                          CRGB(0, 0, 255));
  // leds.DrawRectangle(0, 0, leds.Width() - 1, leds.Height() - 1,
  //                    CRGB(255, 255, 255));
  // leds.DrawLine(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255,
  // 255)); leds.DrawLine(0, 1, leds.Width() - 1, leds.Height() - 2, CRGB(255,
  // 255, 255)); leds.DrawLine(0, leds.Height() - 1, leds.Width() - 1, 0,
  // CRGB(255, 255, 255)); leds.DrawLine(0, leds.Height() - 2, leds.Width() - 1,
  // 1, CRGB(255, 255, 255)); FastLED.show(); delay(5000);

  // // Japanese Flag
  // leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1,
  //                          CRGB(255, 255, 255));
  // uint16_t r = min((leds.Width() - 1) / 2, (leds.Height() - 1) / 2) - 1;
  // leds.DrawFilledCircle((leds.Width() - 1) / 2, (leds.Height() - 1) / 2, r,
  //                       CRGB(255, 0, 0));
  // FastLED.show();
  // delay(5000);

  // // Norwegian Flag
  // int16_t x = (leds.Width() / 4);
  // int16_t y = (leds.Height() / 2) - 2;
  // leds.DrawFilledRectangle(0, 0, x, y, CRGB(255, 255, 255));
  // leds.DrawFilledRectangle(0, 0, x - 1, y - 1, CRGB(255, 0, 0));
  // leds.DrawFilledRectangle(x + 3, 0, leds.Width() - 1, y, CRGB(255, 255,
  // 255)); leds.DrawFilledRectangle(x + 4, 0, leds.Width() - 1, y - 1,
  // CRGB(255, 0, 0)); leds.DrawFilledRectangle(0, y + 3, x, leds.Height() - 1,
  // CRGB(255, 255, 255)); leds.DrawFilledRectangle(0, y + 4, x - 1,
  // leds.Height() - 1, CRGB(255, 0, 0)); leds.DrawFilledRectangle(x + 3, y + 3,
  // leds.Width() - 1, leds.Height() - 1,
  //                          CRGB(255, 255, 255));
  // leds.DrawFilledRectangle(x + 4, y + 4, leds.Width() - 1, leds.Height() - 1,
  //                          CRGB(255, 0, 0));
  // leds.DrawLine(0, y + 1, leds.Width() - 1, y + 1, CRGB(0, 0, 255));
  // leds.DrawLine(0, y + 2, leds.Width() - 1, y + 2, CRGB(0, 0, 255));
  // leds.DrawLine(x + 1, 0, x + 1, leds.Height() - 1, CRGB(0, 0, 255));
  // leds.DrawLine(x + 2, 0, x + 2, leds.Height() - 1, CRGB(0, 0, 255));
  // FastLED.show();
  // delay(5000);
  // leds.ShiftLeft();
}

void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds(random16(NUM_LEDS - 1)) = CRGB::White;
  }
}

void staticRainbowWithGlitter() {
  Serial.println("Static Rainbow With glitter");
  fill_rainbow(leds[0], NUM_LEDS, 255, 255);
  addGlitter(80);
}

void wigglyRainbow() {
  uint8_t h = sin8(angle);
  leds.ShiftLeft();
  for (int16_t y = leds.Height() - 1; y >= 0; --y) {
    leds(leds.Width() - 1, y) = CHSV(h, 255, 255);
    h += 32;
  }
  // addGlitter(80);
  angle += 1;
}

void confetti() {
  Serial.println("Confetti");
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds[0], NUM_LEDS, 10);

  x = random8(18);
  y = random8(18);
  leds(x, y) += CHSV(gHue + random8(65), 200, 255);
}

void sinelon() {
  Serial.println("Sinelon");
  fadeToBlackBy(leds[0], NUM_LEDS, 1);

  x = beatsin16(40, 0, 17);
  y = beatsin16(12, 0, 17);

  leds(x, y) += CHSV(gHue, 255, 192);

  // FastLED.delay1000 / FRAMES_PER_SECOND);
}

int fixIndex(int i) {
  uint8_t matrixWidth = NUM_LEDS / MATRIX_HEIGHT;
  if (i / matrixWidth & 0x1) {
    i = i + (18 - (2 * (i % 18)) - 1);
  } else {
    i = i;
  }

  return i;
}

void bpm() {
  uint8_t BeatsPerMinute = 45;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds(fixIndex(i)) =
        ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  fadeToBlackBy(leds[0], NUM_LEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 4; i++) {
    *(leds[beatsin16(i + 7, 0, NUM_LEDS - 1)]) |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void iterateThroughMatrix() {
  for (x = 0; x < 18; x++) {
    for (y = 0; y < 18; y++) {
      leds(x, y) = CHSV(gHue, 255, 192);
      FastLED.delay(120);
    }
  }
}
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
    juggle, bpm, sinelon, confetti, wigglyRainbow, staticRainbowWithGlitter};
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}
void loop() {
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();
  EVERY_N_MILLISECONDS(20) { gHue++; }
  EVERY_N_SECONDS(30) { nextPattern(); }

  // FastLED.delay(1000 / FRAMES_PER_SECOND);
}
