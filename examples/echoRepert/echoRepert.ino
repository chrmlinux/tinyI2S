//================================================================================
//
// program name   : tinyI2S/examples/echoRepert.ino
// date/author    : 2021/02/07 @chromlinux03
//
// License
// MIT Copyright(c)2020 M5Stack https://github.com/m5stack/ATOM-ECHO/tree/master/examples/Repeater
// MIT Copyright(c)2022 @chrmlinux03 https://github.com/chrmlinux/tinyI2S
//
// example:
// RECORD_BUFSIZE_80K,SAMPLING_RATE_16K
//================================================================================
#include <M5Atom.h> // Atom Echo Ready !
#define BTN M5.Btn
#define LED24(rgb24)  M5.dis.drawpix(0, CRGB((0xFF0000 & rgb24)>>16, (0xFF00 & rgb24)>>8, (0xFF & rgb24)))

#include "tinyI2S.h"
static tinyI2S tI2S;

void setup() {
  M5.begin(true, false, true);
  tI2S.begin(RECORD_BUFSIZE_80K, SAMPLING_RATE_16K);
  tI2S.setMode(MODE_MIC); LED24(0x0000FF);
}

void loop() {
  M5.update(); if (BTN.isPressed()) {
    LED24(0xFF0000);
    while (1) {
      tI2S.record();
      M5.update(); if (BTN.isReleased() || tI2S.isBufFull()) break;
    }
    tI2S.setMode(MODE_SPK); LED24(0x00FF00);
    tI2S.play();
    tI2S.setMode(MODE_MIC); LED24(0x0000FF);
  }
  tI2S.update();
}
