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

/**
 * @author zhaoj 286897655@qq.com
 * @brief 
 */

#ifndef ZAV_PROTO_VVW_H_
#define ZAV_PROTO_VVW_H_

#include <string>

namespace zav{

/**
 * represent a Virtual Video Wall
 * 3*3 represent follows
 * 1 2 3
 * 4 5 6
 * 7 8 9
 * 2*1 represent follws
 * 1
 * 2
*/

struct Rect{
    uint32_t XCord;
    uint32_t YCord;
    uint32_t Width;
    uint32_t Height;
};

class VirtualVideoWall{
public:
    VirtualVideoWall(const std::string& wallid,int screenRow,int screenCol);
    ~VirtualVideoWall() = default;

    std::string ID() const;
    int Row() const;
    int Col() const;

    Rect Screen(int screenid);

private:
    std::string wall_id_;
    int screen_row_;
    int screen_col_;
};
////////////////////////implemantation///////////////////////////////
inline std::string VirtualVideoWall::ID() const{
    return wall_id_;
}
inline int VirtualVideoWall::Row() const{
    return screen_row_;
}
inline int VirtualVideoWall::Col() const{
    return screen_col_;
}
////////////////////////implemantation///////////////////////////////

};//!namespace zav

#endif//!ZAV_PROTO_VVW_H_