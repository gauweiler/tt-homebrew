import { GmeFile } from "gmelib";
import { readFileSync, writeFileSync } from "fs";
import { execSync } from "child_process";
import minimist from "minimist";

const argv = minimist(process.argv.splice(2))

const printHelp = () => {
    console.log(`-i inputfile
        -b game number or m for main
        -n name of the hook`)
    process.exit(1)
}

if (typeof argv.i !== "string") {
    printHelp()
}

const hookBinary = (file, hook_bin) => {
    if (file.readUint8(3) != 0xea) {
        console.error("first instruction is not a jump")
    }

    const instuctionsToMain = file.readUint32LE() & 0x00ffffff

    console.log(`Main function is located at offset 0x${(instuctionsToMain * 4 + 8).toString(16)}`)

    if (file.readUint32BE(4) == 0x4150505f) { // string APP_
        console.log("found APP_DEMO string, placing jump to main there")
        file.writeUInt32LE(instuctionsToMain - 1, 4)
        file.writeUInt8(0xea, 7)
    } else {
        console.warn("warning: there is no place for a jump to old main, you have to directly jump to main")
    }

    if (file.length % 4) {
        console.error("binary is not 4 byte aligned")
    }
    console.log(`the original file length is 0x${file.length.toString(16)}. Put it in your linker script as ORIGIN`)

    const instuctionsToSkip = (file.length / 4) - 2

    file.writeUInt16LE(instuctionsToSkip, 0)

    console.log(`Adding 0x${hook_bin.length.toString(16)} bytes`)

    const out = Buffer.concat([file, hook_bin])
    // todo: add file to inject here
    console.log(`output file is 0x${out.length.toString(16)}`)
    return out
}

// gme wird auseinandergebaut, binary gelesen,
const gmefile = new GmeFile(readFileSync(argv.i))

const binary = gmefile.gmeFileBuffer.slice(gmefile.game3LbinariesTable[argv.b].offset, gmefile.game3LbinariesTable[argv.b].offset + gmefile.game3LbinariesTable[argv.b].size)

// offsets in link script geschrieben,
writeFileSync("lib/2N_hook.ld", readFileSync("lib/2N.ld").toString().replace("08132000", (0x08132000 + binary.length).toString(16)))
writeFileSync("lib/3L_hook.ld", readFileSync("lib/3L.ld").toString().replace("00930000", (0x00930000 + binary.length).toString(16)))


// make wird ausgeführt
execSync("make", { cwd: `hooks/${argv.n}` }).toString()

// binary wird an ende der orginal binary copiert, jump wird geändert
const new_bin = hookBinary(binary, readFileSync(`hooks/${argv.n}/build/3L.bin`))

// (inject.js) binary wird per changeBinary() reingepackt, da größere größe IMP:
gmefile.replaceBinary(new_bin, gmefile.game3LbinariesTable, gmefile.game3LbinariesTableOffset, argv.b)

writeFileSync(argv.o ?? `hooked_${argv.i}`, gmefile.gmeFileBuffer)
// todo: size check