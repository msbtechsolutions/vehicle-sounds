#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

// ================= PINS =================
const int doorPin = 5; 

// ================= TIMING & STATES =================
unsigned long lastDoorPlayTime = 0;
const int doorInterval = 5000; // තත්පර 5ක පරතරය

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  pinMode(doorPin, INPUT);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer Error!");
    while(true);
  }

  myDFPlayer.volume(28);
  Serial.println("System Ready...");

  // --- පවර් ඔන් වූ සැණින් ප්ලේ කිරීම ---
  delay(1500); // Player එක සූදානම් වීමට කාලය
  Serial.println("Power ON - Welcome");
  myDFPlayer.play(2); // 1 වෙනි ට්‍රැක් එක (Welcome)
  delay(18000); // Welcome එක ප්ලේ වී ඉවර වීමට පොඩි විවේකයක්
}

// ================= LOOP =================
void loop() {
  unsigned long now = millis();
  int doorNow = digitalRead(doorPin);

  // ================= DOOR ALARM LOGIC =================
  // දොර විවෘතව ඇත්නම් (HIGH)
  if (doorNow == LOW) {
    
    // අවසන් වරට ප්ලේ වූ අවස්ථාවේ සිට තත්පර 5ක් ගතවී ඇත්නම් නැවත ප්ලේ කරන්න
    if (now - lastDoorPlayTime >= doorInterval) {
      Serial.println("Door is OPEN - Playing Alert");
      myDFPlayer.play(1); // 5 වෙනි ට්‍රැක් එක (Door Alarm)
      lastDoorPlayTime = now; // වෙලාව අප්ඩේට් කරන්න
    }
  } 
  else {
    // දොර වැසූ සැණින් සවුන්ඩ් එක නවත්වන්න අවශ්‍ය නම්:
    // myDFPlayer.stop(); 
    
    // මීළඟ වරට දොර ඇරිය සැනින් ප්ලේ වීමට සූදානම් කිරීම
    lastDoorPlayTime = now - doorInterval; 
  }
}