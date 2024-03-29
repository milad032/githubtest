#include <Arduino.h>
#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000Device.h"



// connection pins
// connection pins
const uint8_t PIN_RST = 3; // 3reset pin
const uint8_t PIN_IRQ = 7; // 7irq pin
const uint8_t PIN_SS = 10; 



void newRange() {

  //Serial.print("from: "); Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  //Serial.print("\t Range: "); Serial.print(DW1000Ranging.getDistantDevice()->getRange()); Serial.print(" m");
 // Serial.print("\t RX power: "); Serial.print(DW1000Ranging.getDistantDevice()->getRXPower()); Serial.println(" dBm");
}




void setup() {
  Serial.begin(115200);
  delay(1000);
  //init the configuration
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  //we start the module as an anchor
  DW1000Ranging.startAsAnchor("1A:2A:1A:1A:1A:1A:1A:1A", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
}

void loop() {
  DW1000Ranging.loop();
}
