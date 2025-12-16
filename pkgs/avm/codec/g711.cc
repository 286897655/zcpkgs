/** 
 * @copyright Copyright © 2020-2025 code by zhaoj
 * 
 * LICENSE
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

 /**
 * @author zhaoj 286897655@qq.com
 * @brief G711 Codec g711a(alaw) g711u(ulaw)
 */
#include "g711.h"

namespace avm { 

#ifdef __x86_64__
static int top_bit(unsigned int bits) {
  int res;

  __asm__ __volatile__(
      " movq $-1,%%rdx;\n"
      " bsrq %%rax,%%rdx;\n"
      : "=d"(res)
      : "a"(bits));
  return res;
}

static int bottom_bit(unsigned int bits) {
  int res;

  __asm__ __volatile__(
      " movq $-1,%%rdx;\n"
      " bsfq %%rax,%%rdx;\n"
      : "=d"(res)
      : "a"(bits));
  return res;
}
#else // not __x86_64__ back to c
static int top_bit(unsigned int bits) {
  int i;

  if (bits == 0) {
    return -1;
  }
  i = 0;
  if (bits & 0xFFFF0000) {
    bits &= 0xFFFF0000;
    i += 16;
  }
  if (bits & 0xFF00FF00) {
    bits &= 0xFF00FF00;
    i += 8;
  }
  if (bits & 0xF0F0F0F0) {
    bits &= 0xF0F0F0F0;
    i += 4;
  }
  if (bits & 0xCCCCCCCC) {
    bits &= 0xCCCCCCCC;
    i += 2;
  }
  if (bits & 0xAAAAAAAA) {
    bits &= 0xAAAAAAAA;
    i += 1;
  }
  return i;
}

static  int bottom_bit(unsigned int bits) {
  int i;

  if (bits == 0) {
    return -1;
  }
  i = 32;
  if (bits & 0x0000FFFF) {
    bits &= 0x0000FFFF;
    i -= 16;
  }
  if (bits & 0x00FF00FF) {
    bits &= 0x00FF00FF;
    i -= 8;
  }
  if (bits & 0x0F0F0F0F) {
    bits &= 0x0F0F0F0F;
    i -= 4;
  }
  if (bits & 0x33333333) {
    bits &= 0x33333333;
    i -= 2;
  }
  if (bits & 0x55555555) {
    bits &= 0x55555555;
    i -= 1;
  }
  return i;
}
#endif // 

/*
 * g711.c
 *
 * u-law, A-law and linear PCM conversions.
 * Source: http://www.speech.kth.se/cost250/refsys/latest/src/g711.c
 */
#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0x0F)		/* Quantization field mask. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
#define ALAW_AMI_MASK (0x55)    /* ALAW AMI mask. */
#define ULAW_BIAS      (0x84)  /* ULAW Bias for linear code. */

#ifdef TOP_BIT_SEARCH
#define	NSEGS		(8)		/* Number of A/U-law segments. */
static constexpr int16_t seg_aend[NSEGS] = { 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF };
static constexpr int16_t seg_uend[NSEGS] = { 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF };

static int16_t search(int16_t val, int16_t *table, int16_t size){
   int16_t i;
   
   for (i = 0; i < size; i++) {
      if (val <= *table++)
	 return (i);
   }
   return (size);
}
#endif

uint8_t	linear2alaw(int16_t linear){
    int16_t mask;
    int16_t seg;

    if (linear >= 0) {
        /* Sign (bit 7) bit = 1 */
        mask = ALAW_AMI_MASK | SIGN_BIT;
    } else {
        /* Sign (bit 7) bit = 0 */
        mask = ALAW_AMI_MASK;
        /* WebRtc, tlegrand: Changed from -8 to -1 to get bitexact to reference
            * implementation */
        linear = -linear - 1;
    }

    /* Convert the scaled magnitude to segment number. */
#ifdef TOP_BIT_SEARCH
    seg = search(linear, seg_aend, NSEGS);
#else
    seg = top_bit(linear | 0xFF) - 7;
#endif
    
    if (seg >= 8) {
        if (linear >= 0) {
            /* Out of range. Return maximum value. */
            return (uint8_t)(0x7F ^ mask);
        }
        /* We must be just a tiny step below zero */
        return (uint8_t)(0x00 ^ mask);
    }
    /* Combine the sign, segment, and quantization bits. */
    return (uint8_t)(((seg << 4) | ((linear >> ((seg) ? (seg + 3) : 4)) & QUANT_MASK)) ^
                    mask);
}

int16_t	alaw2linear(uint8_t alaw){
    int i;
    int seg;

    alaw ^= ALAW_AMI_MASK;
    i = ((alaw & QUANT_MASK) << 4);
    seg = (((int)alaw & SEG_MASK) >> 4);
    if (seg)
        i = (i + 0x108) << (seg - 1);
    else
        i += 8;
    return (int16_t)((alaw & SIGN_BIT) ? i : -i);
}


uint8_t	linear2ulaw(int16_t linear){
    uint8_t u_val;
    int mask;
    int seg;

    /* Get the sign and the magnitude of the value. */
    if (linear < 0) {
        /* WebRtc, tlegrand: -1 added to get bitexact to reference implementation */
        linear = ULAW_BIAS - linear - 1;
        mask = 0x7F;
    } else {
        linear = ULAW_BIAS + linear;
        mask = 0xFF;
    }

#ifdef TOP_BIT_SEARCH
    seg = search(linear, seg_uend, NSEGS);
#else
    seg = top_bit(linear | 0xFF) - 7;
#endif
    /*
    * Combine the sign, segment, quantization bits,
    * and complement the code word.
    */
    if (seg >= 8)
        u_val = (uint8_t)(0x7F ^ mask);
    else
        u_val = (uint8_t)(((seg << 4) | ((linear >> (seg + 3)) & QUANT_MASK)) ^ mask);
#ifdef ULAW_ZEROTRAP
    /* Optional ITU trap */
    if (u_val == 0)
        u_val = 0x02;
#endif
    return u_val;
}

int16_t	ulaw2linear(uint8_t ulaw){
    int t;

    /* Complement to obtain normal u-law value. */
    ulaw = ~ulaw;
    /*
    * Extract and bias the quantization bits. Then
    * shift up by the segment number and subtract out the bias.
    */
    t = (((ulaw & QUANT_MASK) << 3) + ULAW_BIAS) << (((int)ulaw & SEG_MASK) >> 4);
    return (int16_t)((ulaw & SIGN_BIT) ? (ULAW_BIAS - t) : (t - ULAW_BIAS));
}

/* Copied from the CCITT G.711 specification */
static constexpr uint8_t ulaw_to_alaw_table[256] = {
   42,  43,  40,  41,  46,  47,  44,  45,  34,  35,  32,  33,  38,  39,  36,
   37,  58,  59,  56,  57,  62,  63,  60,  61,  50,  51,  48,  49,  54,  55,
   52,  53,  10,  11,   8,   9,  14,  15,  12,  13,   2,   3,   0,   1,   6,
    7,   4,  26,  27,  24,  25,  30,  31,  28,  29,  18,  19,  16,  17,  22,
   23,  20,  21, 106, 104, 105, 110, 111, 108, 109,  98,  99,  96,  97, 102,
  103, 100, 101, 122, 120, 126, 127, 124, 125, 114, 115, 112, 113, 118, 119,
  116, 117,  75,  73,  79,  77,  66,  67,  64,  65,  70,  71,  68,  69,  90,
   91,  88,  89,  94,  95,  92,  93,  82,  82,  83,  83,  80,  80,  81,  81,
   86,  86,  87,  87,  84,  84,  85,  85, 170, 171, 168, 169, 174, 175, 172,
  173, 162, 163, 160, 161, 166, 167, 164, 165, 186, 187, 184, 185, 190, 191,
  188, 189, 178, 179, 176, 177, 182, 183, 180, 181, 138, 139, 136, 137, 142,
  143, 140, 141, 130, 131, 128, 129, 134, 135, 132, 154, 155, 152, 153, 158,
  159, 156, 157, 146, 147, 144, 145, 150, 151, 148, 149, 234, 232, 233, 238,
  239, 236, 237, 226, 227, 224, 225, 230, 231, 228, 229, 250, 248, 254, 255,
  252, 253, 242, 243, 240, 241, 246, 247, 244, 245, 203, 201, 207, 205, 194,
  195, 192, 193, 198, 199, 196, 197, 218, 219, 216, 217, 222, 223, 220, 221,
  210, 210, 211, 211, 208, 208, 209, 209, 214, 214, 215, 215, 212, 212, 213,
  213
};

/* copied from the CCITT G.711 specification */
static constexpr uint8_t alaw_to_ulaw_table[256] = {
   42,  43,  40,  41,  46,  47,  44,  45,  34,  35,  32,  33,  38,  39,  36,
   37,  57,  58,  55,  56,  61,  62,  59,  60,  49,  50,  47,  48,  53,  54,
   51,  52,  10,  11,   8,   9,  14,  15,  12,  13,   2,   3,   0,   1,   6,
    7,   4,   5,  26,  27,  24,  25,  30,  31,  28,  29,  18,  19,  16,  17,
   22,  23,  20,  21,  98,  99,  96,  97, 102, 103, 100, 101,  93,  93,  92,
   92,  95,  95,  94,  94, 116, 118, 112, 114, 124, 126, 120, 122, 106, 107,
  104, 105, 110, 111, 108, 109,  72,  73,  70,  71,  76,  77,  74,  75,  64,
   65,  63,  63,  68,  69,  66,  67,  86,  87,  84,  85,  90,  91,  88,  89,
   79,  79,  78,  78,  82,  83,  80,  81, 170, 171, 168, 169, 174, 175, 172,
  173, 162, 163, 160, 161, 166, 167, 164, 165, 185, 186, 183, 184, 189, 190,
  187, 188, 177, 178, 175, 176, 181, 182, 179, 180, 138, 139, 136, 137, 142,
  143, 140, 141, 130, 131, 128, 129, 134, 135, 132, 133, 154, 155, 152, 153,
  158, 159, 156, 157, 146, 147, 144, 145, 150, 151, 148, 149, 226, 227, 224,
  225, 230, 231, 228, 229, 221, 221, 220, 220, 223, 223, 222, 222, 244, 246,
  240, 242, 252, 254, 248, 250, 234, 235, 232, 233, 238, 239, 236, 237, 200,
  201, 198, 199, 204, 205, 202, 203, 192, 193, 191, 191, 196, 197, 194, 195,
  214, 215, 212, 213, 218, 219, 216, 217, 207, 207, 206, 206, 210, 211, 208,
  209
};

uint8_t	alaw2ulaw(uint8_t alaw){
    return alaw_to_ulaw_table[alaw];
}

uint8_t	ulaw2alaw(uint8_t ulaw){
    return ulaw_to_alaw_table[ulaw];
}
};//! namespace avm