#include "api.h"

void (*orginal_binary)(system_api *) = (void *)(BINARY_OFFSET + 4);

#define NUMBER_OF_LOGS 1000

int counter;
int filehandle;
int isFileOpen;
system_api *sysapi_copy;

// project
// ziele: weg um töne zu definieren
// binary wird als main binary fürs erste reingepackt
// erstmal herausfinden was oid register sind

// when writing to the firmware, the pen crashes
// lets just overwrite the pointer to the printing function

int strlen(unsigned char *string) {
    int i = 0;
    while (string[i] != 0 && string[i] != '%') {
        i++;
    }
    return i;
}

int int_to_str(char *dst, unsigned int value) {
    if (value == 0) {
        dst[0] = '0';
        dst[1] = 0;
        return 1;
    }
    char tmp[11];
    int ti = 0;
    while (value) {
        tmp[ti++] = '0' + (value % 10);
        value /= 10;
    }
    int pos = 0;
    for (int j = ti - 1; j >= 0; --j) {
        dst[pos++] = tmp[j];
    }
    dst[pos] = '\0';
    return pos;
}

void hook(unsigned char *string) {
    unsigned int a, b, c;
    __asm__ volatile(
        "mov %0, r1\n"
        "mov %1, r2\n"
        "mov %2, r3\n"
        : "=r"(a), "=r"(b), "=r"(c));
    unsigned int args[3];
    int test;
    args[0] = a;
    args[1] = b;
    args[2] = c;
    int currentArg = 0;
    unsigned char buff[12];

    if (counter > NUMBER_OF_LOGS) {
        return;
    }
    counter++;
    if (counter == NUMBER_OF_LOGS + 1) {
        sysapi_copy->close(filehandle);
        return;
    }

    int i = 0;
    while (string[i] != 0) {
        int len = strlen(string + i);
        sysapi_copy->write(filehandle, string + i, len);
        i += len;
        if (string[i] == '%') {
            i++;
            if (string[i] == 'd' || string[i] == 'x') {
                int len = int_to_str(buff, args[currentArg]);
                // sysapi_copy->write(filehandle, "(d)", 3);
                sysapi_copy->write(filehandle, buff, len);
            } else if (string[i] == 'c') {
                // sysapi_copy->write(filehandle, "(c)", 3);
                sysapi_copy->write(filehandle, &args[currentArg], 1);
            } else {
                sysapi_copy->write(filehandle, string + i - 1, 2);
            }
            currentArg++;
            i++;
        }
    }
}

void main(system_api *api) {
    api->printf = hook;
    sysapi_copy = api;
    char buff[6];
    if (api->fpAkOidPara[First_time_exec] == 0) {
        counter = 0;
        filehandle = api->open(L"B:/dump/print", 1, 1);
        isFileOpen = 1;
    }
    // if (api->fpAkOidPara[6] == 100) {
    //     api->write(filehandle, "hook read oid: ", 15);
    //     int len = int_to_str(buff, api->fpAkOidPara[0x10]);
    //     api->write(filehandle, buff, len);
    //     api->write(filehandle, "\n", 1);
    // }
    orginal_binary(api);
}