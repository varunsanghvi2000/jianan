/*****************************************************************************
  The MIT License (MIT)

  Copyright (c) 2016 by bbx10node@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 **************************************************************************/
#include <i2s.h>
#include "wavspiffs.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_IS31FL3731.h>

// If you're using the full breakout...
Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();


//const char* ssid = "404";
//const char* password = "user not found";

const char* ssid = "NETGEAR68";
const char* password = "jollymint969";

unsigned int remoteport=1390 ;      // local port to listen on
unsigned int localPort =  1809 ;      // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet
char *pt;
char *sec;
int comman=0;
int deadswitch=0;
int value;


IPAddress remoteip(192, 168, 1, 65);
WiFiUDP Udp;


static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };



    


// Non-blocking version of I2S write for left and right 16-bit PCM
// should actually already be in i2s library...
bool ICACHE_FLASH_ATTR i2s_write_lr_nb(int16_t left, int16_t right)
{
  int sample = (right) & 0xFFFF;
  sample = sample << 16;
  sample |= (left) & 0xFFFF;
  return i2s_write_sample_nb(sample); // 32bit write nb = non-blocking, when DMA is full returns false
}

struct I2S_status_s {
  wavFILE_t wf;
  int16_t bufferarray[512];
  int bufferlen;
  int buffer_index;
  int playing;
} I2S_WAV;

void wav_stopPlaying()
{
  I2S_WAV.playing = false;
  wavClose(&I2S_WAV.wf);
}

void wav_setup()
{
  Serial.println(F("wav_setup"));
  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = false;
  i2s_begin();
}

void wav_loop()
{
  int rc;
 
  while (I2S_WAV.playing && !i2s_is_full()) {
    while (I2S_WAV.buffer_index < I2S_WAV.bufferlen) {
      int16_t pcmleft = I2S_WAV.bufferarray[I2S_WAV.buffer_index];
      int16_t pcmright = pcmleft; 
      if (i2s_write_lr_nb(pcmleft, pcmright)) {
        I2S_WAV.buffer_index++;
      }
      else  break;
    }
    yield(); // must feed WDT dog or will crash but not inside loop, causes sound glitches
    if (i2s_is_full()) break;

    rc = wavRead(&I2S_WAV.wf, I2S_WAV.bufferarray, sizeof(I2S_WAV.bufferarray));
    if (rc > 0) {
      I2S_WAV.bufferlen = rc / sizeof(I2S_WAV.bufferarray[0]);
      I2S_WAV.buffer_index = 0;
    }
    else {
      Serial.println(F(" Stop playing."));
      wav_stopPlaying();
      break;
    }
  }
}
  
void wav_startPlayingFile(const char *wavfilename)
{
  wavProperties_t wProps;
  int rc;

  Serial.print("Playing File ");Serial.print(wavfilename);
  rc = wavOpen(wavfilename, &I2S_WAV.wf, &wProps);
  if (rc != 0) {
    Serial.println("wavOpen failed");
    return;
  }
  i2s_set_rate(wProps.sampleRate);

  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = true;
  wav_loop();
}

/**************************************************************************************/


void showDir(void)
{
  wavFILE_t wFile;
  wavProperties_t wProps;
  int rc;

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.println(dir.fileName());
    rc = wavOpen(dir.fileName().c_str(), &wFile, &wProps);
    if (rc == 0) {
      Serial.printf("  audioFormat %d\r\n", wProps.audioFormat);
      Serial.printf("  numChannels %d\r\n", wProps.numChannels);
      Serial.printf("  sampleRate %d\r\n", wProps.sampleRate);
      Serial.printf("  byteRate %d\r\n", wProps.byteRate);
      Serial.printf("  blockAlign %d\r\n", wProps.blockAlign);
      Serial.printf("  bitsPerSample %d\r\n", wProps.bitsPerSample);
      Serial.println();
      wavClose(&wFile);
    }
  }
}

void setupudp()
{
  
  WiFi.begin(ssid, password);// Connect to WiFi 
  WiFi.config(IPAddress(192, 168, 1, 65),
  IPAddress(192, 168, 1, 1),
  IPAddress(255, 255, 255, 0));
  
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);

}

void readdata()
{
    Dir dir = SPIFFS.openDir("/");
    int packetSize = Udp.parsePacket();
    if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    
    pt = strtok (packetBuffer,":");
    sec=pt;
    Serial.println("command ");
    Serial.println(sec);
    comman=atoi(sec);
    pt = strtok (NULL,":");
    value=atoi(pt);
    Serial.println("value ");
    Serial.println(value);
    if(comman==5){
      if(value==1){
        matrix.clear();
        matrix.drawBitmap(3, 0, smile_bmp, 8, 8, 255);
      }
      else if(value==2){
        matrix.clear();
      matrix.drawBitmap(3, 0, neutral_bmp, 8, 8, 64);

      }
      else if(value==3){
       matrix.clear();
       
       matrix.drawBitmap(3, 0, frown_bmp, 8, 8, 32);

      }
    }else if(comman==8){
      if(value==1){
        
        wav_startPlayingFile("/call_to_arms_8."); 
        // wav_startPlayingFile("/example.wav");
         while (I2S_WAV.playing) {
            wav_loop();
            // can do other things here for short periods of time delay(4);
          }
    
      }
      else if(value==2){
       wav_startPlayingFile("/baby_cry."); 
        // wav_startPlayingFile("/example.wav");
         while (I2S_WAV.playing) {
            wav_loop();
            // can do other things here for short periods of time delay(4);
          }

      }
      else if(value==3){
          wav_startPlayingFile("/call_to_arms_8."); 
        // wav_startPlayingFile("/example.wav");
         while (I2S_WAV.playing) {
            wav_loop();
            // can do other things here for short periods of time delay(4);
          }

      }
    }
    
    
    }
 }
  


void setup()
{
  Serial.begin(115200); Serial.println();
  Serial.println("Sound through I2S device and SPIFF file");
  setupudp();
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS.begin() failed");
    return;
  }
  //SPIFFS.format();
  // Confirm track files are present in SPIFFS
  //showDir();

  wav_setup();
  
  if (! matrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 Found!");
    
}


void loop()
{
  matrix.setRotation(0);
  readdata();
  Dir dir = SPIFFS.openDir("/");
//  while (dir.next()) {
//    wav_startPlayingFile(dir.fileName().c_str()); 
//    // wav_startPlayingFile("/example.wav");
//    while (I2S_WAV.playing) {
//      wav_loop();
//     // can do other things here for short periods of time delay(4);
//    }
//    delay(1000);
//  }
}
