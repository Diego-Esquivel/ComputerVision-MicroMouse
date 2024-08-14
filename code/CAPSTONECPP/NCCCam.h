// ArduCAM Mini demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Mini camera, and can run on any Arduino platform.
// This demo was made for ArduCAM_Mini_5MP_Plus.
// It needs to be used in combination with PC software.
// It can test OV5642 functions.
//

// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM_Mini_5MP_Plus
// and use Arduino IDE 1.6.8 compiler or above
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"
const float DISTANCEMAP[120] = {48.23797897,
48.66174136,
49.09252815,
49.53051953,
49.9759019,
50.42886814,
50.88961788,
51.35835785,
51.83530213,
52.32067254,
52.81469895,
53.31761966,
53.82968181,
54.35114177,
54.88226558,
55.42332943,
55.97462012,
56.53643558,
57.10908544,
57.69289158,
58.28818875,
58.89532525,
59.51466357,
60.14658114,
60.79147116,
61.44974333,
62.12182482,
62.80816116,
63.50921726,
64.22547844,
64.95745161,
65.70566647,
66.47067677,
67.25306173,
68.05342752,
68.87240883,
69.71067056,
70.56890966,
71.44785709,
72.34827988,
73.27098344,
74.21681394,
75.18666094,
76.18146022,
77.20219681,
78.24990827,
79.32568824,
80.4306903,
81.56613211,
82.7332999,
83.93355345,
85.16833133,
86.43915674,
87.74764382,
89.09550453,
90.48455619,
91.91672969,
93.39407853,
94.91878866,
96.49318941,
98.11976538,
99.80116962,
101.5402381,
103.3400059,
105.2037249,
107.1348833,
109.1372278,
111.2147879,
113.3719031,
115.6132532,
117.9438922,
120.3692869,
122.8953596,
125.5285368,
128.2758034,
131.1447655,
134.1437207,
137.2817382,
140.5687512,
144.0156618,
147.6344625,
151.4383756,
155.4420153,
159.6615757,
164.1150504,
168.8224891,
173.8063006,
179.0916096,
184.7066817,
190.6834295,
197.0580207,
203.8716114,
211.1712375,
219.010903,
227.45292,
236.5695711,
246.4451853,
257.1787581,
268.8872837,
281.7100414,
295.8141659,
311.4019784,
328.7207604,
348.0759769,
369.8494598,
394.5248622,
422.7240242,
455.2601211,
493.2173867,
538.0743216,
591.9008268,
657.686759,
739.9169035,
845.6387799,
986.5982543,
1183.937885,
1479.942791,
1973.278246,
2959.940073,
5919.907391
};
// set pin 7 as the slave select for the digital pot:
const int CS = 31;
bool is_header = false;
int mode = 0;
uint8_t start_capture = 0;
 ArduCAM myCAM( OV5642, CS );
  uint16_t VH, VL;
    int i = 0, j = 0;
    int count = 0;
    uint16_t in_arr[120];
    uint8_t temp = 0xff, temp_last = 0;
bool isWallFound = false;
float wallDistanceCM = 100.;


void setupCam() {
  SPI.begin();
  Wire.begin();
  // put your setup code here, to run once:
  uint8_t vid, pid;
  uint8_t temp;
  //Serial.println(F("ACK CMD ArduCAM Start! END"));
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100); 
  
  //Change to JPEG capture mode and initialize the OV5642 module
  temp = 0xff;
  myCAM.set_format(BMP);
  myCAM.InitCAM();
  myCAM.OV5642_set_Special_effects(BW);temp = 0xff;

  myCAM.clear_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
    myCAM.wrSensorReg16_8(0x3818, 0x81);
      myCAM.wrSensorReg16_8(0x3621, 0xA7);
  //myCAM.OV5642_set_RAW_size(OV5642_640x480);
  //myCAM.OV5642_set_Color_Saturation(Saturation_4);temp = 0xff;
  //myCAM.OV5642_set_Contrast(Contrast4);temp = 0xff;
  //myCAM.OV5642_Test_Pattern(Color_bar);temp = 0xff;
  //myCAM.OV5642_set_Brightness(Brightness2);temp = 0xff;
  //myCAM.OV5642_set_Exposure_level(5);
  //myCAM.OV5642_set_hue(degree60);temp = 0xff;
  myCAM.OV5642_set_Mirror_Flip(FLIP);temp = 0xff;
  start_capture = 1;
    temp = 0xff;
    mode = 49;
}

void doTheThing() {
// put your main code here, to run repeatedly:

  if (start_capture == 1)
  {
    myCAM.flush_fifo();
    myCAM.clear_fifo_flag();
    //Start capture
    myCAM.start_capture();
    start_capture = 0;
  }
  //  delay(1000);
  

}

void getTheStuff(){
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
  {
    delayMicroseconds(10);
  }
    uint32_t length = 0;
    length = myCAM.read_fifo_length();
    if (length >= MAX_FIFO_SIZE ) 
    {
      //Serial.println(F("ACK CMD Over size. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    if (length == 0 ) //0 kb
    {
      //Serial.println(F("ACK CMD Size is 0. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    myCAM.CS_LOW();
    myCAM.set_fifo_burst();//Set fifo burst mode
  
    count = 0;
    for (i = 0; i < 10; i++)
    {
      for (j = 0; j < 320; j++)
      {
        if(j == 159){
          in_arr[count] = SPI.transfer16(0x0000);// << 8;
          count++;
        }else{
          VH = SPI.transfer16(0x0000);
        }
      }
    }
    myCAM.CS_HIGH();
    SPI.endTransaction();
    int good_val = 120;
      uint8_t blue2 = ((in_arr[9] >> 8) & 0x01F);
      uint8_t green2 = (((in_arr[9] >> 13)) | ((in_arr[9] << 3) & 0x0038));
      uint8_t red2 = ((in_arr[9] >> 3) & 0x001F);
      bool isdiff = (float(blue2)*.07) + (float(green2)*.35) + (float(red2)*.22) < 10.;
    //Serial.println("Blue:\t" + String(blue2)+"\nRed:\t"+String(red2)+"\nGreen:\t"+String(green2));
      if(isdiff){
        digitalWrite(13,HIGH);
        good_val = 1;
      }
    
    if(good_val == 120 ){
      //Serial.println("Wall is close");
      digitalWrite(13,LOW);
      isWallFound = true;
    }else{
      isWallFound = false;
    }

    //Clear the capture done flag
    myCAM.clear_fifo_flag();
    start_capture = 1;
}

void getTheDistanceCM(){
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
  {
    delayMicroseconds(10);
  }
    uint32_t length = 0;
    length = myCAM.read_fifo_length();
    if (length >= MAX_FIFO_SIZE ) 
    {
      //Serial.println(F("ACK CMD Over size. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    if (length == 0 ) //0 kb
    {
      //Serial.println(F("ACK CMD Size is 0. END"));
      myCAM.clear_fifo_flag();
      return;
    }
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    myCAM.CS_LOW();
    myCAM.set_fifo_burst();//Set fifo burst mode
    count = 0;
    for (i = 0; i < 120; i++)
    {
      for (j = 0; j < 320; j++)
      {
        if(j == 159){
          in_arr[count] = SPI.transfer16(0x0000);// << 8;
          count++;
        }else{
          VH = SPI.transfer16(0x0000);
        }
      }
    }
    myCAM.CS_HIGH();
    SPI.endTransaction();
    int good_val = 120;
    i = 119;
    int highcount = 0;
    //Serial.println("Start of a new frame");
    while(i > 0){
      uint8_t blue2 = ((in_arr[i] >> 8) & 0x01F);
      uint8_t green2 = (((in_arr[i] >> 13)) | ((in_arr[i] << 3) & 0x0038));
      uint8_t red2 = ((in_arr[i] >> 3) & 0x001F);
      bool isdiff = (float(blue2)*.3) + (float(green2)*.15) + (float(red2)*.3) < 9.;//(20.5-((15./119.)*(120-i)));
      //Serial.println("\tPixel #" + String(i) + "\t" + String((float(blue2)*.3) + (float(green2)*.15) + (float(red2)*.3)));
    //Serial.println("Blue:\t" + String(blue2)+"\nRed:\t"+String(red2)+"\nGreen:\t"+String(green2));
      if(isdiff){
        highcount++;
      }else
        highcount = 0;
      if(highcount == 3){
        good_val = i+3;
        break;
      }
      i--;
    }
      
    
    if(good_val == 120 ){
      //Serial.println("Wall is close");
      digitalWrite(13,LOW);
      isWallFound = true;
      i = 113-10;
    }else{
      isWallFound = false;
    }

    //Clear the capture done flag
    myCAM.clear_fifo_flag();
    start_capture = 1;
    wallDistanceCM = DISTANCEMAP[i+3]/10.;
}