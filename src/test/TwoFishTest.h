/*
* Copyright (c) 2003-2009 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/
// TwoFishTest.h: Unit test for TwoFish implementation
#include "test.h"
#include "../../corelib/TwoFish.h"

class CTwoFishTest : public Test
{

public:
  CTwoFishTest()
  {
  }

  void run()
  {
    _test(twofish_test() == 0);
    ecb_vk();
  }


  int twofish_test(void)
  {
    static const struct { 
      int keylen;
      unsigned char key[32], pt[16], ct[16];
    } tests[] = {
      { 16,
      { 0x9F, 0x58, 0x9F, 0x5C, 0xF6, 0x12, 0x2C, 0x32,
      0xB6, 0xBF, 0xEC, 0x2F, 0x2A, 0xE8, 0xC3, 0x5A },
      { 0xD4, 0x91, 0xDB, 0x16, 0xE7, 0xB1, 0xC3, 0x9E,
      0x86, 0xCB, 0x08, 0x6B, 0x78, 0x9F, 0x54, 0x19 },
      { 0x01, 0x9F, 0x98, 0x09, 0xDE, 0x17, 0x11, 0x85,
      0x8F, 0xAA, 0xC3, 0xA3, 0xBA, 0x20, 0xFB, 0xC3 }
      }, {
        24,
        { 0x88, 0xB2, 0xB2, 0x70, 0x6B, 0x10, 0x5E, 0x36,
        0xB4, 0x46, 0xBB, 0x6D, 0x73, 0x1A, 0x1E, 0x88,
        0xEF, 0xA7, 0x1F, 0x78, 0x89, 0x65, 0xBD, 0x44 },
        { 0x39, 0xDA, 0x69, 0xD6, 0xBA, 0x49, 0x97, 0xD5,
        0x85, 0xB6, 0xDC, 0x07, 0x3C, 0xA3, 0x41, 0xB2 },
        { 0x18, 0x2B, 0x02, 0xD8, 0x14, 0x97, 0xEA, 0x45,
        0xF9, 0xDA, 0xAC, 0xDC, 0x29, 0x19, 0x3A, 0x65 }
      }, { 
        32,
        { 0xD4, 0x3B, 0xB7, 0x55, 0x6E, 0xA3, 0x2E, 0x46,
        0xF2, 0xA2, 0x82, 0xB7, 0xD4, 0x5B, 0x4E, 0x0D,
        0x57, 0xFF, 0x73, 0x9D, 0x4D, 0xC9, 0x2C, 0x1B,
        0xD7, 0xFC, 0x01, 0x70, 0x0C, 0xC8, 0x21, 0x6F },
        { 0x90, 0xAF, 0xE9, 0x1B, 0xB2, 0x88, 0x54, 0x4F,
        0x2C, 0x32, 0xDC, 0x23, 0x9B, 0x26, 0x35, 0xE6 },
        { 0x6C, 0xB4, 0x56, 0x1C, 0x40, 0xBF, 0x0A, 0x97,
        0x05, 0x93, 0x1C, 0xB6, 0xD4, 0x08, 0xE7, 0xFA }
      }
    };

    TwoFish *tf;

    unsigned char tmp[2][16];
    int i, y;

    for (i = 0; i < (int)(sizeof(tests)/sizeof(tests[0])); i++) {
      tf = new TwoFish(tests[i].key, tests[i].keylen);

      tf->Encrypt(tests[i].pt, tmp[0]);
      tf->Decrypt(tmp[0], tmp[1]);
      if (memcmp(tmp[0], tests[i].ct, 16) != 0 || memcmp(tmp[1], tests[i].pt, 16) != 0) {
        delete tf;
        return 1;
      }
      /* now see if we can encrypt all zero bytes 1000 times, decrypt and come back where we started */
      for (y = 0; y < 16; y++) tmp[0][y] = 0;
      for (y = 0; y < 1000; y++) tf->Encrypt(tmp[0], tmp[0]);
      for (y = 0; y < 1000; y++) tf->Decrypt(tmp[0], tmp[0]);
      for (y = 0; y < 16; y++) if (tmp[0][y] != 0) {delete tf; return 2;}
      delete tf;
    }
    return 0;
  }
  void ecb_vk()
  {
    unsigned char PT[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // variable key known answer test vectors
    struct {
      unsigned char key[32];
      unsigned char CT[16];
    } vectors[] = {
      /* I=1 */
      {{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x78, 0x52, 0x29, 0xB5, 0x1B, 0x51, 0x5F, 0x30,
      0xA1, 0xFC, 0xC8, 0x8B, 0x96, 0x9A, 0x4E, 0x47}},
      /* I=8 */
      {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x00, 0x5E, 0xA3, 0xAF, 0x8A, 0xFF, 0x3D, 0xDA,
      0x32, 0x31, 0x48, 0x69, 0x05, 0x37, 0x85, 0x3C}},
      /* I=16 */
      {{0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x6D, 0x1B, 0x6F, 0x14, 0x09, 0x03, 0x68, 0x03,
      0x4E, 0x10, 0xCF, 0x0C, 0x1E, 0x4F, 0x57, 0x44}},
      /* I=24 */
      {{0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x13, 0x29, 0xEA, 0x75, 0x51, 0xCE, 0x6C, 0x33,
      0x5D, 0xB9, 0x24, 0xD5, 0x63, 0x69, 0x40, 0x58}},
      /* I=32 */
      {{0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x43, 0x6A, 0x5D, 0x31, 0x5A, 0xF4, 0x43, 0xDE,
      0xA9, 0xBE, 0xF8, 0xD1, 0xE8, 0x17, 0xE7, 0xE0}},
      /* I=40 */
      {{0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0xE0, 0x92, 0x06, 0xEB, 0x6A, 0x5E, 0x8A, 0xC9,
      0x33, 0xBA, 0xAB, 0x46, 0x54, 0x7E, 0x4C, 0xD9}},
      /* I=52 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x8F, 0xB6, 0xCA, 0x96, 0x6B, 0x5A, 0xCF, 0xB1,
      0x80, 0xA2, 0x96, 0xEA, 0x5D, 0x93, 0x71, 0x1F}},
      /* I=64 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x13, 0x64, 0x5D, 0xBE, 0xDE, 0x21, 0xFF, 0x7C,
      0x79, 0xC0, 0x61, 0x41, 0xAD, 0x9E, 0x4C, 0xD1}},
      /* I=81 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x0C, 0xF4, 0x08, 0xA2, 0xFB, 0xDA, 0x07, 0x06,
      0x8B, 0xDB, 0x13, 0xA3, 0x71, 0x86, 0x7F, 0xCC}},
      /* I=92 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0xB7, 0xDF, 0x97, 0x1F, 0x9C, 0x34, 0x44, 0xEF,
      0xCC, 0x13, 0x21, 0x02, 0x92, 0x12, 0x69, 0x42}},
      /* I=129 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x12, 0xC0, 0x06, 0x18, 0xDA, 0x7E, 0xBA, 0x5E,
      0xFA, 0x5E, 0x58, 0xD2, 0x69, 0x6D, 0x89, 0x1F}},
      /* I=142 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x7D, 0xB9, 0xAD, 0xCF, 0xA2, 0x6C, 0x1E, 0x78,
      0x4E, 0x7F, 0x48, 0x5B, 0xD0, 0xA0, 0xA5, 0x2C}},
      /* I=160 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x6B, 0x35, 0xA3, 0x44, 0xBD, 0x8D, 0xFD, 0x40,
      0x02, 0xF5, 0xF2, 0x2E, 0xA2, 0x88, 0xF8, 0xE6}},
      /* I=183 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0xF5, 0x14, 0x10, 0x47, 0x5B, 0x33, 0xFB, 0xD3,
      0xDB, 0x21, 0x17, 0xB5, 0xC1, 0x7C, 0x82, 0xD4}},
      /* I=202 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      {0x51, 0xBE, 0x07, 0xBA, 0x81, 0x06, 0xA1, 0x9C,
      0xCC, 0x20, 0x05, 0xB8, 0xB3, 0x93, 0x2F, 0xBF}},
      /* I=219 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00},
      {0x39, 0xAE, 0x85, 0x9D, 0x25, 0x54, 0x87, 0xA6,
      0x80, 0x93, 0xA3, 0x76, 0xD3, 0x58, 0xBB, 0xC2}},
      /* I=233 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00},
      {0x55, 0x07, 0x26, 0x37, 0x5A, 0xBB, 0x0F, 0x9A,
      0x7C, 0x01, 0x0D, 0xC4, 0xE4, 0x78, 0x33, 0xF9}},
      /* I=247 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00},
      {0x88, 0x81, 0x1B, 0x50, 0x6D, 0x56, 0x57, 0x79,
      0xF0, 0x9D, 0xE9, 0xBD, 0xF8, 0x70, 0x2B, 0xD8}},
      /* I=256 */
      {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
      {0x85, 0xF3, 0x45, 0x36, 0x61, 0x55, 0xD1, 0x3F,
      0x8F, 0x25, 0x77, 0x34, 0xD2, 0xCB, 0xD6, 0xD9}},
    };
    unsigned char res[16];
    for (size_t i = 0; i < sizeof(vectors)/sizeof(vectors[0]); i++) {
      TwoFish *tf = new TwoFish(vectors[i].key, 32);
      tf->Encrypt(PT, res);
      delete tf;
      _test(memcmp(res, vectors[i].CT, 16) == 0);
    }
  }
};

