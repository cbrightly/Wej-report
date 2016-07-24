#include "Adafruit_DotStar.h"
#include "Wej.h"
#define WEJ_LEDS 48
#define LEDS_BLOCK 4
#define DRAW_BANK 8
#define DATAPIN    A5
#define CLOCKPIN   A4
enum
{
    kMod1 = 0,
    kMod2 = 1,
    kMod3 = 2,
    kPatternSelect = 3,
    kBang = 4,
    kBeat = 5,
    kBangUp = 6,
    kBrightness = 7
};
Adafruit_DotStar  strip(WEJ_LEDS, DATAPIN, CLOCKPIN,DOTSTAR_BGR);

Wej::Wej() :  WejUSBMIDI(), mDraw(WEJ_LEDS,8)
{
  strip.begin();
  mDrawPhase = 0;
  strip.clear();
  strip.show();
  bootDisplay();
  delay(500);
  mBrightness = 1;
  mDropSpeed = 1000;
  mBrightnessParameter = 16;
  mFrameTime = 0;
  mToggle = false;
  mBangDown = false;
}
Wej::~Wej()
{
}
void
Wej::bootDisplay()
{
  uint8_t r[] = {16, 26, 37, 30, 31, 34, 38, 13, 39, 14, 45, 20, 41, 15, 36, 12, 42, 2, 8, 32, 10, 22, 28, 21, 29, 43, 25, 24, 5, 44, 23, 27, 3, 19, 35, 46, 18, 11, 7, 6, 40, 0, 4, 9, 33, 47, 17, 1};
  int p = 0;
  for (int i=0;i<WEJ_LEDS;i++)//white
  {
    uint8_t idx = r[i];
    strip.setPixelColor(idx,200,200,200);
    strip.show();  
  }
  delay(300);
  for (int i=0;i<WEJ_LEDS;i++)//red
  {
    uint8_t idx = r[i];
    strip.setPixelColor(idx,255,0,0);
    strip.show();  
  }
  delay(300);

  for (int i=0;i<WEJ_LEDS;i++)//gree
  {
    uint8_t idx = r[i];
    strip.setPixelColor(idx,0,255,0);
    strip.show();  
  }
  delay(300);
  for (int i=0;i<WEJ_LEDS;i++)//blue
  {
    uint8_t idx = r[i];
    strip.setPixelColor(idx,0,0,255);
    strip.show();  
  }
  strip.show();
  for (int f=0;f<255;f+=4)
  {
    for (int i=0;i<WEJ_LEDS;i++)//blue
    {
      strip.setPixelColor(i,0,0,254-f);
    }
    strip.show();
  }  
  strip.clear();
  strip.show();  
}
void
Wej::task()
{
  unsigned long now = millis();
  if (mFrameTime==0 || now-mFrameTime>mDropSpeed)
  {
    mFrameTime = now;
    if (mBrightness>mBrightnessParameter/8)
       mBrightness--;
  }
  mDraw.draw(&strip,mDrawPhase,LEDS_BLOCK,now);
  mDrawPhase+=LEDS_BLOCK;
  mDrawPhase %= WEJ_LEDS;
  for (int i=0;i<4;i++)
    strip.showTask(mBrightness);
  WejUSBMIDI::task();
}
void
Wej::handleBtleMidiIn(MidiMessage *msg)
{
/*  if (mToggle)
  {
     mDraw.setVariation(1);
     mToggle = false;
  }
  else
  {
    mDraw.setVariation(0);
    mToggle = true;
  }*/
  if (msg->isSysEx() && msg->data[1]==42)// && msg->size == 5)
  //if (msg->isController())
  {
    uint8_t what = msg->data[2];
    uint8_t value = msg->data[3];
    //uint8_t what = msg->controlNumber();
    //uint8_t value = msg->controlValue();
    if (what == kBang)
    {
      mBangDown = true;
      mDropSpeed = 2500;
      mBrightness = mBrightnessParameter;
      mDraw.beat(&strip);
    }
    if (what == kBangUp)
    {
      mBangDown = false;
      mDropSpeed = 10;
    }
    if (what == kBeat && !mBangDown)
    {
      mBrightness = mBrightnessParameter;
      mDropSpeed = 6;
      mDraw.beat(&strip);
    }
    else if (what == kPatternSelect)
    {
      if (value < DRAW_BANK)
        mDraw.setVariation(msg->data[3]);
    }
    else if (what >= kMod1 && what <= kMod3) 
    {
       mDraw.setMod(what,value);
    }
    else if (what == kBrightness)
    {
      mBrightnessParameter = value / 4;
      if (mBrightnessParameter<8)
        mBrightnessParameter = 8;
      mBrightness = mBrightnessParameter / 8;
    }
    
  }
  
}
void
Wej::handleMidiMessage(int portnum,MidiMessage *msg)
{
  if (msg->isNoteOn())
  {
  }
  else if (msg->isNoteOff())
  {
  }
}
void
Wej::deviceConnected(int portnum)
{
  /*if (portnum==1)
  {
     strip.setPixelColor(23,100,100,100);
     strip.setPixelColor(24,100,100,100);
  }
  else if (portnum==2)
  {
      strip.setPixelColor(0,100,100,100);
      strip.setPixelColor(47,100,100,100);
  }
  strip.show();*/
}
void
Wej::deviceDisconnected(int portnum)
{
  /*if (portnum==1)
  {
      strip.setPixelColor(23,0,0,0);
      strip.setPixelColor(24,0,0,0);
  }
  else if (portnum==2)
  {
      strip.setPixelColor(0,0,0,0);
      strip.setPixelColor(47,0,0,0);
  }
  strip.show();*/
}
