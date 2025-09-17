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

#include "xmath.h"
#include <cmath>

namespace zpkg{

bool primes::is_prime(uint32_t x){
    if (x && x < 4) {
        // eliminates special-casing 2.
        return true;
    } else if ((x & 1) == 0) {
        // eliminates even numbers >2.
        return false; 
    }

    uint32_t limit = std::sqrt(x);
    for (uint32_t factor = 3; factor <= limit; factor += 2) {
        if ((x % factor) == 0) {
            return false;
        }
    }
    return true;
}

uint32_t primes::next_prime_over(uint32_t x){
    x += (x % 2) + 1;
    while (!is_prime(x)) {
        x += 2;
    }
    return x;
}
};//!namespace zpkg