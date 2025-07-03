/*
* Megademo: A collection of classic demo effects (starfield, vectorballs, scroller, image rotation) for a LovyanGFX-compatible display on Arduino-based hardware.
* The algorithms for these effects are sourced from https://github.com/flightcrank/demo-effects.
*/

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

#include <Arduino.h>
#include <string.h>
#include "vectorballs.h"
#include "scroller.h"
#include "starfield.h"
#include "image_rotate.h"
#include "image.h"

#define LED_BLUE 16
#define LED_RED 4
#define LED_GREEN 17
#define LED_BACKLIGHT 21

#define FPS 50

static LGFX tft;
static LGFX_Sprite sprite(&tft);

int16_t width = 320;
int16_t height = 240;
unsigned long last_fps_update = 0;
int fps = 0, last_fps_frame = 0;
unsigned int frame = 1;

Starfield starfield;
Vectorballs vballs;
Scroller scroller1("!!!megademo!!!");
Scroller scroller2("yo to all cool coders,graphix wizards and chiptune masters out there!");
ImageRotate image_rotate(img_face, 100, 100, 0, 0.5);
unsigned long timer[6];


void setup() {
    //Serial.begin(115200);
    //Serial.println("Starting...");

    tft.init();
    tft.setRotation(1);
    tft.setColorDepth(8);
    tft.fillScreen(TFT_BLACK);
    tft.initDMA();
    sprite.setColorDepth(8);
    // Serial.printf("width: %d, height: %d\n", tft.width(), tft.height());
    sprite.createSprite(tft.width(), tft.height());

    starfield.init(tft.width(), tft.height());
    vballs.init(tft.width(), tft.height());
    scroller1.init(tft.width(), tft.height());
    scroller2.init(tft.width(), tft.height());
    image_rotate.init(tft.width(), tft.height());

    tft.fillScreen(TFT_BLACK);

    // switch off LED
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);

    // switch on LED
    pinMode(LED_BACKLIGHT, OUTPUT);
    digitalWrite(LED_BACKLIGHT, HIGH);

    unsigned long now = millis();
    timer[0] = now + 3000;        // start time for scroller1
    timer[1] = timer[0] + 6800;   // end time for scroller1
    timer[2] = timer[1] + 2000;   // start time for image_rotate
    timer[3] = timer[2] + 12000;  // end time for image_rotate
    timer[4] = timer[3] + 2000;   // start time for vballs
    timer[5] = timer[4] + 10000;  // start time for scroller2
}

void loop() {
	unsigned long now = millis();

    sprite.fillSprite(TFT_BLACK);

    starfield.draw(&sprite);
    if (now > timer[0] && now < timer[1]) {
        scroller1.draw(&sprite);
    }
    if (now > timer[2] && now < timer[3]) {
        image_rotate.draw(&sprite);
    }
    if (now > timer[4]) {
        vballs.draw(&sprite);
    }
    if (now > timer[5]) {
        scroller2.draw(&sprite);
    }

    //sprite.setCursor(0, 0);
    //sprite.printf("FPS: %d\n", fps);

    sprite.pushSprite(0, 0);

    //time it takes to render 1 frame in milliseconds
    now += 1000 / FPS;

    if(now > millis()) {
        unsigned long sleep = now - millis();
        // Serial.printf("sleep: %d\n", sleep);
        delay(sleep);
    }

    // Calculate FPS every second
    if(millis() - last_fps_update >= 1000) {
        float time_diff = (millis() - last_fps_update) / 1000.;
        int frame_diff = frame - last_fps_frame;
        fps = (int)(frame_diff / time_diff);
        last_fps_update = millis();
        last_fps_frame = frame;
    }

    frame++;
}
