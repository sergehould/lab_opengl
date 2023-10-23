#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
//#include<iostream>
//#include <strsafe.h>
//#include <tchar.h>

//using namespace std;
#pragma warning(disable : 4996)
// Choose the serial port name. 
// COM ports higher than COM9 need the \\.\ prefix, which is written as
// "\\\\.\\" in C because we need to escape the backslashes.
char buffer[100], buffer2[100];
unsigned char readBuf[1];
int rx1, rx2, _range, center, status, i = 0, j = 0, k = 0, m = 0, bar;
DWORD nb_bytes;
struct timeval start;	/* starting time */
struct timeval end, boot_time;	/* booting time and ending time */
unsigned long e_usec, boot_sec, end_sec, current_ms;	/* elapsed microseconds */
//166x300

// Choose the baud rate (bits per second).  
uint32_t baud_rate = 115200;

static void print_error(const char* context);
static int write_port(HANDLE port, uint8_t* buffer, size_t size);
HANDLE port;

int serial_init(void) {

    wchar_t pszPortName[] = L"5"; //com port id
    wchar_t PortNo[20] = { 0 }; //contain friendly name
    printf_s("Enter the Com Port. (E.g. 5).\nTo find out the Com Port, open Device Manager and then look at the Ports tab.\n");
    wscanf_s(L"%s", pszPortName, (unsigned)_countof(pszPortName));
    swprintf_s(PortNo, 20, L"\\\\.\\COM%s", pszPortName);
    //sprintf(PortNo,"\\\\.\\COM%d", port_num);
    //printf("fffff");
    //system("pause");


    port = CreateFile(PortNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //port = CreateFile(L"\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (port == INVALID_HANDLE_VALUE)
    {
        //print_error(device);
        printf_s("\n Port can't be opened\n\n");
        //system("pause");
        return -1;
    }
    // Flush away any bytes previously read or written.
    BOOL success = FlushFileBuffers(port);
    if (!success)
    {
        // print_error("Failed to flush serial port");
        CloseHandle(port);
        return -1;
    }

    // Configure read and write operations to time out after 100 ms.
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    success = SetCommTimeouts(port, &timeouts);
    if (!success)
    {
        //print_error("Failed to set serial timeouts");
        CloseHandle(port);
        return -1;
    }

    // Set the baud rate and other options.
    DCB state = { 0 };
    state.DCBlength = sizeof(DCB);
    state.BaudRate = baud_rate;
    state.ByteSize = 8;
    state.Parity = NOPARITY;
    state.StopBits = ONESTOPBIT;
    success = SetCommState(port, &state);
    if (!success)
    {
        //print_error("Failed to set serial settings");
        CloseHandle(port);
        return -1;
    }
    if (port == INVALID_HANDLE_VALUE) { return -1; }
    return 0;
}

char serial_read(void)
{
        static unsigned char last = 0;
        int ch = 0;
        fflush(stdin);
        if (kbhit()) {//Check whether there is data in the buffer
            ch = getch();//Read the data in the buffer in the form of characters
        }
        status = ReadFile(port, readBuf, 1, &nb_bytes, NULL); // non-blocking
        //if (nb_bytes == 0) return 0xff; //empty
        if (nb_bytes == 0) return last; //empty
        else { 
            last = readBuf[0];
            return last;
        }
}

void serial_close(void) {
    CloseHandle(port);
}

/* Function that sends one byte to the serial port */
void send_one_int8(char tx) {
    uint8_t buff[1];
    static int cnt = 0;
    buff[0] = tx;
    //if (cnt++ == 10) {
        write_port(port, buff, 1);
        cnt = 0;
    //}
}

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
static int write_port(HANDLE port, uint8_t* buffer, size_t size)
{
    DWORD written;
    BOOL success = WriteFile(port, buffer, size, &written, NULL);
    if (!success)
    {
        print_error("Failed to write to port");
        return -1;
    }
    if (written != size)
    {
        print_error("Failed to write all bytes to port");
        return -1;
    }
    return 0;
}


static void print_error(const char* context)
{
    DWORD error_code = GetLastError();
    char buffer[256];
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL, error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buffer, sizeof(buffer), NULL);
    if (size == 0) { buffer[0] = 0; }
    fprintf(stderr, "%s: %s\n", context, buffer);
}


/*
 Blocking polling function that receives two bytes 
 from the serial port and merge them as an int16 
 using a start byte, a stop byte and checksum byte.
 Returns the received character if no error.
 Returns -1 if there is an error.
*/

#define START_BYTE 0x7E
#define STOP_BYTE 0x7F
int16_t rec_one_int16_b() {
    uint8_t byte;
    uint8_t start_byte = START_BYTE;
    uint8_t stop_byte = STOP_BYTE;
    uint8_t data_bytes[2];
    uint8_t checksum = start_byte + stop_byte;

    while ((byte = serial_read()) != start_byte) {
        // Wait for the start byte
    }

    for (int i = 0; i < 2; i++) {
        data_bytes[i] = serial_read();
        checksum += data_bytes[i];
    }
    byte= serial_read();
    if (byte != checksum) {
        // Checksum error
        return -1;
    }

    if (serial_read() != stop_byte) {
        // Stop byte error
        return -1;
    }
    return (data_bytes[0] << 8) | data_bytes[1];
}
