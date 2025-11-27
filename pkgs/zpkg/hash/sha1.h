#pragma once

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
 * @brief 
 */

//	100% Public Domain.
//
//	Original C Code
//	 -- Steve Reid <steve@edmweb.com>
//	Small changes to fit into bglibs
//	  -- Bruce Guenter <bruce@untroubled.org>
//	Translation to simpler C++ Code
//	  -- Volker Grabsch <vog@notjusthosting.com>
//	Safety fixes
//	  -- Eugene Hopkinson <slowriot at voxelstorm dot com>
//  Adapt for project
//      Dmitriy Khaustov <khaustov.dm@gmail.com>
//
// File created on: 2017.02.25

// SHA1.h
#ifndef ZPKG_HASH_SHA1_H
#define ZPKG_HASH_SHA1_H


#include <cstdint>
#include <iostream>
#include <string>

namespace zpkg {

class SHA1 final
{
public:
    SHA1();

    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    std::string final_bin();

    static std::string from_file(const std::string &filename);

    static std::string encode(const std::string &s);
    static std::string encode_bin(const std::string &s);

private:
    uint32_t digest[5];
    std::string buffer;
    uint64_t transforms;
};

}//namespace zpkg

#endif//!ZPKG_HASH_SHA1_H