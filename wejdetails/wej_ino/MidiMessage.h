#pragma once
typedef unsigned char uint8_t;
#define MAX_MIDI_MESSAGE 32
class MidiMessage
{
  public:
    MidiMessage();
    MidiMessage(uint8_t s,uint8_t b1, uint8_t b2);
    uint8_t data[MAX_MIDI_MESSAGE];
    uint8_t size;
    void   appendByte(uint8_t byte);
    void   clear();
    bool   isValidSysEx();
    bool   isController();
    bool   isNoteOn();
    bool   isNoteOff();
    bool   isSysEx();
    uint8_t channel();
    uint8_t controlNumber();
    uint8_t controlValue();
    uint8_t noteNumber();
    uint8_t velocity();
    
};
