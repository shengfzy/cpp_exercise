#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;

int cnt = 0;

std::condition_variable cv;
std::mutex mtx1;
bool ready = false;

int safeFuncWithLockGuard() {
    std::lock_guard<std::mutex> lk(mtx);
    std::cout << std::this_thread::get_id() << "cnt = " << cnt << std::endl;
    return cnt++;
}

class ClassThread {
public:
    void operator() (int args) const {
        std::unique_lock<std::mutex> lk_s(mtx1);
        cv.wait(lk_s, [](){return ready;});
        lk_s.unlock();
        std::cout << "Class Thread Start" << std::endl;
        while(1) {
            (void) safeFuncWithLockGuard();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        return;
    }
};

void workthread(int args) {
    std::unique_lock<std::mutex> lk_s(mtx1);
    cv.wait(lk_s, []{return ready;});
    lk_s.unlock();
    std::cout << "Work Thread Start" << std::endl;
    while(1) {
        (void) safeFuncWithLockGuard();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return;
}

int main(int argc, char **argv) {

    std::thread t1(ClassThread(), 1);
    std::thread t2(workthread, 2);
    std::thread t3([](int args){
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::cout << "lambada Thread Start" << std::endl;
        std::unique_lock<std::mutex> lk_s(mtx1);
        ready = true;
        cv.notify_all();
        lk_s.unlock();
        while(1) {
            (void) safeFuncWithLockGuard();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        return;
    }, 3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
