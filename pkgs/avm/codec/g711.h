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

 /*
 * SpanDSP - a series of DSP components for telephony
 *
 * g711.h - In line A-law and u-law conversion routines
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
 *
 *  Despite my general liking of the GPL, I place this code in the
 *  public domain for the benefit of all mankind - even the slimy
 *  ones who might try to proprietize my work and use it to my
 *  detriment.
 *
 * $Id: g711.h,v 1.1 2006/06/07 15:46:39 steveu Exp $
 *
 * Modifications for WebRtc, 2011/04/28, by tlegrand:
 * -Changed to use WebRtc types
 * -Changed __inline__ to __inline
 * -Two changes to make implementation bitexact with ITU-T reference
 * implementation
 * 
 * Modification for zav,2024/07,by zhaoj
 * -merge old g711 and webrtc-g711
 */

/*! \page g711_page A-law and mu-law handling
Lookup tables for A-law and u-law look attractive, until you consider the impact
on the CPU cache. If it causes a substantial area of your processor cache to get
hit too often, cache sloshing will severely slow things down. The main reason
these routines are slow in C, is the lack of direct access to the CPU's "find
the first 1" instruction. A little in-line assembler fixes that, and the
conversion routines can be faster than lookup tables, in most real world usage.
A "find the first 1" instruction is available on most modern CPUs, and is a
much underused feature.

If an assembly language method of bit searching is not available, these routines
revert to a method that can be a little slow, so the cache thrashing might not
seem so bad :(

Feel free to submit patches to add fast "find the first 1" support for your own
favourite processor.

Look up tables are used for transcoding between A-law and u-law, since it is
difficult to achieve the precise transcoding procedure laid down in the G.711
specification by other means.
*/

#ifndef AVM_CODEC_G711_H_
#define AVM_CODEC_G711_H_ 

namespace avm{ 

};//! namespace avm

#endif//!AVM_CODEC_G711_H_