#include "ThreadBase.h"

ThreadBase::~ThreadBase()
{
    stop();
}

void ThreadBase::start()
{
    running = true;
    t = std::thread(&ThreadBase::run, this);
};

void ThreadBase::stop() {
    running = false;
    if (t.joinable()) {
        t.join();
    }
}
