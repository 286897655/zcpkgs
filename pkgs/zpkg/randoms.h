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

#ifndef ZPKG_RANDOMS_H_
#define ZPKG_RANDOMS_H_

#include <random>

namespace zpkg{

class Random{
public:
    explicit Random(uint32_t seed);
    explicit Random();
    ~Random();

    // // default use uniform_int_distribution
    // uint32_t Next();
    // uint64_t Next64();
    // // uniform_int_distribution
    // uint32_t UniformNext(uint32_t n);
    // // normal_distribution
    // uint32_t NormalNext(uint32_t mean, uint32_t stddev);
    // bool PercentBool(int percentage);

    /// @brief generate random string
    /// @param length 
    /// @param has_upper 
    /// @param has_lower 
    /// @param has_number 
    /// @param has_special 
    /// @return 
    std::string RandomString(size_t length,bool has_upper = true,bool has_lower = true,bool has_number = true,bool has_special =false);
    
    /// @brief generate random hex string
    /// @param length 
    /// @return 
    std::string RandomHexString(size_t length);

    static Random& TLSInstance();
private:
    std::default_random_engine rng_;
    // std::uniform_int_distribution<uint32_t> uniform_;
    // std::normal_distribution<uint32_t> normal_;
};

class Random32{

private:
    std::mt19937 rng32_;
};

class Random64{

private:
    std::mt19937_64 rng64_;
};

}//!namesapce zpkg

#endif//!ZPKG_RANDOMS_H_