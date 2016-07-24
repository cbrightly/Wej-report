#include "MidiMessage.h"
MidiMessage::MidiMessage()
{
  size = 0;
}
MidiMessage::MidiMessage(uint8_t s,uint8_t b1, uint8_t b2)
{
  data[0] = s;
  data[1] = b1;
  data[2] = b2;
  size = 3;
}
void
MidiMessage::appendByte(uint8_t b)
{
  if (size==MAX_MIDI_MESSAGE)
     size = 255;
  else
     data[size++] = b;
}
void
MidiMessage::clear()
{
  size = 0;
}
bool
MidiMessage::isValidSysEx()
{
  if (size==255)
    return false;
  return true;
}
bool
MidiMessage::isSysEx()
{
  if (data[0]==0xF0)
      return true;
  else
      return false;
}
bool 
MidiMessage::isController()
{
  if ((data[0] & 0xf0) == 0xb0)
     return true;
  else
     return false;
}

bool
MidiMessage::isNoteOn()
{
  if ((data[0] & 0xf0) == 0x90)
    return true;
  else
    return false;
}

bool
MidiMessage::isNoteOff()
{
  if ((data[0] & 0xf0) == 0x80)
    return true;
  else
    return false;
}

uint8_t
MidiMessage::channel()
{
  return 1;
}

uint8_t
MidiMessage::controlNumber()
{
  return data[1];
}
uint8_t
MidiMessage::controlValue()
{
  return data[2];
}
uint8_t
MidiMessage::noteNumber()
{
  return data[1];
}

uint8_t
MidiMessage::velocity()
{
  return data[2];
}
