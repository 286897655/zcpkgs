#include <zio/io_poller.h>

int main(int argc,char** argv){
    size_t created_loop = zio::io_loop_t::create_loop_pool();
    zio::io_loop_t* default_loop = zio::io_loop_t::default_loop();
    default_loop->run();
}