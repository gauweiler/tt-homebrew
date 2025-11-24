# tt-homebrew

A powerful toolchain for developing custom games and applications for the Ravensburger TipToi in C, providing full access to all hardware features.

## Features

- 🎮 Write custom games and applications in C
- 🔧 Full hardware access (LED, microphone, UART, WiFi on supported models)
- 📦 Support for TipToi 2N, 3L, 4E, 5E, and 6E models
- 🛠️ Built-in binary injection and patching
- 📚 Comprehensive API for system functions
- 🎯 Multiple example projects included

## Disclaimer

**⚠️ Important:** tt-homebrew is not affiliated with Ravensburger. While extensive testing has been performed, using this tool carries risk. Incorrect usage could potentially brick your pen by overwriting critical flash memory. Use at your own risk. 

## Prerequisites

You'll need the following installed:

- **Node.js** (v14 or higher)
- **gcc-arm-none-eabi** (ARM cross-compiler)

**Quick setup with Nix:** If you have Nix installed, simply run `nix-shell` to get a complete environment.

## Getting Started

### Installation

```bash
# Install dependencies
npm install
```

### Basic Usage

```bash
# Display help and all available options
node build.js -h
```

### Your First Example

Compile the `dump_ram` example, which dumps TipToi RAM to storage when you start the first game:

```bash
node build.js \
  -i gme/Wimmelbuch.gme \
  -o "/run/media/$(whoami)/tiptoi/Wimmelbuch.gme" \
  -n dump_ram \
  -p \
  -b 0
```

After running this, connect your TipToi to your PC and check the "dump" folder on the device.

## Command-Line Options

- `-i` - Input .gme file
- `-o` - Output .gme file path
- `-n` - Binary name to build (e.g., dump_ram, play_sounds)
- `-l` - List games inside the input .gme file
- `-p` - Patch mode: faster than full write (requires `dd` command)
- `-d` - Output dd commands for manual execution
- `-s` - New start OID
- `-b` - Binary to overwrite (number for game, or M for main binary)

## Example Projects

The `binaries/` directory contains several example projects:

- **dump_ram** - Dump RAM contents to storage
- **blink** - Control the LED
- **play_sounds** - Audio playback example
- **uart** - UART communication
- **mp3_player** - Custom MP3 player
- **wifi** - WiFi functionality (supported models)
- And many more...



## Roadmap

Want to contribute or stay up to date? Join the [tt-homebrew Discord](https://discord.gg/bsRAuBnX3j)!

- [x] Support for 2N and 3L
- [x] Microphone / CREATE support
- [x] Automatic injection in binaries
- [x] UART support
- [ ] Create a main template that makes scripts work
- [ ] Create files without tttool (coming soon, check out gmelib)
- [ ] List files and directories
- [ ] WiFi support
- [ ] Add firmware offsets with functions that aren't accessible via the sys_api


## Technical Documentation

### Memory Map

**Key Information:**
- On 2N and 3L, the system_api appears to be compatible, but with different `fpAkOidPara` addresses
- The main binary handles the execution of scripts inside the GME file
- If the offset to the main binary table is 0, the pen uses a main binary from the firmware
- Reading from unmapped memory on 4E crashes between 0x80 9000 and 0x82 9000

| Memory Region      | Gen 1 | 2N            | 3L and later (tested on 4E) |
| ------------------ | ----- | ------------- | ---------------------------- |
| Bootrom            | TBD   | 0x0000 0000   | 0x0000 0000?                 |
| Bios.bin           | TBD   | 0x0800 0000   | 0x0080 0000                  |
| Prog.bin           | TBD   | 0x0800 9000   | 0x0082 9000                  |
| Prog.bin entry     | TBD   | 0x0800 9100   | 0x0085 ed01 (thumb mode)     |
| GME bins           | TBD   | 0x0813 2000   | 0x0093 0000                  |
| GME Gamestate      | TBD   | 0x0814 1000   | 0x0094 0000                  |
| Alloc stuff        | TBD   | ?             | 0x0095 0000                  |
| Return Game        | TBD   | —             | 0x0082 c3c5?                 |
| Return Main        | TBD   | —             | —                            |
| malloc             | TBD   | 0x0814 2000   | —                            |
| malloc len         | TBD   | 0x0009 8000   | —                            |

**Additional Addresses:**
- sysparam: 0x00805E50
- fpAkOidPara: 0x00950780
- filehandle: 0x00958640

> **Note:** Testing on first-generation processors is welcome. If you have a Gen 1 pen, please contribute test results!

### Binary Size Limits

- **3L and later:** Maximum 65KB (0xFFFF)
- **2N:** Maximum 61KB (0xF000)

> TODO: Investigate if larger sizes are possible since GME files use 32-bit size fields

### Ghidra Decompilation Guide

Want to decompile binaries from GME files? Follow this workflow:

1. **Extract binaries**
   ```bash
   tttool binaries <your-file.gme>
   ```

2. **Identify the target game**
   ```bash
   tttool export <your-file.gme>
   ```
   - For 2N: Look for `2_Game` + the number from the YAML
   - For 3L/4E/5E/6E: Look for `6_Game` + the number from the YAML

3. **Import into Ghidra**
   - Drag and drop the binary file into Ghidra
   - Select **`v5t little endian`** as the language
   - Click **Options** and enter the `GME bins` address from the memory map table as the base address

4. **Set up the entry point**
   - Open the file and navigate to the start address
   - Press `d` to decompile the first instruction (typically a jump to main)
   - Press `f` to declare it as a function
   - Rename the function to `main`

5. **Import API definitions**
   - Go to **File → Parse C Source**
   - Import `lib/api.h`
   - Declare the first argument of your main function as this struct

6. **Configure memory regions**
   - Click the RAM symbol
   - Add a memory region for the game state
   - Set the binary region as **RX** (read-execute)
   - Set the game state region as **RW** (read-write)

7. **Run analysis**
   - Run the auto-analysis
   - If configured correctly, you should see:
     - API function names properly resolved
     - Correct memory offsets
     - No invalid/red addresses

## Contributing

Contributions are welcome! Whether you have:
- Bug fixes
- New examples
- Documentation improvements
- Hardware testing results

Please join our [Discord community](https://discord.gg/bsRAuBnX3j) to discuss your contributions or open an issue/PR on GitHub.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to the TipToi reverse engineering community
- Special thanks to all contributors and testers