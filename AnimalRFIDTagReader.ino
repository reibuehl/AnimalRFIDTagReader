#include <Wire.h>               // I2C support for the OLED display
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "fonts.h"

// Define "DEBUG" for more debug output on serial connection
#define DEBUG
#ifdef DEBUG
 #define DEBUG_PRINTLN(x)  Serial.println (x)
 #define DEBUG_PRINT(x)  Serial.print (x)
#else
 #define DEBUG_PRINTLN(x)
 #define DEBUG_PRINT(x)
#endif

// Pins for Red/Greed LED
#define GREENPIN 2
#define REDPIN 9

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48 );   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h

unsigned char message[35];
unsigned long lastSignal = 0;
bool transmission = false;
byte state = 1;
int pos;
unsigned long long testChip = 99200025140LL;

struct countrycode_t {
   int numCode;
   String alpha3Code;
} countrycodes[] = { 4, "AFG", 8, "ALB", 10, "ATA", 12, "DZA", 16, "ASM", 20, "AND", 24, "AGO", 28, "ATG", 31, "AZE", 32, "ARG", 
                     36, "AUS", 40, "AUT", 44, "BHS", 48, "BHR", 50, "BGD", 51, "ARM", 52, "BRB", 56, "BEL", 60, "BMU", 64, "BTN", 
                     68, "BOL", 70, "BIH", 72, "BWA", 74, "BVT", 76, "BRA", 84, "BLZ", 86, "IOT", 90, "SLB", 92, "VGB", 96, "BRN", 
                     100, "BGR", 104, "MMR", 108, "BDI", 112, "BLR", 116, "KHM", 120, "CMR", 124, "CAN", 132, "CPV", 136, "CYM", 
                     140, "CAF", 144, "LKA", 148, "TCD", 152, "CHL", 156, "CHN", 158, "TWN", 162, "CXR", 166, "CCK", 170, "COL", 
                     174, "COM", 175, "MYT", 178, "COG", 180, "COD", 184, "COK", 188, "CRI", 191, "HRV", 192, "CUB", 196, "CYP", 
                     203, "CZE", 204, "BEN", 208, "DNK", 212, "DMA", 214, "DOM", 218, "ECU", 222, "SLV", 226, "GNQ", 231, "ETH", 
                     232, "ERI", 233, "EST", 234, "FRO", 238, "FLK", 239, "SGS", 242, "FJI", 246, "FIN", 248, "ALA", 250, "FRA", 
                     254, "GUF", 258, "PYF", 260, "ATF", 262, "DJI", 266, "GAB", 268, "GEO", 270, "GMB", 275, "PSE", 276, "DEU", 
                     288, "GHA", 292, "GIB", 296, "KIR", 300, "GRC", 304, "GRL", 308, "GRD", 312, "GLP", 316, "GUM", 320, "GTM", 
                     324, "GIN", 328, "GUY", 332, "HTI", 334, "HMD", 336, "VAT", 340, "HND", 344, "HKG", 348, "HUN", 352, "ISL", 
                     356, "IND", 360, "IDN", 364, "IRN", 368, "IRQ", 372, "IRL", 376, "ISR", 380, "ITA", 384, "CIV", 388, "JAM", 
                     392, "JPN", 398, "KAZ", 400, "JOR", 404, "KEN", 408, "PRK", 410, "KOR", 414, "KWT", 417, "KGZ", 418, "LAO", 
                     422, "LBN", 426, "LSO", 428, "LVA", 430, "LBR", 434, "LBY", 438, "LIE", 440, "LTU", 442, "LUX", 446, "MAC", 
                     450, "MDG", 454, "MWI", 458, "MYS", 462, "MDV", 466, "MLI", 470, "MLT", 474, "MTQ", 478, "MRT", 480, "MUS", 
                     484, "MEX", 492, "MCO", 496, "MNG", 498, "MDA", 499, "MNE", 500, "MSR", 504, "MAR", 508, "MOZ", 512, "OMN", 
                     516, "NAM", 520, "NRU", 524, "NPL", 528, "NLD", 531, "CUW", 533, "ABW", 534, "SXM", 535, "BES", 540, "NCL", 
                     548, "VUT", 554, "NZL", 558, "NIC", 562, "NER", 566, "NGA", 570, "NIU", 574, "NFK", 578, "NOR", 580, "MNP", 
                     581, "UMI", 583, "FSM", 584, "MHL", 585, "PLW", 586, "PAK", 591, "PAN", 598, "PNG", 600, "PRY", 604, "PER", 
                     608, "PHL", 612, "PCN", 616, "POL", 620, "PRT", 624, "GNB", 626, "TLS", 630, "PRI", 634, "QAT", 638, "REU", 
                     642, "ROU", 643, "RUS", 646, "RWA", 652, "BLM", 654, "SHN", 659, "KNA", 660, "AIA", 662, "LCA", 663, "MAF", 
                     666, "SPM", 670, "VCT", 674, "SMR", 678, "STP", 682, "SAU", 686, "SEN", 688, "SRB", 690, "SYC", 694, "SLE", 
                     702, "SGP", 703, "SVK", 704, "VNM", 705, "SVN", 706, "SOM", 710, "ZAF", 716, "ZWE", 724, "ESP", 728, "SSD", 
                     729, "SDN", 732, "ESH", 740, "SUR", 744, "SJM", 748, "SWZ", 752, "SWE", 756, "CHE", 760, "SYR", 762, "TJK", 
                     764, "THA", 768, "TGO", 772, "TKL", 776, "TON", 780, "TTO", 784, "ARE", 788, "TUN", 792, "TUR", 795, "TKM", 
                     796, "TCA", 798, "TUV", 800, "UGA", 804, "UKR", 807, "MKD", 818, "EGY", 826, "GBR", 831, "GGY", 832, "JEY", 
                     833, "IMN", 834, "TZA", 840, "USA", 850, "VIR", 854, "BFA", 858, "URY", 860, "UZB", 862, "VEN", 876, "WLF", 
                     882, "WSM", 887, "YEM", 894, "ZMB"};

String getCountryCode(int numCode) {
  for (int i=0; i!=sizeof(countrycodes)/sizeof(countrycodes[0]); ++i) {
    countrycode_t* c  = countrycodes+i;
    if (c->numCode == numCode) {
      return c->alpha3Code;
    }
  } 
  return String(numCode);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(GREENPIN, OUTPUT);
  digitalWrite(GREENPIN, LOW);
  pinMode(REDPIN, OUTPUT);
  digitalWrite(REDPIN, LOW);
  lastSignal = millis();
  display.init();
  display.clear();
  display.flipScreenVertically();
  DEBUG_PRINTLN("Start");
}

void loop() {
  char hexvalue[2];
  switch (state) {
    // State 1: Wait for Serial input from the RFID reader module
    case 1: {
        if (Serial2.available() > 0) {
          lastSignal = millis();
          pos = 0;
          state = 2;
          DEBUG_PRINTLN("Changing from state 1 to 2");
        }
        break;
      }
    // State 2: Read Serial input from the RFID reader module until max. 35 bytes or no input for 100ms
    case 2: {
        // Reading of message
        if (Serial2.available() > 0 && pos < 35) {
          lastSignal = millis();
          message[pos] = Serial2.read();
                         sprintf(hexvalue, "%02X", message[pos]);
                         DEBUG_PRINT(hexvalue);
                         DEBUG_PRINT(",");
          pos++;
        }
        if (millis() - lastSignal > 100) {
          state = 3;
          DEBUG_PRINTLN("");
          DEBUG_PRINTLN("Changing from state 2 to 3");
        }
        if (pos >= 35) {
          delay(300);
          state = 1;
          DEBUG_PRINTLN("");
          DEBUG_PRINTLN("Changing from state 2 to 1");
        }
      }
      break;
    // Stage 3: Calculate checksum and decode tag information if checksum is OK  
    case 3: {
        // checksum
        byte check = message[1];
        for (int i = 2; i < 11; i++) {
          check = check ^ message[i];
        }
        if (check == message[11]) {
          DEBUG_PRINTLN("Check OK");

          // Decode
          unsigned long long id = 0LL;
          for (int i = 6; i < 11; i++) {
            id = (id << 8) + (unsigned long)message[i];
          }
          int countryNbr = (uint32_t) message[4] << 8;
          countryNbr |= (uint32_t) message[5]; 
          String countryCode = getCountryCode(countryNbr);
          Serial.print("Country=");
          Serial.println(countryCode);
          display.clear();
          display.setFont(Dialog_Plain_12);
          display.drawString(0, 2, countryCode + " " + countryNbr);
          // Serial.println(display.getStringWidth(countryCode + " " + countryNbr));
          char highBuf[10];
          char lowBuf[10];
          sprintf(highBuf, "%06ld", id/1000000L);
          sprintf(lowBuf, "%06ld", id%1000000L);  
          Serial.print("TagNumber=");
          Serial.print(countryNbr);
          Serial.print(highBuf);
          Serial.println(lowBuf);
          display.drawString(0, 16, highBuf);
          display.drawString(0, 30, lowBuf);
          display.display();

          if (id == testChip) {
            digitalWrite(GREENPIN, HIGH);
            digitalWrite(REDPIN, LOW);
            DEBUG_PRINTLN("It is Buddy!");
          } else {
            digitalWrite(GREENPIN, LOW);
            digitalWrite(REDPIN, HIGH);
            DEBUG_PRINTLN("It is not Buddy!");
          }
          state = 4;
          DEBUG_PRINTLN("Changing from state 3 to 4");
        } else {
          Serial.println("Read Error");
          #ifdef DEBUG
            Serial.println("Check FAILED");
            Serial.print("Calculated Checksum: ");
            Serial.println(check, HEX);
            Serial.print("Expected Checksum: ");
            Serial.println(message[11], HEX);
          #endif
          display.clear();
          display.setFont(Dialog_Plain_12);
          display.drawString(0, 2, "Read Error");
          state = 1;
          DEBUG_PRINTLN("Changing from state 3 to 1");
        }
        break;
      }
    case 4: {
        delay(2000);
        digitalWrite(GREENPIN, LOW);
        digitalWrite(REDPIN, LOW);
        state = 1;
        DEBUG_PRINTLN("Changing from state 4 to 1");
        break;
      }
  } //switch
}
