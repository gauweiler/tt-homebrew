all:
	arm-none-eabi-gcc -Dbuild_for_2N -I ../../lib -T ../../lib/2N_sysmodule.ld -nostdlib -fno-builtin -fshort-wchar ../../lib/startup.s main.c -o build/2N.out
	arm-none-eabi-objcopy -O binary build/2N.out build/2N.bin
# 	arm-none-eabi-gcc -I ../../lib -T ../../lib/3L.ld -nostdlib -fno-builtin -fshort-wchar ../../lib/startup.s main.c -o build/3L.out
# 	arm-none-eabi-objcopy -O binary build/3L.out build/3L.bin

MOUNT_DIR=/run/media/$$(whoami)/tiptoi

flash:
	@$(MAKE) all
	@echo "Waiting for tiptoi at $(MOUNT_DIR)..."
	@while [ ! -d "$(MOUNT_DIR)" ]; do sleep 1; done
	cp build/2N.bin "$(MOUNT_DIR)/payload.bin"
	sync
	umount "$(MOUNT_DIR)" || true
	@echo "Done! Safe to unplug."