#include <SPI.h>
#include "WejUSBMIDI.h"
#include "MidiMessage.h"
#include "WejDraw.h"

#define DRAW_BANK 8
class Wej: public WejUSBMIDI
{
  public:
    Wej();
    virtual ~Wej();
            void bootDisplay();
    virtual void handleMidiMessage(int portnum,MidiMessage *msg);
    virtual void deviceConnected(int portnum);
    virtual void deviceDisconnected(int portnum);
    virtual void task();
    virtual void handleBtleMidiIn(MidiMessage *msg); //in from btle
  private:
    bool mBangDown;
    byte mBrightness;
    byte mBrightnessParameter;
    unsigned long mDropSpeed;
    unsigned long mFrameTime;
    unsigned long mLastMidiPacketSentTime;
    unsigned long mTestBeat;
    byte    mDrawPhase;
    Exp     mDraw;
    bool    mToggle;
};
