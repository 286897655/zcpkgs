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

#include "zav/proto/hls.h"
#include <inttypes.h>
#include <zlog/log.h>
namespace zav{

static constexpr const char kM3U8_START[] = "#EXTM3U";
static constexpr const char kM3U8_ALLOW_CACHE[] = "#EXT-X-ALLOW-CACHE";
static constexpr const char kM3U8_VERSION[] = "#EXT-X-VERSION";
static constexpr const char kM3U8_TARGETDURATION[] = "#EXT-X-TARGETDURATION";
static constexpr const char kM3U8_MEDIA_SEQUENCE[] = "#EXT-X-MEDIA-SEQUENCE";
static constexpr const char kM3U8_EMBEDDED_M3U8[] = "#EXT-X-STREAM-INF";
static constexpr const char kM3U8_EXTINF[] = "#EXTINF";
static constexpr const char kM3U8_ENDLIST[] = "#EXT-X-ENDLIST";

m3u8_parser::m3u8_parser(const std::string& url)
    :is_m3u8(false),allow_cache(false),is_live(true),embedded_m3u8(false),
        version(0),target_duration(0),total_duration(0),media_sequence(0),
        m3u8_url(url){
}

static std::string join_ts_url(const std::string& m3u8_url,const std::string& ts_path)
{
    Z_ASSERT(!m3u8_url.empty());
    Z_ASSERT(!ts_path.empty());

    // start with someprotocol
    if(ts_path.find("://") != std::string::npos){
        return ts_path;
    }

    // start with // ts直接带了path 则直接拼接m3u8的协议头就行
    if(ts_path.find("//") == 0){
        std::string head_protocol = m3u8_url.substr(0,m3u8_url.find("://"));
        return head_protocol + ":" + ts_path;
    }

    std::string m3u8_head = m3u8_url.substr(0,m3u8_url.rfind("/"));
    // start with / ts直接带/ 表名相对于m3u8地址的 去除m3u8后缀文件名然后拼接ts地址即可
    if(ts_path.find("/") == 0){
        return m3u8_head + ts_path;
    }

    // else start 
    return m3u8_head + "/" + ts_path;
}

bool m3u8_parser::parse(const std::string& m3u8_content){
    if(m3u8_content.empty()) return false;

    float cur_inf_duration = 0.0f;
    int ts_index = 0;
    std::vector<std::string> lines = zpkg::strings::split(m3u8_content,zpkg::kstrings::kLF);
    for(std::string& cur_line : lines){
        // trim space
        zpkg::strings::trim(cur_line);
        if(cur_line.size() < 2)
            continue;
        // #EXTM3U
        if(cur_line.find(kM3U8_START) == 0){
            is_m3u8 = true;
            continue;
        }
        // #EXT-X-ALLOW-CACHE
        if(cur_line.find(kM3U8_ALLOW_CACHE) == 0){
            this->allow_cache = cur_line.find(":YES") != std::string::npos;
            continue;
        }
        // #EXT-X-ENDLIST
        if(cur_line.find(kM3U8_ENDLIST) == 0){
            // this is vod
            is_live = false;
            continue;
        }
        // #EXT-X-VERSION
        if(cur_line.find(kM3U8_VERSION) == 0){
            version = std::strtol(cur_line.c_str()+(sizeof("#EXT-X-VERSION:")-1),nullptr,10);
            continue;
        }
        // #EXT-X-TARGETDURATION
        if(cur_line.find(kM3U8_TARGETDURATION) == 0){
            target_duration = std::strtof(cur_line.c_str()+(sizeof("#EXT-X-TARGETDURATION:")-1),nullptr);
            continue;
        }
        // #EXT-X-MEDIA-SEQUENCE
        if(cur_line.find(kM3U8_MEDIA_SEQUENCE) == 0){
            media_sequence = std::strtoul(cur_line.c_str()+(sizeof("#EXT-X-MEDIA-SEQUENCE:")-1),nullptr,10);
            continue;
        }

        // #EXT-X-STREAM-INF
        if(cur_line.find(kM3U8_EMBEDDED_M3U8) == 0){
            embedded_m3u8 = true;
            zlog_warn("zav:not support embedded m3u8 now...");
            return false;
        }

        // #EXTINF
        if(cur_line.find(kM3U8_EXTINF) == 0){
            cur_inf_duration = std::strtof(cur_line.c_str()+(sizeof("#EXTINF:")-1),nullptr);
            total_duration += cur_inf_duration;
            continue;
        }

        // ts info 
        if(cur_inf_duration > 0.0f && cur_line.find("#") != 0){
            // this is ts got extinf duration and not start #
            ts_segment::unique ts_seg = std::make_unique<ts_segment>();
            ts_seg->ts_url = join_ts_url(m3u8_url,cur_line);
            ts_seg->duration = cur_inf_duration;
            ts_map.emplace(ts_index++,std::move(ts_seg));
            cur_inf_duration = 0.0f;
            continue;
        }
    }
    // parse success
    return true;
}

std::string m3u8_parser::format(){
    std::ostringstream oss;
    oss << "m3u8_url:" << m3u8_url << zpkg::kstrings::kLF
        << "is_m3u8:" << is_m3u8 << zpkg::kstrings::kLF
        << "allow_cache:"<< allow_cache << zpkg::kstrings::kLF
        << "is_live:"<< is_live << zpkg::kstrings::kLF
        << "embedded_m3u8:"<< embedded_m3u8 << zpkg::kstrings::kLF
        << "version:" << version << zpkg::kstrings::kLF
        << "target_duration:"<< target_duration << zpkg::kstrings::kLF
        << "total_duration:"<< total_duration << zpkg::kstrings::kLF
        << "media_sequence:" << media_sequence << zpkg::kstrings::kLF
        << std::endl;
    
    return oss.str();
}

std::string m3u8_merger::merge_m3u8_content(const std::map<std::string,std::string>& contents,
                            uint64_t begin_time,uint64_t start_time,uint64_t end_time){
    if(contents.empty())
        return zpkg::kstrings::kEmpty;
    
    std::string content = contents.begin()->second;
    std::ostringstream oss;
    oss << hls::kMergedM3U8Prefix << content;
    return oss.str();

    // std::ostringstream oss;
    // oss << hls::kMergedM3U8Prefix << kM3U8_START << zpkg::kstrings::kLF
    //     << kM3U8_VERSION << ":" << 3 << zpkg::kstrings::kLF;
    // std::map<std::string,m3u8_parser::unique> parsed;
    // float max_target_duration = 0.0f;
    // int last_media_sequence = -1;
    // // begin_time == 0 start_time == 0 end_time == 0
    // // 全部合并
    // // begin_time == 0 start_time != 0 end_time != 0
    // // start_time和end_time都是相对值秒来合并
    // // begin_time != 0 start_time != 0 end_time != 0
    // // 知道m3u8的开始时间 都是绝对值的秒
    // for(auto it = contents.begin(); it != contents.end(); it++){
    //     m3u8_parser::unique parser = std::make_unique<m3u8_parser>(it->first);
    //     if(!parser->parse(it->second)){
    //         // parse fail
    //         zlog("cuc_hls:parse m3u8 {} fail",it->first);
    //         // return empty with merge fail
    //         return zpkg::kstrings::kEmpty; 
    //     }
    //     max_target_duration = std::max(max_target_duration,parser->target_duration);
    // }

    // if(contents.size() == 1){
    //     // 只有一个的话 直接返回首个的内容
    //     return it->second;
    // }
    // // TODO zhaoj 都先只支持 只有一个m3u8 并且从头开始播放
    // return it->second;
}


};//!namespace zav