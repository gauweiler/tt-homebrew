all:
	echo placeholder > build/2N.bin
	arm-none-eabi-gcc -I ../../lib -T build/3L.ld -nostdlib -fno-builtin -fshort-wchar startup.s main.c -o build/3L.out
	arm-none-eabi-objcopy -O binary build/3L.out build/3L_wrapper.bin
	node ../../inject.js -u build/6_Game10.b -i build/3L_wrapper.bin -o build/3L.bin