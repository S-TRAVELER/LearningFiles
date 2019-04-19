#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

class MyTimer{
public:
    void run(){
        while(flag){
            cout<<"hello!"<<endl;
        }
    }
    void shutdown(){
        flag=false;
    }
private:
    bool flag=true;
};

int main()
{
      MyTimer tim;
      std::vector<std::thread> threads;
      threads.push_back(std::thread(&MyTimer::run,&tim));
      usleep(1000);
      tim.shutdown();
      for(auto &t:threads){
         t.join();
      }
      return 0;
}
