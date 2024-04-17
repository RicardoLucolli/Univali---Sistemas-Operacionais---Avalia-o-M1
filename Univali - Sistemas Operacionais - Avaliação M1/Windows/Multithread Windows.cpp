
//• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada 2 segundos pelo sensor.
//• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 1 segundo pelo sensor.


#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int p1 = 0, p1_kg = 0;
int p2 = 0, p2_kg = 0;

void esteira1() {
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

void esteira2() {
    while (1) {

        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(1));
        p2++;
        p2_kg += 2;
    }
}

int main()
{

    thread _esteira1(esteira1);
    thread _esteira2(esteira2);

    _esteira1.join();
    _esteira2.join();
    return 0;
}