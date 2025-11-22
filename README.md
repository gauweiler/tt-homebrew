# tt-homebrew
tt-homebrew is a tool that makes it easy to write games for the TipToi in C, providing full access to all the features of the TipToi.

## disclaimer
tt-homebrew is not in anyway affiliated with Ravensburger. It's possible to brick your pen, so use this tool at your own risk. It never happened to me in all my testing, but you could call functions that overwrite the flash, making it unable to boot again. 

## getting started
Install `nodejs` and `gcc-arm-none-eabi` or just use `nix-shell`.  
Then run `npm i` to install the dependencies.  
Get the syntax with `node build.js -h`.

With `node build.js -i gme/Wimmelbuch.gme -o "/run/media/$(whoami)/tiptoi/Wimmelbuch.gme" -n dump_ram -p -b 0` you can compile your first example, which dumps ram from the tiptoi to its storage, as soon you try to start the first game. You can find it by connecting your pen to your pc and looking in the "dump" folder.



## roadmap
If you want to contribute or just stay up to date, join the [tt-homebrew discord](https://discord.gg/bsRAuBnX3j)
- [x] Support for 2N and 3L
- [x] Microphone / CREATE support 
- [x] Automatic injection in binaries
- [ ] Create a main template, that makes scripts work
- [ ] Create files without tttool (coming soon, checkout gmelib)
- [ ] list files and directorys
- [x] Uart
- [ ] Wifi support 
- [ ] add firmware offsets with functions that arent accessible via the sys_api


## knowledge
- On 2N and 3L the system_api seems to be compatible, but different ``fpAkOidPara``
- The main binary handels the execution of the scripts inside the gme. If the offset to the main binary table is 0, the pen uses a main binary thats inside the firmware.
- Reading from unmaped memory on 4E (0x80 9000 < crash < 0x82 9000)

| Processor     | 1             | 2N            | 3L and later (tested on 4E, other firmwares are different)     
| ------------- | ------------- | ------------- | ------------- |
| Bootrom       | tbd           | 0x0000 0000   | 0x0000 0000?  |
| Bios.bin      | tbd           | 0x0800 0000   | 0x0080 0000   |
| Prog.bin      | tbd           | 0x0800 9000   | 0x0082 9000
| Prog.bin entry| tbd           | 0x0800 9100   | 0x0085 ed01 the 1 at the end means thumb mode / only for 4E / differs on 5E and 6E
| GME bins      | tbd           | 0x0813 2000   | 0x0093 0000
| GME Gamestate | tbd           | 0x0814 1000   | 0x0094 0000
| alloc stuff   | tbd           | 0x?           | 0x0095 0000
| Return Game   | tbd           |               | 0x0082 c3c5? looks wrong
| Return Main   | tbd           | 
| malloc        | tbd           | 0x08142000
| malloc len    | tbd           | 0x00098000

sysparam at 0x00805E50
fpAkOidPara at 0x00950780
filehandle 0x00958640

> i dont have a pen with first gen processor, feel free to test it on your pen


## little ghidra guide
Want to decompile binaries from gme files? This is my routine:
- Use `tttool binaries` to get the binaries.
- Use `tttool export` to see which game is started when you press the game button.
- For 2N use `2_Game` and for 3L (3L, 4E, 5E, 6E) use `6_Game` plus the number from the yaml.
- Drag and drop this file into ghidra and select `v5t little endian` as the language.
- Click on `Options` and enter the `GME bins` address from the table as the base address.
- Open the file and go to the start and press `d`. This will decompile the first instruction. This will be a jump to the main function.
- Declare the main function with `f` und rename it to `main`
- Go to file -> parse c source and import the `lib/api.h`
- Now declare the first argument from your main function as this struct
- Click on the ram symbol and add a memory region for the game state. Make this region rw and the binary rx.
- then run the auto-analysis 
- If you have done everything right, you should now have a project with api names, correct offsets and no red addresses

## Note
Binarys can only be 65kb (0xffff) of size on 3L and 61kb (0xf000) on 2N
todo: check if it could be larger since the gmefile has 32bit size