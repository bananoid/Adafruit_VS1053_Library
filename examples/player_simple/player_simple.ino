/***************************************************
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI "Connections"

// These can be any pins:
#define RESET_1 8      // VS1053 reset pin (output)
#define CS_1 6        // VS1053 chip select pin (output)
#define DCS_1 7        // VS1053 Data/command select pin (output)
#define CARDCS_1 9     // Card chip select pin
#define DREQ_1 2       // VS1053 Data request, ideally an Interrupt pin // DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt

#define RESET_2 15      // VS1053 reset pin (output)
#define CS_2 17        // VS1053 chip select pin (output)
#define DCS_2 16        // VS1053 Data/command select pin (output)
#define CARDCS_2 14     // Card chip select pin
#define DREQ_2 18       // VS1053 Data request, ideally an Interrupt pin // DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt



Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(RESET_1, CS_1, DCS_1, DREQ_1, CARDCS_1);

Adafruit_VS1053_FilePlayer musicPlayer2 = Adafruit_VS1053_FilePlayer(RESET_2, CS_2, DCS_2, DREQ_2, CARDCS_2);

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  musicPlayer.begin(); // initialise the music player
  musicPlayer2.begin();
  SD.begin(CARDCS_1);    // initialise the SD card

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  musicPlayer2.setVolume(20,20);
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT,1);  // DREQ int
  musicPlayer2.useInterrupt(VS1053_FILEPLAYER_PIN_INT,2);

  // Play one file, don't return until complete
  // musicPlayer.playFullFile("track001.mp3");
  // musicPlayer2.playFullFile("track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  musicPlayer.startPlayingFile("track001.mp3");
  musicPlayer2.startPlayingFile("track002.mp3");

}

void loop() {
  // File is playing in the background
  if (musicPlayer.stopped()) {
    Serial.println("Done playing music");
    while (1);
  }
  if (Serial.available()) {
    char c = Serial.read();

    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }

    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else {
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }

  delay(100);
}
