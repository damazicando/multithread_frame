#ifndef _MULTI_THREAD_FRAME_H_
#define _MULTI_THREAD_FRAME_H_

#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include "thread_queue.h"
#include <algorithm>

template <typename T>
class MultiThreadFrame
{
public:
    void Start(int thread_num, std::function<void(const T &t)> func)
    {
        func_ = func;
        terminate_ = false;
        for(int i=0; i<thread_num; i++)
        {
            std::future<void> ftr = std::async(std::launch::async, &MultiThreadFrame::run, this);
            vftr_.push_back(std::move(ftr));
        }
    }

    void Stop()
    {
        terminate_ = true;
        for (auto &ftr : vftr_)
        {
            ftr.wait();
        }
    }

    void Push(const T &t)
    {
        mq_.push_back(t);
    }
private:
    void run()
    {
        while(!terminate_)
        {
            try
            {
                T t;
                if (mq_.pop_front(t, 500))
                {
                    func_(t);
                }
            }
            catch (std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
            }
            catch (...)
            {

            }
        }
    }
private:
    std::atomic_bool terminate_;
    ThreadQueue<T> mq_;
    std::function<void(const T &t)> func_;
    std::vector<std::future<void>> vftr_;
};

#endif // MULTI_THREAD_FRAME_H
