/** 
 * @copyright Copyright © 2020-2024 zhaoj
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

#ifndef ZAV_HLS_H_
#define ZAV_HLS_H_

#include <zpkg/utility.h>
#include <zav/av.h>
#include <map>

namespace zav{

struct ts_segment : public zpkg::apply_unique<ts_segment>{
    // ts duration #EXTINF:10.00
    float duration = 0.0f;
    // Embedded m3u8 program
    int program_id = 0;;
    // Bandwidth
    int band_width = 0;
    int width = 0;
    int height = 0;
    // ts url 
    std::string ts_url;
};

class m3u8_parser : public zpkg::apply_unique<m3u8_parser>{
public:
    // #EXTM3U
    bool is_m3u8;
    // #EXT-X-ALLOW-CACHE
    bool allow_cache;
    // #EXT-X-ENDLIST exist is live or vod
    bool is_live;
    // 是否嵌套m3u8(有些m3u8里面含别的m3u8)
    bool embedded_m3u8;
    // #EXT-X-VERSION
    int version;
    // #EXT-X-TARGETDURATION
    float target_duration;
    // duration total
    float total_duration;
    // #EXT-X-MEDIA-SEQUENCE
    uint64_t media_sequence;
    // ts map key with index
    std::map<int,ts_segment::unique> ts_map;
    std::string m3u8_url;
public:
    explicit m3u8_parser(const std::string& url);
    ~m3u8_parser()=default;

    bool parse(const std::string& m3u8_content);


};


};//!namespace zav

#endif//!ZAV_HLS_H_