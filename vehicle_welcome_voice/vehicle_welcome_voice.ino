#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

// SD Card Pins (ඔබේ Wiring එකට අනුව)
#define SD_CS_PIN    5
#define SD_SCK_PIN   18
#define SD_MISO_PIN  19
#define SD_MOSI_PIN  23

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);
  delay(2000); // Serial එක පණගැන්වෙන තෙක් පොඩි වෙලාවක්
  
  Serial.println("\n--- ESP32 Audio & SD Card Test ---");

  // 1. SPI සහ SD Card එක පණගැන්වීම
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error: SD Card එක වැඩ නැත! වයර් පරීක්ෂා කරන්න.");
    while (1); // මෙතනින් නවත්වන්න
  }
  Serial.println("Success: SD Card එක වැඩ කරයි.");

  // 2. SD එකේ ෆයිල් එක තියෙනවද බලන්න
  // ඔබේ SD card එකේ ඇතුලේ default.mp3 කියලා ෆයිල් එකක් තිබිය යුතුයි.
  if (SD.exists("/default.mp3")) {
    Serial.println("Success: /default.mp3 ෆයිල් එක සොයාගත්තා.");
  } else {
    Serial.println("Error: /default.mp3 ෆයිල් එක SD Card එකේ නැත!");
    while (1);
  }

  // 3. Audio Output එක සකස් කිරීම (Internal DAC - GPIO 25)
  Serial.println("Audio Output එක සකස් කරයි (Using GPIO 25)...");
  out = new AudioOutputI2S(0, AudioOutputI2S::INTERNAL_DAC);
  out->SetOutputModeMono(true);
  out->SetGain(0.8); // සද්දය 80% ක්

  // 4. ප්ලේ කිරීම ආරම්භ කිරීම
  file = new AudioFileSourceSD("/default.mp3");
  mp3 = new AudioGeneratorMP3();

  if (mp3->begin(file, out)) {
    Serial.println("Success: MP3 ප්ලේ වීම ආරම්භ විය!");
  } else {
    Serial.println("Error: MP3 එක ප්ලේ කිරීමට නොහැක.");
  }
}

void loop() {
  // සද්දය ඇසෙන අතරතුර Serial Monitor එකේ Status එක පෙන්වයි
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      Serial.println("\nPlayback අවසන් විය.");
    }
  } else {
    Serial.println("Audio එක නැවතුණා හෝ දෝෂයක් ඇත.");
    delay(5000);
  }
}