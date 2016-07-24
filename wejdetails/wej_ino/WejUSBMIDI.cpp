#include <SPI.h>
#include "usb.h"
#include "ch9.h"
#include "WejUSBMIDI.h"
WejUSBMIDI * WejUSBMIDI::sWej = NULL;
static void usbEventHandler(usb_device * device, usb_eventType event);

WejUSBMIDI::WejUSBMIDI()
{
  hub = NULL;
  midi[0] = NULL;
  midi[1] = NULL;
  mInSysEx = false;
  mRunningStatus = false;
  memset(mUSBBuffer,0,4);  
  mUSBBufferWriteIndex = 0;
  mEndOfPacket = false;
  delay(1000);
  mPacketByteOffset = 0;
  mLastFlush = millis();
  mNumInputCables[0] = 1;  
  mNumInputCables[1] = 1;
  Serial.begin(115200);

}
WejUSBMIDI::~WejUSBMIDI()
{
}
void WejUSBMIDI::start()
{
  sWej = this;
  USB::setEventHandler(usbEventHandler);
  USB::init();
}
WejUSBMIDI * WejUSBMIDI::getActive()
{
  return sWej;
}
bool
WejUSBMIDI::midiAvailable()
{
  return false;
}
uint8_t 
WejUSBMIDI::midiRaw(uint8_t *buf,uint8_t count)
{
  
}

bool
WejUSBMIDI::midiRead(uint8_t &stat,uint8_t &byte1,uint8_t &byte2)
{
  
}
void
WejUSBMIDI::writeBtleMidi(uint8_t byte)
{
  if (mPacketByteOffset==0)
  {
      uint32_t ms = millis();
      uint8_t head_ts = ms>>7;
      head_ts |= 1 << 7; //set the 7th bit to 1
      head_ts &= ~(1 << 6); //set the 6th bit to zero
      mMidiPacket[mPacketByteOffset++] = head_ts;
  }
  mMidiPacket[mPacketByteOffset++] = byte;
  if (mPacketByteOffset==20)
      flushBtleMidi();
}
void
WejUSBMIDI::flushBtleMidi()
{
  mLastFlush = millis();
  if (mPacketByteOffset>0)
  {
    uint8_t buf[255];
    uint8_t idx = 0;
    buf[idx++] = UT_CHAR_START;
    buf[idx++] = mPacketByteOffset + 2;
    buf[idx++] = MSG_ID_MAJOR_MIDI;
    buf[idx++] = MSG_ID_MINOR_MIDI;
    for (int i=0;i<mPacketByteOffset;i++)
    {
      uint8_t b = mMidiPacket[i];
      if (b==UT_CHAR_START ||  b==UT_CHAR_ESC || b==UT_CHAR_END)
      {
        buf[idx++] = UT_CHAR_ESC;
        buf[idx++] = (b ^ 0x20);  
      }
      else
      {
        buf[idx++] = b;
      }
    }
    buf[idx++] = UT_CHAR_END;
    uint8_t pos = 0;
    char bufbefore[] = {UT_CHAR_START,3,98,99,0,UT_CHAR_END};
    //Serial.write(bufbefore,sizeof(bufbefore));
    if (idx<32)
    {
      uint8_t w = Serial.write(buf,idx);
      
      idx -= w;
      pos += w;
    }
    char bufend[] = {UT_CHAR_START,3,100,101,1,UT_CHAR_END};
    //Serial.write(bufend,sizeof(bufend));
    //Serial.flush();
    mPacketByteOffset = 0;
  }
}
uint8_t 
WejUSBMIDI::btleBytesInPacket()
{
  return mPacketByteOffset;
}

void
WejUSBMIDI::processBtleMidiBuffer(uint8_t *buf,uint8_t len)
{
  uint8_t idx = 0;
  byte header = buf[idx++];
  byte usb_byte[4];

  while (idx<len)
  {
    if (!mInSysEx)
    {
      byte nextByte = buf[idx+1];  //idx + 0 is a timestamp in all cases but running status.
      if (nextByte==0xF0) //begin sysex
      {
        byte timestamp = buf[idx++];
        byte stat = buf[idx++];
        byte byte1 = buf[idx++];
        byte byte2 = buf[idx++];
        mUSBBufferWriteIndex = 0;
        mBtleMidiMessage.clear();
        mBtleMidiMessage.appendByte(stat);
        mBtleMidiMessage.appendByte(byte1);
        mBtleMidiMessage.appendByte(byte2);     
        if (byte1==0xF7) //two byte sysex
        {
          handleBtleMidiIn(&mBtleMidiMessage);
          usb_byte[0] = 0x06;
          usb_byte[1] = 0xf0;
          usb_byte[2] = 0xf7;
          usb_byte[3] = 0;
          //dont send sysex to external devices writeUSBMidiByte(usb_byte);
        }
        else if (byte2==0xF7) //three byte sysex
        {
          handleBtleMidiIn(&mBtleMidiMessage);
          usb_byte[0] = 0x06;
          usb_byte[1] = 0xf0;
          usb_byte[2] = byte1;
          usb_byte[3] = 0xf7;
          //dont send sysex to external devices writeUSBMidiByte(usb_byte);
        }
        else //n byte sysex
        {
          //todo write the start of the sysex message out to usb.
          usb_byte[0] = 0x04;
          usb_byte[1] = stat;
          usb_byte[2] = byte1;
          usb_byte[3] = byte2;
          //dont send sysex to external devices writeUSBMidiByte(usb_byte);
          mInSysEx = true;
        }
      }
      else if (nextByte&128) //is status.  standard message
      {
        byte timestamp = buf[idx++];
        byte stat = buf[idx++];
        byte byte1 = buf[idx++];
        byte byte2 = buf[idx++];
        mLastStatus = stat;
        mBtleMidiMessage.clear();
        mBtleMidiMessage.appendByte(stat);
        mBtleMidiMessage.appendByte(byte1);
        mBtleMidiMessage.appendByte(byte2);
        handleBtleMidiIn(&mBtleMidiMessage);
        usb_byte[0] = stat>>4;
        usb_byte[1] = stat;
        usb_byte[2] = byte1;
        usb_byte[3] = byte2;
        writeUSBMidiByte(usb_byte);
      }
      else //running status
      {
        mRunningStatus = true;
        byte stat = mLastStatus;
        byte byte1 = buf[idx++];
        byte byte2 = buf[idx++];
        mBtleMidiMessage.clear();
        mBtleMidiMessage.appendByte(stat);
        mBtleMidiMessage.appendByte(byte1);
        mBtleMidiMessage.appendByte(byte2);
        handleBtleMidiIn(&mBtleMidiMessage);
        usb_byte[0] = stat>>4;
        usb_byte[1] = stat;
        usb_byte[2] = byte1;
        usb_byte[3] = byte2;
        writeUSBMidiByte(usb_byte);
      }
    }
    if (mInSysEx)
    {
      byte sysbyte = buf[idx++];
      if (sysbyte==0xF7) //end of sysex
      {
        mInSysEx = false;
        mBtleMidiMessage.appendByte(sysbyte);
        if (mBtleMidiMessage.isValidSysEx())
            handleBtleMidiIn(&mBtleMidiMessage);
        mUSBBuffer[mUSBBufferWriteIndex]  = sysbyte;
        mUSBBufferWriteIndex ++;
        usb_byte[0] = (4 + mUSBBufferWriteIndex); //(page 16 http://www.usb.org/developers/docs/devclass_docs/midi10.pdf)
        usb_byte[1] = 0;
        usb_byte[2] = 0;
        usb_byte[3] = 0;
        for (int i=0;i<mUSBBufferWriteIndex;i++)
        {
          usb_byte[i+1] = mUSBBuffer[i];
        }
        mUSBBufferWriteIndex = 0;
        //dont send sysex to external devices writeUSBMidiByte(usb_byte);
      }
      else if (sysbyte& 1<<7) //timestamp
      {
        
      }  
      mBtleMidiMessage.appendByte(sysbyte);
      mUSBBuffer[mUSBBufferWriteIndex]  = sysbyte;
      mUSBBufferWriteIndex ++;
      if (mUSBBufferWriteIndex==3)
      {
        usb_byte[0] = 0x04;
        for (int i=0;i<mUSBBufferWriteIndex;i++)
        {
          usb_byte[i+1] = mUSBBuffer[i];
        }
        mUSBBufferWriteIndex=0;
        //dont send sysex to external devices writeUSBMidiByte(usb_byte);         
      }
    }
  }
}

void
WejUSBMIDI::convertBTLEToUSB()
{
  static uint8_t thisByte;
  static bool byteEscaped = false; // This current byte followed an escape byte
  static ParserState state = SOF;         // The field that we expecting this byte to be part of
  static uint16_t identifier = 0xffff;
  static uint8_t payloadIndex = 0;    
  static uint8_t payloadLength = 0;
  static uint8_t payloadBuffer[128];
  static uint16_t readSize = 0;
  while (Serial.available())
  {
    thisByte = Serial.read();
    
    if(thisByte == UT_CHAR_ESC)
    {
      byteEscaped = true;
      return;
    }
    
    if(thisByte == UT_CHAR_START)
    {
      state = SOF;
      byteEscaped = false;
    }
    
    if(byteEscaped == true)
    {
      thisByte ^= 0x20;
      byteEscaped = false;
    }
    
    
    switch(state)
    {
      case SOF:
      {
        if(thisByte == UT_CHAR_START)
        {
          payloadLength = 0;
          payloadIndex = 0;
          state = LENGTH;
        }
      }
      break;
      
      case LENGTH:
      {
        payloadLength = thisByte - 2;
        state = MAJORID;
      }
      break;
      
      case MAJORID:
      {
        identifier = ((uint16_t)thisByte) << 8;
        state = MINORID;
      }
      break;
      
      case MINORID:
      {
        identifier |= ((uint16_t)thisByte) & 0x00FF;
        state = PAYLOAD;
      }
      break;
      
      case PAYLOAD:
      {
        payloadBuffer[payloadIndex++] = thisByte;
        if(payloadIndex == payloadLength)
        {
          state = FEND;
        }
      }
      break;
      
      case FEND:
      {
        if(thisByte == UT_CHAR_END)  //check identifier
        {
          processBtleMidiBuffer(payloadBuffer,payloadLength);
          state = SOF;
        }
        else 
        {
          return;
        }
      }
      break;      
    }
  }
}

void WejUSBMIDI::task() 
{
  unsigned long now = millis();
  // put your main code here, to run repeatedly:
  USB::poll();
  usb_hubEvent evt;
  uint8_t data[64];
  if (hub)
  {
    memset(data,0,64);
    int r = USB::bulkRead(hub,64,data,true);
    for(uint8_t port = 1, mask = 0x02; port < 8; mask <<= 1, port++)
    {
      if(data[0] & mask)
      {
        USB::getPortStatus(hub,port,4,evt.evtBuff);
        if (evt.bmEvent == bmHUB_PORT_EVENT_DISCONNECT)
        {
          deviceDisconnected(port);
          USB::clearPortFeature(hub,HUB_FEATURE_C_PORT_ENABLE, port, 0);
          USB::clearPortFeature(hub,HUB_FEATURE_C_PORT_CONNECTION, port, 0);
          USB::removeDevice(port);
          WejUSBMIDI::sWej->midi[port-1] = NULL;

        }
        else if (evt.bmEvent == bmHUB_PORT_EVENT_CONNECT || evt.bmEvent == bmHUB_PORT_EVENT_LS_CONNECT)
        {
          deviceConnected(port);
          USB::resetHubPort(hub,port);
          USB::newDevice(port);        
        }
        else
        {
          USB::clearPortFeature(hub,HUB_FEATURE_C_PORT_ENABLE, port, 0);
          USB::clearPortFeature(hub,HUB_FEATURE_C_PORT_CONNECTION, port, 0);
          delay(100);
        }
      }
    }
  }
  uint8_t cin_size_map[] = {0,0,2,3,3,1,2,3,3,3,3,3,1,1,3,1};
  //convert https://developer.apple.com/bluetooth/Apple-Bluetooth-Low-Energy-MIDI-Specification.pdf
  //to http://www.usb.org/developers/docs/devclass_docs/midi10.pdf
  int  portReadCount = 0;
  for (int p=0;p<2;p++)
  {
    if (midi[p])
    {
      bool isFirstTime = true;
      bool isSysEx = false;
      while (isFirstTime || isSysEx)
      {
        isFirstTime = false;
        memset(data,0,64);
        int r = USB::bulkRead(midi[p],64,data,true);
        if (r>=0)
        {
            //Serial.println(r);
            portReadCount ++;
        }
        uint8_t lastStatus = 0;
        for (int i=0;i<r;i+=4)
        {
          uint8_t msg_timestamp = millis();
          msg_timestamp |= 1<<7;
          uint8_t cin = data[i]&0x0f;
          uint8_t bytes_in_message = cin_size_map[cin];   
          if (cin==0x4) //in sysx or continue.  Simply write the bytes
          {
            isSysEx = true;
            mUsbMidiMessage.clear();
            for (int b=0;b<3;b++)
            {
              //writeBtleMidi(data[i+1+b]);
              mUsbMidiMessage.appendByte(data[i+1+b]);
            }
          }
          else if (cin>=0x5 && cin<=7)
          {
            isSysEx = false;
            writeBtleMidi(msg_timestamp);
            int bytes_left = cin-4;
            for (int b=0;b<bytes_left;b++)
            {
              //writeBtleMidi(data[b]);
              mUsbMidiMessage.appendByte(data[b]);
            }
            handleMidiMessage(p+1,&mUsbMidiMessage);
          }
          else if (cin>=0x8 && cin <=0xE)
          {
            mUsbMidiMessage.clear();
            mUsbMidiMessage.appendByte(data[i+1]);
            mUsbMidiMessage.appendByte(data[i+2]);
            mUsbMidiMessage.appendByte(data[i+3]);
            handleMidiMessage(p+1,&mUsbMidiMessage);
            if (lastStatus == data[i+1]) 
            {
              if (btleBytesInPacket()>18) //blte packets are 20 bytes. 
                 flushBtleMidi();
              writeBtleMidi(data[i+2]);
              writeBtleMidi(data[i+3]); 
            }
            else
            {
              lastStatus = data[i+1];
              if (btleBytesInPacket()>16)
                 flushBtleMidi();
              writeBtleMidi(msg_timestamp);
              for (int b=0;b<bytes_in_message;b++)
              {
                writeBtleMidi(data[i+b+1]);
              }
            }
          }
          
        }
      }
    }
  }
  if (now-mLastFlush > 7)
  {
      flushBtleMidi();
  }
  convertBTLEToUSB();
}

void WejUSBMIDI::writeUSBMidiByte(uint8_t *buf)
{
  for (int p=0;p<2;p++)
  {
    if (midi[p])
    {
      uint8_t stat = buf[0];
      for (int c=0;c<mNumInputCables[p];c++)
      {
        buf[0] = (c<<4) + stat;
        USB::bulkWrite(midi[p],4,buf);
      }
    }
  }
}
bool WejUSBMIDI::isHubInterface(usb_interfaceDescriptor *interface)
{
  if (interface->bInterfaceClass==USB_CLASS_HUB && interface->bInterfaceSubClass== USB_SUBCLASS_HUB)
    return true;
  return false; 
}
bool WejUSBMIDI::isMidiInterface( usb_interfaceDescriptor * interface)
{
  if (interface->bInterfaceSubClass==USB_SUBCLASS_MIDISTREAMING && interface->bInterfaceClass==USB_CLASS_AUDIO)
    return true;
  else
    return false;
}

#define CS_ENDPOINT         0x25
#define MS_GENERAL          0x01

bool WejUSBMIDI::isMidiDevice( usb_device * device, int configuration,  midi_usbConfiguration * handle)
{
  bool ret = false;
  uint8_t buf[MAX_BUF_SIZE];
  int bytesRead;

  // Read the length of the configuration descriptor.
  bytesRead = USB::getConfigurationDescriptor(device, configuration, MAX_BUF_SIZE, buf);
  if (bytesRead<0) return false;

  int pos = 0;
  uint8_t descriptorLength;
  uint8_t descriptorType;

  usb_configurationDescriptor * config = NULL;
  usb_interfaceDescriptor * interface = NULL;
  usb_endpointDescriptor * endpoint = NULL;

  while (pos < bytesRead)
  {
    descriptorLength = buf[pos];
    descriptorType = buf[pos + 1];
    switch (descriptorType)
    {
    case (USB_DESCRIPTOR_CONFIGURATION):
      config = (usb_configurationDescriptor *)(buf + pos);
      break;
    case (USB_DESCRIPTOR_INTERFACE):
      interface = (usb_interfaceDescriptor *)(buf + pos);
      if (isMidiInterface(interface))
      {
        // handle->address = address;
        handle->configuration = config->bConfigurationValue;
        handle->interface = interface->bInterfaceNumber;
        ret = true;
      }
      break;
    case (USB_DESCRIPTOR_ENDPOINT):
      endpoint = (usb_endpointDescriptor *)(buf + pos);
      // If this endpoint descriptor is found right after the midi interface descriptor, it belong to that interface.
      if (interface->bInterfaceNumber == handle->interface)
      {
        if (endpoint->bEndpointAddress & 0x80)
        {
          handle->inputEndPointAddress = endpoint->bEndpointAddress & ~0x80;
        }
        else
        {
          handle->outputEndPointAddress = endpoint->bEndpointAddress;
        }
      }
      break;
   case (CS_ENDPOINT):
   {
      if (buf[pos+2] == MS_GENERAL && device->port<=2)
      {
        uint8_t num = buf[pos+3];
        if (num<1)
          num=1;
        mNumInputCables[device->port-1] = num;
      }
   }
   break;
   default:
   break;
    }

    pos += descriptorLength;
  }

  return ret;

}
bool WejUSBMIDI::isHubDevice( usb_device * device, int configuration,  hub_usbConfiguration * handle)
{
  bool ret = false;
  uint8_t buf[MAX_BUF_SIZE];
  int bytesRead;

  // Read the length of the configuration descriptor.
  bytesRead = USB::getConfigurationDescriptor(device, configuration, MAX_BUF_SIZE, buf);
  if (bytesRead<0) return false;

  int pos = 0;
  uint8_t descriptorLength;
  uint8_t descriptorType;

  usb_configurationDescriptor * config = NULL;
  usb_interfaceDescriptor * interface = NULL;
  usb_endpointDescriptor * endpoint = NULL;

  while (pos < bytesRead)
  {
    descriptorLength = buf[pos];
    descriptorType = buf[pos + 1];

    switch (descriptorType)
    {
    case (USB_DESCRIPTOR_CONFIGURATION):
      config = (usb_configurationDescriptor *)(buf + pos);
      break;
    case (USB_DESCRIPTOR_INTERFACE):
      interface = (usb_interfaceDescriptor *)(buf + pos);
      if (isHubInterface(interface))
      {
        handle->configuration = config->bConfigurationValue;
        handle->interface = interface->bInterfaceNumber;
        ret = true;
      }
      break;
    case (USB_DESCRIPTOR_ENDPOINT):
      endpoint = (usb_endpointDescriptor *)(buf + pos);
      if (interface->bInterfaceNumber == handle->interface)
      {
        if (endpoint->bEndpointAddress & 0x80)
        {
          handle->inputEndPointAddress = endpoint->bEndpointAddress & ~0x80;
        }
      }

      break;
    default:
      break;
    }

    pos += descriptorLength;
  }

  return ret;

}


void WejUSBMIDI::initUsbMidi( usb_device * device,  midi_usbConfiguration * handle)
{
  // Initialise/configure the USB device.
  // TODO write a usb_initBulkDevice function?
  USB::initDevice(device, handle->configuration);

  // Initialise bulk input endpoint.
  USB::initEndPoint(&(device->bulk_in), handle->inputEndPointAddress);
  device->bulk_in.attributes = USB_TRANSFER_TYPE_BULK;
  device->bulk_in.maxPacketSize = 64;

  // Initialise bulk output endpoint.
  USB::initEndPoint(&(device->bulk_out), handle->outputEndPointAddress);
  device->bulk_out.attributes = USB_TRANSFER_TYPE_BULK;
  device->bulk_out.maxPacketSize = 64;

  // Success, signal that we are now connected.
  midi[device->port-1] = device;
}
void WejUSBMIDI::initUsbHub( usb_device * device,  hub_usbConfiguration * handle)
{
  // Initialise/configure the USB device.
  // TODO write a usb_initBulkDevice function?
  USB::initDevice(device, handle->configuration);

  // Initialise bulk input endpoint.
  USB::initEndPoint(&(device->bulk_in), handle->inputEndPointAddress);
  device->bulk_in.attributes = USB_TRANSFER_TYPE_INTERRUPT;
  device->bulk_in.maxPacketSize = 64;
  usb_hubDescriptor desc;
  USB::getHubDescriptor(device,&desc);

  for (int i=0;i<desc.bNbrPorts;i++)
  {
    USB::setPortFeature(device,HUB_FEATURE_PORT_POWER,i+1,0);    
    //USB::resetHubPort(device,i+1);
  }
  // Success, signal that we are now connected.
  hub = device;
}

void WejUSBMIDI::usbEventHandler( usb_device * device, usb_eventType event)
{
  midi_usbConfiguration midi_handle;
  hub_usbConfiguration hub_handle;
  usb_deviceDescriptor descriptor;
  switch (event)
  {
     case USB_CONNECT:
     {
        if (WejUSBMIDI::sWej->isMidiDevice(device,0,&midi_handle))
        {
          WejUSBMIDI::sWej->initUsbMidi(device,&midi_handle);
        }
        else if (WejUSBMIDI::sWej->isHubDevice(device,0,&hub_handle))
        {
          WejUSBMIDI::sWej->initUsbHub(device,&hub_handle);
        }
     }
     break;
     case USB_DISCONNECT:
     {
     }
     break;
  }
}


