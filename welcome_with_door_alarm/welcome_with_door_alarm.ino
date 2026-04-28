#include <DFRobotDFPlayerMini.h>

// ESP32 Pins: RX=16, TX=17
// DFPlayer RX -> Resistor 1k -> ESP32 TX (17)
// DFPlayer TX -> ESP32 RX (16)

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // Serial Monitor එක සඳහා
  Serial.begin(115200);
  
  // DFPlayer සඳහා Hardware Serial 2 (Pin 16, 17)
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println(F("DFPlayer පරීක්ෂා කරමින්..."));

  if (!myDFPlayer.begin(Serial2)) {
    Serial.println(F("Error: DFPlayer එක සොයාගත නොහැක!"));
    Serial.println(F("1. Wiring පරීක්ෂා කරන්න."));
    Serial.println(F("2. SD Card එක දමා ඇත්දැයි බලන්න."));
    while(true); // මෙතනින් එහාට වැඩ කරන්නේ නැත
  }

  Serial.println(F("DFPlayer සාර්ථකව සම්බන්ධ විය."));
  
  myDFPlayer.volume(20);  // සද්දය (0 to 30)
  myDFPlayer.play(1);     // පළමු සින්දුව ප්ලේ කරයි
}

void loop() {
  static unsigned long timer = millis();

  // සෑම තත්පර 5 කට වරක් ඊළඟ සින්දුව ප්ලේ කරයි
  if (millis() - timer > 5000) {
    timer = millis();
    Serial.println(F("ඊළඟ සින්දුව ප්ලේ වේ..."));
    myDFPlayer.next(); 
  }
}