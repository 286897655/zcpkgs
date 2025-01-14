#include <zio/io_poller.h>
#include <zlog/log.h>


int main(int argc,char** argv){
    zlog::logger::create_defaultLogger();
    size_t created_loop = zio::io_loop_t::create_loop_pool();
    zio::io_loop_t* default_loop = zio::io_loop_t::default_loop();
    std::thread new_thread([default_loop](){
        default_loop->async([](){
            zlog("run in new thread");
        });
    });

    default_loop->run();
}