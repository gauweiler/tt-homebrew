import { readFileSync, writeFileSync } from "fs";
import minimist from "minimist"

const argv = minimist(process.argv.splice(2))

if (!argv.u || !argv.i || !argv.o) {
    console.log(`syntax:
        -u unpatched orginal binary
        -i binary that should wrap the binary
        -o outputfile
        `)
        process.exit(1)
}

const file = readFileSync(argv.u)
if (file.readUint8(3) != 0xea) {
    console.error("first instruction is not a jump")
}

const instuctionsToMain = file.readUint32LE() & 0x00ffffff

console.log(`Main function is located at offset 0x${(instuctionsToMain*4+8).toString(16)}`)

if (file.readUint32BE(4) == 0x4150505f) { // string APP_
    console.log("found APP_DEMO string, placing jump to main there")
    file.writeUInt32LE(instuctionsToMain-1, 4)
    file.writeUInt8(0xea, 7)
}

if (file.length % 4) {
    console.error("binary is not 4 byte aligned")
}
console.log(`the original file length is 0x${file.length.toString(16)}. Put it in your linker script as ORIGIN`)

const instuctionsToSkip = (file.length / 4) - 2

file.writeUInt16LE(instuctionsToSkip, 0)

const wrapper = readFileSync(argv.i)

console.log(`Adding 0x${wrapper.length.toString(16)} bytes`)

const out = Buffer.concat([file, wrapper])
// todo: add file to inject here
console.log(`output file is 0x${out.length.toString(16)}`)

writeFileSync(argv.o, out)