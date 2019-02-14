#include <iostream>
#include "multithread_frame.h"

using namespace std;

class SampleData
{
public:
    int seq;
    int ms;
};

int main()
{
    MultiThreadFrame<SampleData> mtf;
    std::mutex mtx;

    auto pfunc = [&mtx](const SampleData &d){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::unique_lock<std::mutex> lock_(mtx);
        std::cout << "thread " << std::this_thread::get_id() << " do work " << d.seq << " " <<
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
    };
    mtf.Start(3, pfunc);

    for(int i=0; i<100; i++)
    {
        SampleData d;
        d.seq = i;
        mtf.Push(d);
    }

    int x;
    std::cin >> x;

    return 0;
}
