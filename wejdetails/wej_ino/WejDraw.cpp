#include "WejDraw.h"
static byte sMods[3];
WejDraw::WejDraw(int num_leds)
{
  mBeatTime = 0;
  mNumLeds = num_leds;
  for (int i=0;i<3;i++)
  {
    sMods[i] = 0;
  }
  mDrawByte = 0;
  mDrawByteOn = 0;
}

void WejDraw::hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v)
{
    unsigned char region, fpart, p, q, t;
    
    if(s == 0) {
        /* color is grayscale */
        *r = *g = *b = v;
        return;
    }
    
    /* make hue 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;
    
    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;
        
    /* assign temp vars based on color cone region */
    switch(region) {
        case 0:
            *r = v; *g = t; *b = p; break;
        case 1:
            *r = q; *g = v; *b = p; break;
        case 2:
            *r = p; *g = v; *b = t; break;
        case 3:
            *r = p; *g = q; *b = v; break;
        case 4:
            *r = t; *g = p; *b = v; break;
        default:
            *r = v; *g = p; *b = q; break;
    }
    
    return;
}

WejDraw::~WejDraw()
{
}
void
WejDraw::beat(Adafruit_DotStar *strip)
{
  mBeatTime = millis();
}
void
WejDraw::drawByte(byte b0,byte b1,byte b2,bool on)
{
  mDrawByteOn = on;
  unsigned long l1 = b1;
  unsigned long l2 = b2;
  mDrawByte = 0;
  mDrawByte = b0;
  mDrawByte = mDrawByte | (l1<<8);
  mDrawByte = mDrawByte | (l2<<16);

}
void
WejDraw::setMod(byte m,byte v)
{
  sMods[m] = v;
}

void
WejDraw::draw(Adafruit_DotStar *strip, int start_led,int num_led,unsigned long frame_time)
{
  if (mDrawByteOn)
  {
    for (int i=start_led;i<start_led+num_led;i++)
    {
      if (i<24)
      {
        if (mDrawByte & (1ul<<i))
            strip->setPixelColor(i,0x111111);
        else
            strip->setPixelColor(i,0);
      }
      else
          strip->setPixelColor(i,0x001100);
    }
  }
}

const byte sideWipe[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
                                 23,22,21,20,19,18,17,16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
                                        
const byte centerWipe[] PROGMEM =     {12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,
                                       12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12};


const byte     blocks[] PROGMEM =     { 0, 0,10, 10, 8, 8, 2, 2, 4, 4, 0, 1, 1, 2, 2,10,10, 6, 6, 8, 8,10,10,10,
                                       10,10,10,  8, 8, 6, 6,10,10, 2, 2, 1, 1, 0, 4, 4, 2, 2, 8, 8,10,10, 0, 0};

const byte threeblocks[] PROGMEM = {12,12,12,12,12,12,12,12,8,8,8,8,8,8,8,8,3,3,3,3,3,3,3,3,
                                    3,3,3,3,3,3,3,3,8,8,8,8,8,8,8,8,12,12,12,12,12,12,12,12};


const byte strobe[] PROGMEM = {255,255,0,0,0,0};                                       
const byte ramp[] PROGMEM = { 254 , 253 , 252 , 251 , 250 , 249 , 248 , 247 , 246 , 245 , 244 , 243 , 242 , 241 , 240 , 239 , 238 , 237 , 236 , 235 , 234 , 233 , 232 , 231 , 230 , 229 , 228 , 227 , 226 , 225 , 224 , 223 , 222 , 221 , 220 , 219 , 218 , 217 , 216 , 215 , 214 , 213 , 212 , 211 , 210 , 209 , 208 , 207 , 206 , 205 , 204 , 203 , 202 , 201 , 200 , 199 , 198 , 197 , 196 , 195 , 194 , 193 , 192 , 191 , 190 , 189 , 188 , 187 , 186 , 185 , 184 , 183 , 182 , 181 , 180 , 179 , 178 , 177 , 176 , 175 , 174 , 173 , 172 , 171 , 170 , 169 , 168 , 167 , 166 , 165 , 164 , 163 , 162 , 161 , 160 , 159 , 158 , 157 , 156 , 155 , 154 , 153 , 152 , 151 , 150 , 149 , 148 , 147 , 146 , 145 , 144 , 143 , 142 , 141 , 140 , 139 , 138 , 137 , 136 , 135 , 134 , 133 , 132 , 131 , 130 , 129 , 128 , 127 , 126 , 125 , 124 , 123 , 122 , 121 , 120 , 119 , 118 , 117 , 116 , 115 , 114 , 113 , 112 , 111 , 110 , 109 , 108 , 107 , 106 , 105 , 104 , 103 , 102 , 101 , 100 , 99 , 98 , 97 , 96 , 95 , 94 , 93 , 92 , 91 , 90 , 89 , 88 , 87 , 86 , 85 , 84 , 83 , 82 , 81 , 80 , 79 , 78 , 77 , 76 , 75 , 74 , 73 , 72 , 71 , 70 , 69 , 68 , 67 , 66 , 65 , 64 , 63 , 62 , 61 , 60 , 59 , 58 , 57 , 56 , 55 , 54 , 53 , 52 , 51 , 50 , 49 , 48 , 47 , 46 , 45 , 44 , 43 , 42 , 41 , 40 , 39 , 38 , 37 , 36 , 35 , 34 , 33 , 32 , 31 , 30 , 29 , 28 , 27 , 26 , 25 , 24 , 23 , 22 , 21 , 20 , 19 , 18 , 17 , 16 , 15 , 14 , 13 , 12 , 11 , 10 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1 , 0};
const byte sinTable[] PROGMEM = { 128 , 134 , 141 , 147 , 153 , 159 , 165 , 171 , 177 , 183 , 188 , 194 , 199 , 204 , 209 , 214 , 219 , 223 , 227 , 231 , 234 , 238 , 241 , 244 , 246 , 249 , 250 , 252 , 254 , 255 , 255 , 256 , 256 , 256 , 255 , 255 , 254 , 252 , 250 , 249 , 246 , 244 , 241 , 238 , 234 , 231 , 227 , 223 , 219 , 214 , 209 , 204 , 199 , 194 , 188 , 183 , 177 , 171 , 165 , 159 , 153 , 147 , 141 , 134 , 128 , 122 , 115 , 109 , 103 , 97 , 91 , 85 , 79 , 73 , 68 , 62 , 57 , 52 , 47 , 42 , 37 , 33 , 29 , 25 , 22 , 18 , 15 , 12 , 10 , 7 , 6 , 4 , 2 , 1 , 1 , 0 , 0 , 0 , 1 , 1 , 2 , 4 , 6 , 7 , 10 , 12 , 15 , 18 , 22 , 25 , 29 , 33 , 37 , 42 , 47 , 52 , 57 , 62 , 68 , 73 , 79 , 85 , 91 , 97 , 103 , 109 , 115 , 122 };
const byte noise[] PROGMEM = { 125 , 59 , 27 , 70 , 169 , 76 , 107 , 136 , 236 , 39 , 250 , 228 , 185 , 227 , 41 , 203 , 21 , 235 , 52 , 143 , 122 , 174 , 124 , 134 , 216 , 162 , 116 , 218 , 14 , 36 , 0 , 131 , 78 , 253 , 68 , 65 , 32 , 254 , 107 , 76 , 27 , 135 , 220 , 203 , 71 , 82 , 147 , 44 , 16 , 150 , 232 , 239 , 246 , 203 , 113 , 0 , 178 , 96 , 167 , 26 , 174 , 30 , 132 , 215 , 102 , 90 , 65 , 211 , 135 , 209 , 214 , 238 , 106 , 86 , 98 , 116 , 203 , 142 , 169 , 20 , 102 , 87 , 42 , 99 , 198 , 26 , 46 , 127 , 98 , 239 , 190 , 116 , 165 , 102 , 212 , 79 , 26 , 228 , 65 , 251 , 41 , 35 , 208 , 18 , 126 , 127 , 109 , 48 , 100 , 69 , 231 , 181 , 11 , 95 , 204 , 78 , 4 , 221 , 50 , 44 , 68 , 116 , 60 , 73 , 252 , 6 , 39 , 23 , };
const byte exponet[] PROGMEM = { 255 , 244 , 234 , 224 , 214 , 205 , 196 , 188 , 180 , 173 , 165 , 158 , 152 , 145 , 139 , 133 , 127 , 122 , 117 , 112 , 107 , 103 , 98 , 94 , 90 , 86 , 83 , 79 , 76 , 73 , 70 , 67 , 64 , 61 , 58 , 56 , 54 , 51 , 49 , 47 , 45 , 43 , 41 , 40 , 38 , 36 , 35 , 33 , 32 , 31 , 29 , 28 , 27 , 26 , 25 , 24 , 23 , 22 , 21 , 20 , 19 , 18 , 17 , 17 , 16 , 15 , 15 , 14 , 13 , 13 , 12 , 12 , 11 , 11 , 10 , 10 , 9 , 9 , 9 , 8 , 8 , 8 , 7 , 7 , 7 , 6 , 6 , 6 , 6 , 5 , 5 , 5 , 5 , 5 , 4 , 4 , 4 , 4 , 4 , 4 , 3 , 3 , 3 , 3 , 3 , 3 , 3 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 1 , 1 , 1 , 1 , 1 , 1, 1 , 0 };

const uint32_t rainbow[] PROGMEM = { 0xff0000 , 0xff0c00 , 0xff1800 , 0xff2400 , 0xff3000 , 0xff3c00 , 0xff4800 , 0xff5400 , 0xff6000 , 0xff6c00 , 0xff7800 , 0xff8300 , 0xff8f00 , 0xff9b00 , 0xffa700 , 0xffb300 , 0xffbf00 , 0xffcb00 , 0xffd700 , 0xffe300 , 0xffef00 , 0xfffb00 , 0xf7ff00 , 0xebff00 , 0xdfff00 , 0xd3ff00 , 0xc7ff00 , 0xbbff00 , 0xafff00 , 0xa3ff00 , 0x97ff00 , 0x8bff00 , 0x80ff00 , 0x74ff00 , 0x68ff00 , 0x5cff00 , 0x50ff00 , 0x44ff00 , 0x38ff00 , 0x2cff00 , 0x20ff00 , 0x14ff00 , 0x08ff00 , 0x00ff04 , 0x00ff10 , 0x00ff1c , 0x00ff28 , 0x00ff34 , 0x00ff40 , 0x00ff4c , 0x00ff58 , 0x00ff64 , 0x00ff70 , 0x00ff7c , 0x00ff87 , 0x00ff93 , 0x00ff9f , 0x00ffab , 0x00ffb7 , 0x00ffc3 , 0x00ffcf , 0x00ffdb , 0x00ffe7 , 0x00fff3 , 0x00ffff , 0x00f3ff , 0x00e7ff , 0x00dbff , 0x00cfff , 0x00c3ff , 0x00b7ff , 0x00abff , 0x009fff , 0x0093ff , 0x0087ff , 0x007cff , 0x0070ff , 0x0064ff , 
                                      0x0058ff , 0x004cff , 0x0040ff , 0x0034ff , 0x0028ff , 0x001cff , 0x0010ff , 0x0004ff , 0x0800ff , 0x1400ff , 0x2000ff , 0x2c00ff , 0x3800ff , 0x4400ff , 0x5000ff , 0x5c00ff , 0x6800ff , 0x7400ff , 0x8000ff , 0x8b00ff , 0x9700ff , 0xa300ff , 0xaf00ff , 0xbb00ff , 0xc700ff , 0xd300ff , 0xdf00ff , 0xeb00ff , 0xf700ff , 0xff00fb , 0xff00ef , 0xff00e3 , 0xff00d7 , 0xff00cb , 0xff00bf , 0xff00b3 , 0xff00a7 , 0xff009b , 0xff008f , 0xff0083 , 0xff0078 , 0xff006c , 0xff0060 , 0xff0054 , 0xff0048 , 0xff003c , 0xff0030 , 0xff0024 , 0xff0018 , 0xff000c};
const uint32_t fire[] PROGMEM  = { 0xFFC300 , 0xFFC600 , 0xFFC900 , 0xFFCD00 , 0xFFD200 , 0xFFD700 , 0xFFDC00 , 0xFFE200 , 0xFFE700 , 0xFFEC00 , 0xFFF100 , 0xFFF600 , 0xFFFA00 , 0xFFFD00 , 0xFFFF00 , 0xFFFF00 , 0xFFFF00 , 0xFFFF00 , 0xFFF900 , 0xFFEF00 , 0xFFE000 , 0xFFD000 , 0xFFBE00 , 0xFFAB00 , 0xFF9900 , 0xFF8900 , 0xFF7C00 , 0xFF7600 , 0xFF7300 , 0xFF6F00 , 0xFF6C00 , 0xFF6800 , 0xFF6400 , 0xFF6000 , 0xFF5D00 , 0xFF5900 , 0xFF5500 , 0xFF5100 , 0xFF4E00 , 0xFF4A00 , 0xFF4700 , 0xFF4300 , 0xFF4000 , 0xFF3E00 , 0xFF3B00 , 0xFF3900 , 0xFF3600 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3500 , 0xFF3700 , 0xFF4000 , 0xFF4D00 , 0xFF5E00 , 0xFF7200 , 0xFF8700 , 0xFF9D00 , 0xFFB300 , 0xFFC800 , 0xFFDB00 , 0xFFEB00 , 0xFFF600 , 0xFFFD00 , 0xFFFC0B , 0xFFE625 , 0xFFC345 , 0xFD9A67 , 0xF37583 , 0xE96392 , 0xE45F96 , 0xDE5C9A , 0xD8589E , 0xD154A2 , 0xCA51A5 , 0xC14DA9 , 0xB84AAD , 0xAF47B0 , 0xA644B3 , 0x9D41B7 , 0x933EBA , 0x8A3CBD , 0x8039BF , 0x7737C1 , 0x6E35C2 , 0x6633C2 , 0x5E31C2 , 0x562FC2 , 0x4F2DC2 , 0x482BC2 , 0x432AC2 , 0x3E28C2 , 0x3B27C2 , 0x3826C2 , 0x3725C2 , 0x3624C2 , 0x3824BF , 0x3A24BA , 0x3E24B5 , 0x4224B0 , 0x4824A9 , 0x4E24A3 , 0x56249B , 0x5E2493 , 0x66248B , 0x6F2582 , 0x792679 , 0x832770 , 0x8D2866 , 0x97295D , 0xA22A54 , 0xAC2C4B , 0xB62D42 , 0xC02E39 , 0xCA2F30 , 0xD33028 , 0xDC3120 , 0xE43219 , 0xEB3312 , 0xF2340C , 0xF83407 , 0xFD3502 };
const uint32_t glacier[] PROGMEM  = { 0x80E8ED , 0x75E0ED , 0x69D6ED , 0x5ACAE7 , 0x42B4CC , 0x279AA9 , 0x0F8387 , 0x017570 , 0x017469 , 0x04766B , 0x0A7B6E , 0x118072 , 0x198878 , 0x22907F , 0x2C9887 , 0x38A290 , 0x43AC99 , 0x4FB6A2 , 0x5BC0AC , 0x67CAB6 , 0x72D4C0 , 0x7DDDC9 , 0x87E6D2 , 0x90EEDA , 0x97F5E1 , 0x9EFAE7 , 0xA2FEEC , 0xA3FFED , 0xA3FFED , 0xA0FFED , 0x99FFED , 0x90FFED , 0x86FBED , 0x7BF4ED , 0x72ECED , 0x6AE2ED , 0x63D7ED , 0x61CCED , 0x61BFEE , 0x61AFF0 , 0x629DF2 , 0x6589F3 , 0x6775F5 , 0x6A60F7 , 0x6E4CF9 , 0x7038FB , 0x7226FD , 0x7217FE , 0x720CFF , 0x7204FF , 0x7100FF , 0x6A07FD , 0x5E19FA , 0x5033F6 , 0x4052F2 , 0x3171ED , 0x238EE9 , 0x19A4E5 , 0x18A7E5 , 0x2291E8 , 0x2F75EC , 0x3E56F1 , 0x4E37F6 , 0x5C1CFA , 0x6808FD , 0x7100FF , 0x7203FF , 0x720AFF , 0x7216FE , 0x7224FD , 0x7136FB , 0x6E49F9 , 0x6B5DF8 , 0x6872F6 , 0x6587F4 , 0x639BF2 , 0x61ADF0 , 0x61BDEF , 0x61CAED , 0x63D6ED , 0x69E1ED , 0x71EBED , 0x7AF3ED , 0x85FAED , 0x8FFFED , 0x98FFED , 0xA0FFED , 0xA3FFED , 0xA3FFED , 0xA2FEEC , 0x9EFAE7 , 0x98F5E2 , 0x91EFDB , 0x88E8D3 , 0x7FDFCA , 0x74D5C1 , 0x68CCB7 , 0x5DC1AE , 0x51B7A4 , 0x45AD9A , 0x39A391 , 0x2E9988 , 0x239180 , 0x1A8979 , 0x128173 , 0x0A7B6E , 0x05776B , 0x017469 , 0x01746E , 0x0D8184 , 0x2497A5 , 0x3FB1C8 , 0x58C8E4 , 0x68D5ED , 0x73DFED , 0x7FE7ED , 0x89EEED , 0x93F5ED , 0x9BFAED , 0xA1FEED , 0xA3FFED , 0xA3FFED , 0xA3FFED , 0xA3FFED , 0xA3FFED , 0xA3FFED , 0xA3FFED , 0xA3FFED };
const uint32_t gray[] PROGMEM  = { 0x010101 , 0x020202 , 0x040404 , 0x060606 , 0x080808 , 0x0A0A0A , 0x0C0C0C , 0x0E0E0E , 0x101010 , 0x121212 , 0x151515 , 0x171717 , 0x1A1A1A , 0x1D1D1D , 0x1F1F1F , 0x222222 , 0x252525 , 0x282828 , 0x2B2B2B , 0x2E2E2E , 0x313131 , 0x343434 , 0x373737 , 0x3A3A3A , 0x3E3E3E , 0x414141 , 0x444444 , 0x484848 , 0x4B4B4B , 0x4F4F4F , 0x525252 , 0x565656 , 0x595959 , 0x5D5D5D , 0x616161 , 0x646464 , 0x686868 , 0x6C6C6C , 0x6F6F6F , 0x737373 , 0x777777 , 0x7A7A7A , 0x7E7E7E , 0x848484 , 0x8C8C8C , 0x939393 , 0x9A9A9A , 0xA2A2A2 , 0xA9A9A9 , 0xB0B0B0 , 0xB7B7B7 , 0xBEBEBE , 0xC5C5C5 , 0xCBCBCB , 0xD2D2D2 , 0xD8D8D8 , 0xDDDDDD , 0xE3E3E3 , 0xE8E8E8 , 0xEDEDED , 0xF2F2F2 , 0xF6F6F6 , 0xFAFAFA , 0xFDFDFD , 0xFEFEFE , 0xFAFAFA , 0xF7F7F7 , 0xF2F2F2 , 0xEEEEEE , 0xE9E9E9 , 0xE4E4E4 , 0xDEDEDE , 0xD8D8D8 , 0xD2D2D2 , 0xCCCCCC , 0xC6C6C6 , 0xBFBFBF , 0xB8B8B8 , 0xB1B1B1 , 0xAAAAAA , 0xA3A3A3 , 0x9B9B9B , 0x949494 , 0x8C8C8D , 0x858585 , 0x7F7F7F , 0x7B7B7B , 0x777777 , 0x737373 , 0x707070 , 0x6C6C6C , 0x686868 , 0x656565 , 0x616161 , 0x5D5D5D , 0x5A5A5A , 0x565656 , 0x535353 , 0x4F4F4F , 0x4C4C4C , 0x484848 , 0x454545 , 0x414141 , 0x3E3E3E , 0x3B3B3B , 0x373737 , 0x343434 , 0x313131 , 0x2E2E2E , 0x2B2B2B , 0x282828 , 0x252525 , 0x222222 , 0x202020 , 0x1D1D1D , 0x1A1A1A , 0x181818 , 0x151515 , 0x131313 , 0x101010 , 0x0E0E0E , 0x0C0C0C , 0x0A0A0A , 0x080808 , 0x060606 , 0x040404 , 0x030303 , 0x010101 };
const uint32_t nebula[] PROGMEM  = { 0xF92823 , 0xF52723 , 0xF12523 , 0xEC2423 , 0xE72223 , 0xE12023 , 0xDB1E24 , 0xD41C24 , 0xCE1A24 , 0xC81824 , 0xC21625 , 0xBC1425 , 0xB71226 , 0xB21027 , 0xAE0F29 , 0xAA0D2A , 0xA70C2C , 0xA40B2D , 0xA1092F , 0x9F0831 , 0x9C0733 , 0x9A0636 , 0x980538 , 0x96043B , 0x94033D , 0x92023F , 0x900142 , 0x8E0044 , 0x8B0047 , 0x890049 , 0x87004B , 0x85004C , 0x84004E , 0x82004F , 0x800051 , 0x7E0053 , 0x7C0056 , 0x790059 , 0x76005D , 0x720062 , 0x6E0068 , 0x69056E , 0x631176 , 0x5C1F7F , 0x543188 , 0x4B4593 , 0x425A9E , 0x3870AA , 0x2F86B5 , 0x269CC1 , 0x1DB1CC , 0x15C4D7 , 0x0ED5E2 , 0x08E3EB , 0x04EDF3 , 0x01F4FA , 0x01F4FF , 0x0CCFFF , 0x2585FF , 0x4536FF , 0x6305FF , 0x6B00FF , 0x6F00FF , 0x7500FF , 0x7A00FF , 0x7F00FF , 0x8500FF , 0x8B00FF , 0x9100FF , 0x9700FF , 0x9D00FF , 0xA400FF , 0xAA00FF , 0xB000FF , 0xB600FF , 0xBE00FF , 0xC700FF , 0xD000FF , 0xD900FF , 0xE100FF , 0xE800FF , 0xEF00FF , 0xF500FF , 0xFA00FF , 0xFF00FF , 0xFF00FA , 0xFF00F4 , 0xFF00ED , 0xFF00E5 , 0xFF00DB , 0xFF00D2 , 0xFF00C8 , 0xFF00BD , 0xFF00B3 , 0xFF00A9 , 0xFF00A0 , 0xFF0097 , 0xFF008F , 0xFF0087 , 0xFF0081 , 0xFD017C , 0xF90277 , 0xF40374 , 0xEF0572 , 0xE80770 , 0xE2086F , 0xDB0A6E , 0xD50C6E , 0xCF0E6D , 0xC9106C , 0xC5126B , 0xC1146A , 0xBF1668 , 0xBE1765 , 0xBF1961 , 0xC11A5D , 0xC41C59 , 0xC91D54 , 0xCD1E4F , 0xD3204A , 0xD92144 , 0xDF233F , 0xE5243A , 0xEB2535 , 0xF12630 , 0xF6272C , 0xFA2828 , 0xFE2926 };
const uint32_t northernlights[] PROGMEM  = { 0xC600FF , 0xC600FF , 0xC600FF , 0xC600FF , 0xC600FF , 0xC600FF , 0xC502FE , 0xBC1AEC , 0xAE41CF , 0x9D6FAC , 0x8B9E88 , 0x7BC667 , 0x6EE04E , 0x65E43D , 0x5EE332 , 0x5AD92B , 0x56C726 , 0x53B424 , 0x50A223 , 0x4C9622 , 0x4A8D23 , 0x478526 , 0x457C2A , 0x43742F , 0x416B34 , 0x3F6338 , 0x3D5B3C , 0x3B533F , 0x394D3F , 0x36473E , 0x34413D , 0x313B3B , 0x2E3439 , 0x2B2E36 , 0x282733 , 0x262130 , 0x231C2D , 0x21192A , 0x1F1927 , 0x1F1925 , 0x1F1924 , 0x1F1923 , 0x201C23 , 0x272F26 , 0x334E2C , 0x407232 , 0x4F983A , 0x5CBB40 , 0x66D546 , 0x6BE349 , 0x68DD47 , 0x5FC240 , 0x509A36 , 0x3F6D2C , 0x2F4429 , 0x212729 , 0x1A1E2A , 0x1A202D , 0x1A2432 , 0x1A2938 , 0x1A3040 , 0x1A3848 , 0x1A4150 , 0x1A4B59 , 0x1A5663 , 0x1A626D , 0x1A6E77 , 0x1B7B81 , 0x1D888C , 0x1E9596 , 0x1FA2A1 , 0x20AFAB , 0x22BBB4 , 0x23C7BD , 0x24D2C6 , 0x26DCCE , 0x27E6D5 , 0x28EEDB , 0x29F4E0 , 0x2AF9E4 , 0x2BFDE7 , 0x2CFFE9 , 0x2DFEE8 , 0x2DF7E3 , 0x2EECDA , 0x2EDDCE , 0x2ECABF , 0x2EB5AF , 0x2E9E9C , 0x2F878A , 0x2F7077 , 0x305965 , 0x314554 , 0x323445 , 0x332639 , 0x351D31 , 0x3C1E2E , 0x524D45 , 0x6B9871 , 0x79C087 , 0x7CC78B , 0x7FCD8E , 0x81D390 , 0x83D892 , 0x85DC94 , 0x87E096 , 0x89E397 , 0x8AE698 , 0x8BE899 , 0x8CEB9A , 0x8DED9A , 0x8EEF9A , 0x8FF19A , 0x8FF39A , 0x8FF39A , 0x8FF399 , 0x8EF398 , 0x8EF396 , 0x8DF394 , 0x8BF392 , 0x8AF38F , 0x89F38D , 0x87F38A , 0x86F388 , 0x84F386 , 0x83F384 , 0x82F382 , 0x81F381 };
const uint32_t ocean[] PROGMEM  = { 0x00595B , 0x005759 , 0x005557 , 0x005255 , 0x004E52 , 0x004A4F , 0x00464C , 0x004349 , 0x003F47 , 0x003C44 , 0x003942 , 0x003841 , 0x003840 , 0x003840 , 0x003940 , 0x003C42 , 0x004045 , 0x004548 , 0x004B4C , 0x005250 , 0x005954 , 0x006159 , 0x00695D , 0x007062 , 0x007766 , 0x007E6A , 0x00846E , 0x008970 , 0x008D73 , 0x009074 , 0x009374 , 0x009574 , 0x009673 , 0x009772 , 0x009872 , 0x009972 , 0x009A72 , 0x009B72 , 0x009C72 , 0x009E72 , 0x00A072 , 0x00A373 , 0x04A777 , 0x0DAC7C , 0x19B284 , 0x27B88C , 0x38C094 , 0x49C79E , 0x5BCFA8 , 0x6ED7B2 , 0x80DFBD , 0x91E6C7 , 0xA1EDD0 , 0xB0F3D9 , 0xBCF8E1 , 0xC5FCE8 , 0xCAFFEE , 0xCAFFF2 , 0xCAFFF6 , 0xCAFFF9 , 0xC7FFFB , 0xC2FFFD , 0xBBFDFF , 0xB4FBFF , 0xABF8FF , 0xA2F5FF , 0x9AF3FF , 0x92F0FF , 0x8AEEFF , 0x83ECFF , 0x7EEBFF , 0x7AEBFE , 0x76EBFC , 0x72EBF9 , 0x6EEBF6 , 0x6AEBF3 , 0x67EBF0 , 0x63EBEC , 0x60EBE8 , 0x5DEBE4 , 0x59EBE1 , 0x56EBDE , 0x53EBDB , 0x50EBD9 , 0x4CE8D8 , 0x49E2D8 , 0x45D9D8 , 0x41D0D8 , 0x3DC4D8 , 0x38B8D9 , 0x34ABDA , 0x2F9EDB , 0x2B91DD , 0x2783DE , 0x2476E0 , 0x216AE1 , 0x1F5FE3 , 0x1D55E5 , 0x1C4DE6 , 0x1C46E8 , 0x1D44E9 , 0x2044EB , 0x2544EE , 0x2A44F2 , 0x3144F6 , 0x3944FA , 0x4145FD , 0x4949FF , 0x504DFF , 0x5751FF , 0x5D55FF , 0x6258FF , 0x655BFF , 0x665DFF , 0x655EFD , 0x635EF5 , 0x5E5EEC , 0x575EE1 , 0x505ED4 , 0x475EC6 , 0x3D5EB7 , 0x345DA8 , 0x2A5D9A , 0x205C8B , 0x175C7E , 0x0F5B72 , 0x085A67 , 0x035A5F };
const uint32_t *colorMaps[] = {rainbow,fire,glacier,gray,nebula,northernlights,ocean};
Exp::Exp(int num_leds,int variation) : WejDraw(num_leds)
{
  setVariation(variation);
}
void
Exp::setVariation(int variation)
{
  mVariation = variation;
  if (variation==0)
  {
    mSpatialMap = (byte*)blocks;
    mSpatialMapSize = sizeof(blocks);
    mColorIndex = (byte *)exponet;
    mColorIndexSize = sizeof(exponet);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)sinTable;
    mBrightMapSize = sizeof(sinTable);
  }
  else if (variation==1)
  {
    mSpatialMap = (byte*)noise;
    mSpatialMapSize = sizeof(noise);
    mColorIndex = (byte *)sinTable;
    mColorIndexSize = sizeof(sinTable);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)sinTable;
    mBrightMapSize = sizeof(sinTable);
  }
  else if (variation==2)
  {
    mSpatialMap = (byte*)centerWipe;
    mSpatialMapSize = sizeof(centerWipe);
    mColorIndex = (byte *)exponet;
    mColorIndexSize = sizeof(exponet);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)sinTable;
    mBrightMapSize = sizeof(sinTable);
  }
  else if (variation==3)
  {
    mSpatialMap = (byte*)sideWipe;
    mSpatialMapSize = sizeof(sideWipe);
    mColorIndex = (byte *)sinTable;
    mColorIndexSize = sizeof(sinTable);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)exponet;
    mBrightMapSize = sizeof(exponet);
  }
  else if (variation==4)
  {
    mSpatialMap = (byte*)sideWipe;
    mSpatialMapSize = sizeof(sideWipe);
    mColorIndex = (byte *)exponet;
    mColorIndexSize = sizeof(exponet);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)noise;
    mBrightMapSize = sizeof(noise);
  }
  else if (variation==5)
  {
    mSpatialMap = (byte*)blocks;
    mSpatialMapSize = sizeof(blocks);
    mColorIndex = (byte *)exponet;
    mColorIndexSize = sizeof(exponet);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)noise;
    mBrightMapSize = sizeof(noise);
  }
  else if (variation==6)
  {
    mSpatialMap = (byte*)threeblocks;
    mSpatialMapSize = sizeof(threeblocks);
    mColorIndex = (byte *)sinTable;
    mColorIndexSize = sizeof(sinTable);
    mColorMap = (uint32_t *)rainbow;
    mColorMapSize = sizeof(rainbow)/sizeof(uint32_t);
    mBrightMap = (byte*)exponet;
    mBrightMapSize = sizeof(exponet);
  }
  else if (variation==7)
  {
    
  }
  else
  {
  }
}
void
Exp::beat(Adafruit_DotStar *strip)
{
  WejDraw::beat(strip);
}
void
Exp::draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time)
{
  mColorMap = (uint32_t*)colorMaps[sMods[2]%7];
  if (mVariation > 6)
  {
    for (int i=start_led;i<start_led+num_led;i++)
    {
      strip->setPixelColor(i,0,0,0);
    }
    return;
  }  
  long pos = (frame_time-mBeatTime);
  long phase1 = pos/40;
  long phase2 = pos/10;
  long phase3 = pos/62;
  byte div;
  if (sMods[1]==0)
     div = 128;
  else
    div = 128 / (sMods[1]);
  for (int i=start_led;i<start_led+num_led;i++)
  {
    byte p = pgm_read_byte_near(mSpatialMap+i);
    byte indx = pgm_read_dword(mColorIndex+((p+phase1)%mColorIndexSize));
    short offset = sMods[0];
    short cindx = offset + (indx/div);
    uint32_t color = pgm_read_dword(mColorMap+(cindx%mColorMapSize));
    byte bright = pgm_read_dword(mBrightMap+(indx%mBrightMapSize));
    uint8_t b=color;
    uint8_t g=color>>8;
    uint8_t r=color>>16;
    b=b>>(bright/32);
    g=g>>(bright/32);
    r=r>>(bright/32);
    strip->setPixelColor(i,r,g,b);
  }
}
OffDraw::OffDraw(int num_leds) : WejDraw(num_leds)
{
}

void
OffDraw::draw(Adafruit_DotStar *strip,int start_led,int num_led,unsigned long frame_time)
{  
  for (int i=start_led;i<start_led+num_led;i++)
  {
    strip->setPixelColor(i,255,255,255);
  }
}
