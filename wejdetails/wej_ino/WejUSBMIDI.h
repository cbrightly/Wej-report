#include <SPI.h>
#include "MidiMessage.h"
#include "ch9.h"
#include "usb.h"

#define MAX_BUF_SIZE 256
#define USB_SUBCLASS_MIDISTREAMING 3
#define USB_CLASS_AUDIO 1
#define USB_CLASS_HUB 9
#define USB_SUBCLASS_HUB  0

typedef enum
{
  SOF,
  LENGTH,
  MAJORID,
  MINORID,
  PAYLOAD,
  FEND
}ParserState;

const uint8_t UT_CHAR_START = 0x7E;
const uint8_t UT_CHAR_ESC = 0x7D;
const uint8_t UT_CHAR_END = 0x7F;
const uint8_t MSG_ID_MAJOR_MIDI = 0x55;
const uint8_t MSG_ID_MINOR_MIDI = 0x02;

typedef struct
{
  uint8_t address;
  uint8_t configuration;
  uint8_t interface;
  uint8_t inputEndPointAddress;
  uint8_t outputEndPointAddress;
} midi_usbConfiguration;

typedef struct
{
  uint8_t address;
  uint8_t configuration;
  uint8_t interface;
  uint8_t inputEndPointAddress;
} hub_usbConfiguration;


class WejUSBMIDI
{
public:
  WejUSBMIDI();
  virtual ~WejUSBMIDI();
          void start();
  WejUSBMIDI * getActive();       
  virtual void deviceConnected(int portnum){};
  virtual void deviceDisconnected(int portnum){};
  virtual void handleSysExByte(byte b){};
  virtual void handleMidiMessage(int portnum,MidiMessage *msg){}; //in from usb 
  virtual void handleBtleMidiIn(MidiMessage *msg) {}; //in from btle

          void processBtleMidiBuffer(uint8_t *buf,uint8_t len);
          bool midiAvailable();
          uint8_t midiRaw(uint8_t *buf,uint8_t cnt);
          bool midiRead(uint8_t &stat,uint8_t &byte1,uint8_t &byte2);
          void writeBtleMidi(uint8_t b);
          void flushBtleMidi();
          uint8_t btleBytesInPacket();
  virtual void task();
private:
         void writeUSBMidiByte(uint8_t *buf); //write 4 bytes
         void convertBTLEToUSB();
  static void usbEventHandler(usb_device * device, usb_eventType event);
  bool isHubInterface(usb_interfaceDescriptor *interface);
  bool isMidiInterface( usb_interfaceDescriptor * interface);
  bool isMidiDevice( usb_device * device, int configuration,  midi_usbConfiguration * handle);
  bool isHubDevice( usb_device * device, int configuration,  hub_usbConfiguration * handle);
  void initUsbMidi( usb_device * device,  midi_usbConfiguration * handle);
  void initUsbHub( usb_device * device,  hub_usbConfiguration * handle);

  uint8_t mPacketByteOffset;
  uint8_t mMidiPacket[20];
  unsigned long mLastFlush;
  
  usb_device *midi[2];
  usb_device *hub;
  static WejUSBMIDI *sWej;
  MidiMessage mBtleMidiMessage;
  MidiMessage mUsbMidiMessage;
  bool    mRunningStatus;
  byte    mLastStatus;
  bool    mEndOfPacket;
  bool    mInSysEx;
  byte    mUSBBufferWriteIndex;
  byte    mUSBBuffer[4];
  uint8_t     mNumInputCables[2];
};
