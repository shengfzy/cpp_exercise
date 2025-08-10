#include <iostream>
#include <fstream>
#include <thread>
#include <exception>
#include <typeinfo>

template <class T>
class TypeHandler {
public:
    void typeout(T const &x) {
        std::cout << typeid(x).name() << x << std::endl;
    }
};

void worker_thread() {

    TypeHandler<int> *h1 = new TypeHandler<int>;
    h1->typeout(25);

    TypeHandler<float> *h2 = new TypeHandler<float>;
    h2->typeout(25.3);
    return;
}

int main(int argc, char *argv[]) {

    std::thread t1(worker_thread);
    t1.join();

    return 0;
}
