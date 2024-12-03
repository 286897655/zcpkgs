#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

std::mutex mutex_;
int sum = 0;
int count = 10;
void worker1(){
    for(int i = 0; i < count; i++){
        std::lock_guard<std::mutex> lock(mutex_);
        sum += i;
        std::cout << "work1 sum:"<< sum << std::endl;
    }
}

void worker2(){
    for(int i = 0; i < count; i++){
        std::lock_guard<std::mutex> lock(mutex_);
        sum += i;
        std::cout << "work2 sum:"<< sum << std::endl;
    }
}

void lock_mutex_test(){
    std::thread tr1([](){
        worker1();
    });
    std::thread tr2([](){
        worker2();
    });
    tr1.join();
    tr2.join();
}

std::condition_variable cv_;
std::mutex mtx_;

void worker3(){
    std::cout << "worker3 waitting" << std::endl;
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock);
    std::cout << "worker3 wake up" << std::endl;
}

void worker4(){
    std::cout << "worker4 notifying" << std::endl;
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.notify_one();
    std::cout << "worker4 notifyed" << std::endl;
}

void cond_var_test(){
    std::thread tr1([](){
        worker3();
    });
    std::thread tr2([](){
        worker4();
    });
    tr1.join();
    tr2.join();
}
int main(int argc,char** argv){
    // lock_mutex_test();
    cond_var_test();
}