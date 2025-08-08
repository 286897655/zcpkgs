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

#include <asio.hpp>

// 使用asio实现一个网络测试工具
// asio发送端开xxx个线程池 开指定参数个客户端发送数据 统计发送量
// asio服务端开xxx个线程池 接收客户端连接 统计接收量
// 测试服务器网络性能

// pingpong吞吐测试

// 纯发送接收测试，模拟rtp包

#include <zlog/log.h>
#include <zpkg/opt_flags.h>
#include <zpkg/skutil.h>
#include "avnet.h"

void asio_server(const net_scene& scene,const net_thread_set* t_set,int listen){
    zlog("asioavnet run in server mode for {} scene in {} threads",kScent[scene],t_set->threads);
    zlog("asioavnet server listen on {}",listen);

    // main io context
    asio::io_context main_iocontext;
    main_iocontext.run();
}


void asio_client(const net_scene& scene,const net_thread_set* t_set,const std::string& conn_ip,int conn_port){
    zlog("asioavnet run in client mode for {} scene in {} threads",kScent[scene],t_set->threads);
    zlog("asioavnet client connect server {}:{}",conn_ip,conn_port);

    

    // main io context
    asio::io_context main_iocontext;
    main_iocontext.run();
}

int main(int argc,char** argv){
    zlog::logger::create_defaultLogger();
    zpkg::OptionParser option_parser("asioavnet arg:");
    auto option_help = option_parser.add<zpkg::Switch>("h","help","print asioavnet help");
    auto option_client = option_parser.add<zpkg::Switch>("c","client","client for connect server,default is server mode");
    auto option_listen = option_parser.add<zpkg::Value<int>>("l","listen","server mode listen port,default is 12345");
    auto option_mode = option_parser.add<zpkg::Value<int>>("m","bench mode","1-pingpong test for throught 2-c/s mode send and receive in mbps");
    auto option_thread = option_parser.add<zpkg::Value<int>>("t","thread","c/s used thread count");
    auto option_conn = option_parser.add<zpkg::Value<std::string>>("d","address","client connect server ip:port");
    auto option_affinity = option_parser.add<zpkg::Switch>("a","affinity","set thread affinity,default false");
    auto option_nice = option_parser.add<zpkg::Switch>("n","nice","set thread nice,default false,if set nice will be -10");

    option_parser.parse(argc,argv);
    if(option_help->is_set()){
        zlog(option_parser.help());
        return 0;
    }
    // default use server mode
    net_mode mode = net_mode::server;
    if(option_client->is_set()){
        mode = net_mode::client;
    }
    int server_listen = 12345;
    if(option_listen->is_set()){
        server_listen = option_listen->value();
    }
    // default use pingpong test
    net_scene scene = net_scene::ping_pong;
    if(option_mode->is_set()){
        scene = static_cast<net_scene>(option_mode->value());
    }
    // thread set
    net_thread_set thread_set{.threads=std::thread::hardware_concurrency(),.affinity=false,.nice=false};
    if(option_thread->is_set()){
        thread_set.threads = option_thread->value();
    }
    if(option_affinity->is_set()){
        thread_set.affinity = true;
    }
    if(option_nice->is_set()){
        thread_set.nice = true;
    }
    // run client
    if(mode == net_mode::client){
        // client connect address
        std::string address = option_conn->value();
        auto var = zpkg::socket::parse_ip_colon_port(address);
        asio_client(scene,threads,var.first,var.second);
    }else{// run server
        asio_server(scene,threads,server_listen);
    }
};

