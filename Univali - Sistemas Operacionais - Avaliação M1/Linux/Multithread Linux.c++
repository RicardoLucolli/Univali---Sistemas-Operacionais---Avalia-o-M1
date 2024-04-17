//• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada 2 segundos pelo sensor.
//• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 1 segundo pelo sensor.


#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>

using namespace std;

int p1 = 0, p1_kg = 0;
int p2 = 0, p2_kg = 0;

void *esteira1(void *param) {
    while (1) {
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(2));
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        p1 ++;
        p1_kg += 5;
        auto end = std::chrono::steady_clock::now();
        cout << "\n(" << (end - start).count() << " nanoseconds)\n";
        cout << "esteira1: " << p1 << " produtos (" << p1_kg << "kg)" << endl;
        cout << "esteira2: " << p2 << " produtos (" << p2_kg << "kg)" << endl;
    }
}

void *esteira2(void *param) {
    while (1) {

        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(1));
        p2++;
        p2_kg += 2;
    }
}

int main()
{
    pthread_t tid_esteira1, tid_esteira2;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
  
    pthread_create(&tid_esteira1, &attr, esteira1, NULL);
    pthread_create(&tid_esteira2, &attr, esteira2, NULL);

    pthread_join(tid_esteira1, NULL);
    pthread_join(tid_esteira2, NULL);
    return 0;
}