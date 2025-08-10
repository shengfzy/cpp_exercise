#include <iostream>
#include <fstream>
#include <thread>
#include <exception>

class DivByZero : public std::exception {
public:
    const char *what() const noexcept {
        return "data divided by zero";
    }
};

class calculator {
public:
    float divide(float x, float y) {
        if(y == 0) {
            throw DivByZero();
        }
        else {
            return x / y;
        }
    }
};

void worker_thread() {
    std::ofstream outfile;
    char *data = new char[100];

    outfile.open("./outfile.txt", std::ios::out | std::ios::trunc);

    std::cout << "put your name:" << std::endl;
    std::cin.getline(data, 100);

    outfile << data << std::endl;

    outfile.close();

    std::ifstream infile;
    infile.open("./outfile.txt", std::ios::in);

    infile >> data;
    std::cout << data << std::endl;


    calculator *c = new calculator();

    try {
        std::cout << "5 / 2 = " << c->divide(5, 2) << std::endl;
        std::cout << "5 / 0 = " << c->divide(5, 0) << std::endl;
    }catch(DivByZero &e) {
        std::cout << e.what() << std::endl;
    }catch(...) {
        std::cout << "unknown excpetion" << std::endl;
    }

    return;
}

int main(int argc, char *argv[]) {

    std::thread t1(worker_thread);
    t1.join();

    return 0;
}
