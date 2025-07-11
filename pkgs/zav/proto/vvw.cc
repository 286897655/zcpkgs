/** 
 * @copyright Copyright © 2020-2025 zhaoj
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

#include "zav/proto/vvw.h"
#include <zpkg/utility.h>

namespace zav{

constexpr static uint32_t kVirtualVideoWallWidth = 1920;
constexpr static uint32_t kVirtualVideoWallHeight = 1920;

VirtualVideoWall::VirtualVideoWall(const std::string& wallid,int screenRow,int screenCol)
    :wall_id_(wallid),screen_row_(screenRow),screen_col_(screenCol){
    Z_ASSERT(!wall_id_.empty());
    Z_ASSERT(screen_row_ >= 1);
    Z_ASSERT(screen_col_ >= 1);
}

Rect VirtualVideoWall::Screen(int screenid){
    Z_ASSERT(screenid <= screen_row_ * screen_col_);
    Z_ASSERT(screenid > 0);

    // row index from 1
    // col index from 1
    // quot 是商
    // rem 是余数
    ::div_t div = ::div((screenid - 1),screen_col_);
    return Rect{
            div.rem * kVirtualVideoWallWidth,
            div.quot * kVirtualVideoWallHeight,
            kVirtualVideoWallWidth,
            kVirtualVideoWallHeight
    };
}

};//!namespace zav