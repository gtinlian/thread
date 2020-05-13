#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <thread>
#include <string>
#include <signal.h>
#include <vector>
#include <time.h>



//通过中断信号处理的函数
void test(int x){
    //在这个函数构造对象
    printf("执行这个函数的id = %d\n",syscall(SYS_gettid));
    //fflush(stdout);
    return;
}


class th{
public:
    std::thread *th_wait;
    std::thread *t;
    int tid;


    th(std::thread *th_waite_):th_wait(th_waite_),tid(0){}
    int thread(){
        t = new std::thread([](int *tid,std::thread *th_waite){

            //中断
            struct sigaction sig;
            sig.sa_handler = test;
            sigaction(SIGUSR1,&sig,NULL);

            //初始化对象tid
            *tid = syscall(SYS_gettid);

            //进入堵塞状态
            th_waite->join();
            },&tid,th_wait);
        int x = 0;
        while(tid == 0){
            sleep(0.1);
        }
        //释放当前线程的堵塞状态
        t->detach();
    }

};

class thread_pool{
public:
    int thread_number;
    thread_pool(int thread_number_):thread_number(thread_number_){}
    std::thread *init_pool(){

    }
};

int main(){

    std::thread t([](){
        sleep(-1);
    });
    std::vector<th*> thread(10);
    for(int i = 0;i<=9;i++){
        thread[i] = new th(&t);
        thread[i]->thread();
        std::cout << "线程id=" << thread[i]->tid<< std::endl;
    }
    sleep(1);
    //中断触发任务
    while(true){
        for(int i = 0;i<=9;i++){
            //发送中断信号给每个进程
            kill(thread[i]->tid,SIGUSR1);
        }

        //检查每秒产生中断信号
       printf("%ld\n",time(NULL));


    }

    t.join();

}