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

#ifndef ZAV_OBJECT_AV_PTS_DTS_H_
#define ZAV_OBJECT_AV_PTS_DTS_H_

#include <set>
#include <stdint.h>
#include <stddef.h>

namespace zav{

enum{
    AV_INVALID_PTS = 0x8000000000000000
};

class DtsGenWithPts{
public:
    DtsGenWithPts();
    ~DtsGenWithPts();

    bool GenDts(uint64_t pts,uint64_t& dts);

private:
    bool genDts_l(uint64_t pts, uint64_t &dts);

private:
    uint64_t _dts_pts_offset = 0;
    uint64_t _last_dts = 0;
    uint64_t _last_pts = 0;
    uint64_t _last_max_pts = 0;
    size_t _frames_since_last_max_pts = 0;
    size_t _sorter_max_size = 0;
    size_t _count_sorter_max_size = 0;
    std::set<uint64_t> _pts_sorter;
};

/**
 * Pts缓存,用于在比如RTSP场景下 拿到的只有PTS
 * 无B帧的情况下DTS=PTS
 * 有B帧的情况下对PTS缓存B帧+1个PTS
*/
class PtsCache{
public:
    PtsCache();
    ~PtsCache();

    uint64_t GenerateDts(uint64_t pts);

private:
    void reset();
private:
    int delay_;//计算得到的B帧个数
    uint64_t last_pts_;//缓存上一个pts
    uint64_t count_max_pts_;//当前计算的最大pts
    std::set<uint64_t> pts_cache_;
};


};//!namespace zav


#endif//!ZAV_OBJECT_AV_PTS_DTS_H_