#include <zio/io_loop_ctx.h>
#include <zlog/log.h>


std::shared_ptr<zio::io_timer_t> timer;
int tick_count = 0;
void test_loop_func(){
    zio::io_loop_t* current_loop = zio::io_loop_t::this_thread_loop();
    timer = std::make_shared<zio::io_timer_t>(current_loop);
    timer->reset([]() -> bool {
        zlog("timer ticked");
        tick_count++;
        if(tick_count > 5){
            timer.reset();
            return false;
        }
        return true;
    },5*Z_UTIME_MILLISECONDS);
}

int main(int argc,char** argv){
    zlog::logger::create_defaultLogger();
    size_t created_loop = zio::io_loop_t::create_loop_pool();
    zio::io_loop_t* default_loop = zio::io_loop_t::main_loop();
    std::thread new_thread([default_loop](){
        default_loop->async([](){
            zlog("run in new thread");
            test_loop_func();
        });
    });
    default_loop->run();
}