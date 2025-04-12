#ifndef THREADBASE_H
#define THREADBASE_H

#include <atomic>
#include <thread>

class ThreadBase {
public:
    ThreadBase() : running{false} {};
    virtual ~ThreadBase();

    void start();
    void stop();
protected:
    std::atomic_bool running;
    virtual void run() = 0;
private:
    std::thread t;
};

#endif //THREADBASE_H
