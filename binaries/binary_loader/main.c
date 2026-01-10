#include "api.h"
#include "sdk.c"
#include "uart.c"

// api:    83ffdbc

// SAVEDATA8141000
// malloc: 814f830
// malloc: 815f830
// malloc: 816f830
// malloc: 817f830
// malloc: 818f830
// malloc: 819f830
// malloc: 81af830
// malloc: 81bf830

// api: 83ffdbc
// malloc: 8150340 big
// malloc: 8143010 smal
// malloc: 8160340 big
// malloc: 8143020 smal
// malloc: 8170340 e
// malloc: 8143030
// malloc: 8180340 e
// malloc: 8143040
// malloc: 8190340 e
// malloc: 8143050
// malloc: 81a0340 e
// malloc: 8143060
// malloc: 81b0340 e
// malloc: 8143070
// malloc: 81c0340 e
// malloc: 8143080
// malloc: 0
// malloc: 8143090

// malloc: 0
// malloc: 81d12a0
// malloc: 0

#define target 0x08160000
#define size_for_binary 0x10000

void (*sysmodule)() = (void*)target;

void main(system_api* apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        initPlaySoundFifo();
        // put your init code here
        init_uart();
        uart_printf("api: %x\r\n", (int)apiPara);
        int current_malloc_offset = (int)api->malloc(0x10);
        uart_printf("malloc: %x\r\n", current_malloc_offset);

        if (current_malloc_offset > target) {
            uart_printf("malloc to hight, cannot get adress\r\n");
            return;
        }

        int bytes_to_alloc = target - current_malloc_offset - 0x10;
        uart_printf("allocating %d bytes\r\n", bytes_to_alloc);
        int temp_buf = (int)api->malloc(bytes_to_alloc);

        int pBinary = (int)api->malloc(size_for_binary);
        uart_printf("pBinary: %x\r\n", pBinary);
        api->free(temp_buf);
        api->free(current_malloc_offset);
        if (pBinary != target) {
            api->play_chomp_voice(3);
            uart_printf("wrong adressr\r\n");
            return;
        }
        uart_printf("got buffer at right adress with size %x\r\n",
                    size_for_binary);

        uart_printf("alloc %x",
                    api->malloc(0x100));  // test if ram before is still used

        int file = api->open(L"B:/payload.bin", 0, 0);
        int read_bytes = api->read(file, (void*)pBinary, size_for_binary);
        uart_printf("read_bytes %x %d \r\n", read_bytes, read_bytes);

        int(*task) = (int*)0x08009890;

        #define sysmodule_addr 0xEA0559DA
        // void (*sysmodule)() = (void*)sysmodule_addr;
        *task = sysmodule_addr;
        // sysmodule();

        // 2n
        // uart_printf("fpAkOid %x\r\n", api->fpAkOidPara[0x74]); // 0
        // uart_printf("fpAkOid %x\r\n", api->fpAkOidPara[0x4a8]); // 0
        // uart_printf("fpAkOid %d\r\n", *api->tbd68); // 0
        // uart_printf("fpAkOid %x\r\n", api->fpAkOidPara[0xdec]); // 0xff / -1

        // api->fpAkOidPara[0x74] = 0;
        // api->fpAkOidPara[0x4a8] = 0;
        // *api->tbd68 = 0;
        api->fpAkOidPara[First_time_exec] = 0xff;

        // 3L
        // uart_printf("fpAkOid %x", api->fpAkOidPara[0x10]);
        // uart_printf("fpAkOid %x", api->fpAkOidPara[0x9e0]);
        // uart_printf("fpAkOid %x", api->fpAkOidPara[0x144]);
        // uart_printf("fpAkOid %x", api->fpAkOidPara[0x1318]);
    }
    // put your loop code here

    handlePlaySound();
}