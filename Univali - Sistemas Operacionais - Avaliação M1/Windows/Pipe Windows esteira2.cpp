
//• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada 2 segundos pelo sensor.
//• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 1 segundo pelo sensor.


#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>

#define BUFFER_SIZE 1024

using namespace std;

void esteira2() {
    HANDLE hPipe;
    char buffer[1024];
    char sub_buffer1[512], sub_buffer2[512];
    DWORD dwWritten;
    int p2 = 0, p2_kg = 0;

    // Open named pipe
    hPipe = CreateFile("\\\\.\\pipe\\pipeso",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em abrir o pipe. Codigo do erro: %d\n", GetLastError());
        return;
    }

    while (1) {
        // Send data to server
        //fgets(buffer, BUFFER_SIZE, stdin);

        std::sprintf(buffer, "%d", p2_kg);

        if (!WriteFile(hPipe, buffer, strlen(buffer) + 1, &dwWritten, NULL))
        {
            printf("Falha em escrever no pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return;
        }

        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(1));
        p2++;
        p2_kg += 2;
    }
}

int main()
{
    esteira2();
    return 0;
}