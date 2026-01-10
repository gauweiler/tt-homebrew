all:
	arm-none-eabi-gcc -Dbuild_for_2N -I ../../lib -T ../../lib/2N.ld -nostdlib -fno-builtin -fshort-wchar startup.s main.c -o build/2N.out
	arm-none-eabi-objcopy -O binary build/2N.out build/2N.bin
	arm-none-eabi-gcc -I ../../lib -T ../../lib/3L.ld -nostdlib -fno-builtin -fshort-wchar startup.s main.c -o build/3L.out
	arm-none-eabi-objcopy -O binary build/3L.out build/3L.bin