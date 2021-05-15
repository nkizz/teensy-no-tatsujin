#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=787.0000228881836,497.00001430511475
AudioMixer4              mixer1;         //xy=946.0000267028809,509.00001525878906
AudioAnalyzeFFT256       fft;       //xy=1128,476
AudioOutputI2S           i2s1;           //xy=1129,525
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(i2s2, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, fft);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;     //xy=969,701
// GUItool: end automatically generated code
#define donPin 8
#define konPin 12
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

bool hitOngoing = false;
long lastHit = 0;
void setup(){
  AudioMemory(25);
  audioShield.enable();
  audioShield.volume(0.5);
  audioShield.inputSelect(myInput);
  fft.windowFunction(AudioWindowHanning1024);
  
  Serial.begin(9600);
  Serial.println("Hello!");

  pinMode(donPin, OUTPUT);
  pinMode(konPin, OUTPUT);
  digitalWrite(donPin, HIGH);
  digitalWrite(donPin, HIGH);
}
void loop(){
  bool sound = false;
  if (fft.available()) {
    for (int i = 2; i < 8; i++){
      if (fft.read(i) >= 0.01) {sound = true;} //There is some kind of sound peak here!
    }
    if (sound) {
      if (!hitOngoing) {
        hitOngoing = true; 
        bool isDon = fft.read(0) > 0.01; //Is the hit on the center or the rim?
        digitalWrite(isDon ? donPin : konPin, LOW);
        lastHit = millis();
        Serial.println(isDon ? "don" : "kon");  
      }
    } else { //Hit has ended
      hitOngoing = false;
      digitalWrite(donPin, HIGH);
      digitalWrite(konPin, HIGH);
    }
  }
  /*float n;
  int i;

  if (fft.available()) {
    // each time new FFT data is available
    // print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (i=0; i<40; i++) {
      n = fft.read(i);
      if (n >= 0.01) {
        Serial.print(n);
        Serial.print(" ");
      } else {
        Serial.print("  -  "); // don't print "0.00"
      }
    }
    Serial.println();
  }*/
}
