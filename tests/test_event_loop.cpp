#include <zio/events.h>
#include <zlog/log.h>


std::shared_ptr<zio::event_timer_t> timer;
int tick_count = 0;
void test_loop_func(){
    zio::event_loop_t* current_loop = zio::events_ctx::this_thread_loop();
    timer = std::make_shared<zio::event_timer_t>(current_loop);
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
    size_t created = zio::events_ctx::create_loop_pool();
    zlog("create loop {}",created);
    zio::event_loop_t* default_loop = zio::events_ctx::default_loop();
    test_loop_func();
    default_loop->run_loop();
}