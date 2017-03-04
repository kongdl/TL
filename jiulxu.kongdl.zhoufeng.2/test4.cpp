#include <iostream>
#include <cstdlib>
#include "thread.h"
#ifdef DEBUG
#define _(x) x
#else
#define _(x)
#endif
#ifdef ASSERTENABLE
#include <cassert>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif
using namespace std;
cv cv1,cv2;
mutex mutex1;
int cnt = 0;

void consumer(void *a)
{
	intptr_t arg = (intptr_t) a;
	mutex1.lock();
	while(cnt == 0){
		cv1.wait(mutex1);
	}
	cnt--;
	cout << "consumer "<< arg <<" consumed! cnt = " << cnt <<endl;
	cv2.signal();
	mutex1.unlock();
}

void producer(void *a)
{
	mutex1.lock();
	while(1){
		while(cnt == 5){
			cv2.wait(mutex1);
		}
		cnt++;
		cout << "produced! cnt = " << cnt << endl;
		cv1.signal();
	}
	mutex1.unlock();
}
void parent(void *a)
{
    intptr_t arg = (intptr_t) a;
    cout << "parent called with arg " << arg << endl;
    for(int i=0;i<10000;i++){

    	thread t1 ( (thread_startfunc_t) consumer, (void *)i);
    	cout << i << endl;
    }
    thread t1 ( (thread_startfunc_t) producer, (void *) "producer thread 1");
    cout << "parent finish" << endl;
}
int main() {
	_(printf("start!"));
    cpu::boot(10, (thread_startfunc_t) parent, (void *) 100, true, true, 0);

}
