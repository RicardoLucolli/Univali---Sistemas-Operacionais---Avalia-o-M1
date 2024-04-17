
//• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada 2 segundos pelo sensor.
//• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 1 segundo pelo sensor.

#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>

#define BUFFER_SIZE 1024

using namespace std;
//using namespace std::literals;

void esteira1() {

    HANDLE hPipe;
    char buffer[BUFFER_SIZE];
    DWORD dwRead;
    int p1 = 0, p1_kg = 0;

    // Create named pipe
    hPipe = CreateNamedPipe(
        "\\\\.\\pipe\\pipeso",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        BUFFER_SIZE,
        BUFFER_SIZE,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em criar o pipe. Codigo do erro: %d\n", GetLastError());
        return;
    }

    // Wait for client to connect
    if (!ConnectNamedPipe(hPipe, NULL))
    {
        printf("Falha em conectar ao cliente. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return;
    }

    while (1) {
        // Read data from client

        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

        if (!ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL))
        {
            printf("Falha em ler do pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return;
        }

        auto end = std::chrono::steady_clock::now();

        cout << "esteira1:\t" << p1 << " produtos (" << p1_kg << ")\n";
        cout << "esteira2:\t" << atoi(buffer)/2 << " produtos (" << buffer << ")\n";
        cout << "(" << (end - start).count() << " nanoseconds)\n\n";
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(2));
        p1 ++;
        p1_kg += 5;
    }
}

int main()
{
    esteira1();
    return 0;
}