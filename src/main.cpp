

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define PIN_BAT 35  /* read battery voltage, Disabled when using USB - ONLY INPUT PIN */

int BP = 0;
float v_bat = 0;
const int VReads = 15;    // number of voltage readings before choosing a median, keep this number odd

void setup() {
  Serial.begin(9600);       // Start serial monitor at a baud rate of 9600
  delay(500);
  pinMode(PIN_BAT, INPUT);
}

void sortData(float data[], int8_t size) {             // Sorts all data small to large
  float swapper;
  for (int8_t i = 0; i < (size - 1); i++) {
    for (int8_t o = 0; o < (size - (i + 1)); o++) {
      if (data[o] > data[o + 1]) {
        swapper = data[o];
        data[o] = data[o + 1];
        data[o + 1] = swapper;
      }
    }
  }
}

float mapBatt(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void read_bat() {         // reads and returns the battery voltage
  float voltageBuffer[VReads];
  uint32_t Read_buffer = 0;
  for (int x = 0; x < VReads; x++) {
    for (int i = 0 ; i < VReads; i++) {
      voltageBuffer[i] = (uint32_t)analogRead(PIN_BAT);
    }
    sortData(voltageBuffer, VReads);
    Read_buffer += (voltageBuffer[(VReads - 1) / 2]);
  }
  v_bat = (((float)(Read_buffer / VReads) / 4096) * 3600 * 2) / 1000;
  BP = mapBatt(v_bat,2.5,4.2,0,100);     // get battery voltage as a percentage 0-100%
  if (BP < 0) { BP = 0; }
}


void loop(){
  read_bat();
  Serial.print("Battery Voltage : "); Serial.println(v_bat);  // 2.5v - 4.2v
  Serial.print("Battery Percentage : "); Serial.print(BP);  Serial.println("%");  // 0-100%
  delay(5000);
}