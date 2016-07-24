#include "Adafruit_DotStar.h"
extern const uint32_t rainbow[];
class WejDraw
{
  public:
    WejDraw(int num_leds);
    ~WejDraw();
    static void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);
    void setMod(byte mod,byte val);
    virtual void beat(Adafruit_DotStar *strip);
    void drawByte(byte b0,byte b1,byte b2,bool on);
    virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);
  protected:
    byte mNumLeds;
    unsigned long mDrawByte;
    unsigned long mBeatTime;
    bool mDrawByteOn;
    byte mDrawByteStartAt;
    
};
/*class Parts : public WejDraw
{
  public:
     Parts(int num_leds);
   
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);          
  byte mPart;
};
class Flash : public WejDraw
{
  public:
    
    Flash(int num_leds);
    virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
};

class Wipe : public WejDraw
{
  public:
     Wipe(int num_leds);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
};

class FullOn : public WejDraw
{
  public:
     FullOn(int num_leds);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
};

class Blocks : public WejDraw
{
  public:
     Blocks(int num_leds);
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     uint8_t mPhase;
};

class Twinkle : public WejDraw
{
  public:
     Twinkle(int num_leds);
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     uint16_t mStars;  
};

class Drop : public WejDraw
{
  public:
     Drop(int num_leds);
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     int mBeats;
};

class On : public WejDraw
{
  public:
     On(int num_leds);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
};

class BigWipe : public WejDraw
{
  public:
      BigWipe(int num_leds);
      virtual void beat(Adafruit_DotStar *strip);
      virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);
};
class TimeDraw : public WejDraw
{
  public:
      TimeDraw(int num_leds);
      virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time); 
  //float particlePos[16];
  //float particleVelocity[16];
  unsigned long mLast;
  unsigned long mNow;
};

class Count : public WejDraw
{
  public:
     Count(int num_leds);
     virtual void beat(Adafruit_DotStar *strip);
             void setCount(int c);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     int mCount;  
};

class Glow : public WejDraw
{
  public:
     Glow(int num_leds);
             void frame();
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     unsigned long mNow;
     unsigned long mLast;
     byte mBeats;
     byte mIntensity;
};

class DiscoBall : public WejDraw
{
  public:
     DiscoBall(int num_leds);
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
     byte mPhase;
};
class OffDraw : public WejDraw
{
  public:
     Off(int num_leds);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
};*/

class OffDraw : public WejDraw
{
  public:
     OffDraw(int num_leds);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);     
  private:
};

class Exp : public WejDraw
{
  public:
     Exp(int num_leds,int variation);
             void setVariation(int variation);
     virtual void beat(Adafruit_DotStar *strip);
     virtual void draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time);
     byte        interpolateTable(byte idx,byte *table,byte tablesize);
  private:
     byte       mVariation;
     byte *     mSpatialMap;
     byte       mSpatialMapSize;
     
     byte *     mColorIndex;
     byte       mColorIndexSize;
     byte       mColorOffset[4];
     byte       mColorWidth[4];
     uint32_t * mColorMap;
     byte       mColorMapSize;
     
     byte *     mBrightMap;
     byte       mBrightMapSize;
     
};
