import { GmeFile } from "gmelib"
import { existsSync, readFileSync, writeFileSync } from "fs"
import child from "child_process"
import minimist from "minimist"
const argv = minimist(process.argv.splice(2))

const printHelp = () => {
    console.log(`        tt-homebrew buildscript
        usage:
        -i      .gme file as input
        -o      .gme file output
        -n      the name of the binary that should be build, for example dump_ram or play_sounds
        -l      list games inside the input .gme file
        -p      patch the output file insadt of writing it new, its faster, especially if you output on the pen direct | note: only works if you have dd installed
        -d      output dd commands, that you can use instad of this script
        -b      which binary inside the .gme file should be overwritten
                write a number to overwrite the corrosponding game or write M to overwrite the main binary `)
    process.exit(1)
}

if (argv.h || argv.help) {
    printHelp();
}

if (!argv.i) {
    console.error("Error: Specify an input file with -i input.gme")
    printHelp();
}

const gmefile = new GmeFile(readFileSync(argv.i))

if (argv.l || argv.b > gmefile.game1binariesTable.length - 1 || (typeof argv.b === "string" && argv.b.toString().toLowerCase() !== "m")) {
    console.log(`Valid games are 0-${gmefile.game1binariesTable.length - 1} or M for the main binary`)
    process.exit(1)
}

if (argv.d) {
    if (argv.b.toString().toLowerCase() == "m") {
        console.log(`dd if=main_1.bin of=${argv.o ?? "output.gme"} bs=1 seek=${gmefile.main1binaryTable[0].offset} count=${gmefile.main1binaryTable[0].size} conv=notrunc`)
        console.log(`dd if=main_2N.bin of=${argv.o ?? "output.gme"}  bs=1 seek=${gmefile.main2NbinaryTable[0].offset} count=${gmefile.main2NbinaryTable[0].size} conv=notrunc`)
        console.log(`dd if=main_3L.bin of=${argv.o ?? "output.gme"}  bs=1 seek=${gmefile.main3LbinaryTable[0].offset} count=${gmefile.main3LbinaryTable[0].size} conv=notrunc`)
    } else {
        console.log(`dd if=game_1.bin of=${argv.o ?? "output.gme"}  bs=1 seek=${gmefile.game1binariesTable[argv.b].offset} count=${gmefile.game1binariesTable[argv.b].size} conv=notrunc`)
        console.log(`dd if=game_2N.bin of=${argv.o ?? "output.gme"}  bs=1 seek=${gmefile.game2NbinariesTable[argv.b].offset} count=${gmefile.game2NbinariesTable[argv.b].size} conv=notrunc`)
        console.log(`dd if=game_3L.bin of=${argv.o ?? "output.gme"}  bs=1 seek=${gmefile.game3LbinariesTable[argv.b].offset} count=${gmefile.game3LbinariesTable[argv.b].size} conv=notrunc`)
    }
    process.exit()
}
if (!argv.n) {
    console.error("Error: Use -n to specify which binary to build, for example 'dump_ram'.")
    printHelp()
}

if (!argv.o) {
    console.error("Error: Specify an output file with -o output.gme")
    printHelp()
}

if (!existsSync(`binaries/${argv.n}`)) {
    console.error(`Error: No binary found with the name "${argv.n}" in /binaries`)
    process.exit(1)
}

console.log(child.execSync("make", { cwd: `binaries/${argv.n}` }).toString())

if (argv.p) {
    if (!existsSync(argv.o)) {
        console.error(`Error: Output file "${argv.o}" does not exist. Cannot patch with dd.`)
        process.exit(1)
    }
    // todo: check output binsize
    // patch the output with dd (its faster then writing the whole file to the pen)
    if (argv.b.toString().toLowerCase() == "m") {
        console.log(`replacing ${gmefile.main2NbinaryTable[0].filename} with dd`)
        child.execSync(`dd "if=binaries/${argv.n}/build/2N.bin" "of=${argv.o}" bs=1 seek=${gmefile.main2NbinaryTable[0].offset} count=${gmefile.main2NbinaryTable[0].size} conv=notrunc`)
        child.execSync(`dd "if=binaries/${argv.n}/build/3L.bin" "of=${argv.o}" bs=1 seek=${gmefile.main3LbinaryTable[0].offset} count=${gmefile.main3LbinaryTable[0].size} conv=notrunc`)
    } else {
        console.log(`replacing ${gmefile.game2NbinariesTable[argv.b].filename} with dd`)
        child.execSync(`dd "if=binaries/${argv.n}/build/2N.bin" "of=${argv.o}" bs=1 seek=${gmefile.game2NbinariesTable[argv.b].offset} count=${gmefile.game2NbinariesTable[argv.b].size} conv=notrunc`)
        child.execSync(`dd "if=binaries/${argv.n}/build/3L.bin" "of=${argv.o}" bs=1 seek=${gmefile.game3LbinariesTable[argv.b].offset} count=${gmefile.game3LbinariesTable[argv.b].size} conv=notrunc`)
    }
} else {
    // modify the file with gmelib
    if (argv.b.toString().toLowerCase() == "m") {
        console.log(`replacing ${gmefile.main2NbinaryTable[0].filename} (main) with gmelib`)
        gmefile.replaceBinary(readFileSync(`binaries/${argv.n}/build/2N.bin`), gmefile.main2NbinaryTable, gmefile.main2NbinaryTableOffset, 0)
        gmefile.replaceBinary(readFileSync(`binaries/${argv.n}/build/3L.bin`), gmefile.main3LbinaryTable, gmefile.main3LbinaryTableOffset, 0)
        writeFileSync(argv.o, gmefile.gmeFileBuffer)
    } else {
        console.log(`replacing ${gmefile.game2NbinariesTable[argv.b].filename} (game) with gmelib`)
        gmefile.replaceBinary(readFileSync(`binaries/${argv.n}/build/2N.bin`), gmefile.game2NbinariesTable, gmefile.game2NbinariesTableOffset, argv.b)
        gmefile.replaceBinary(readFileSync(`binaries/${argv.n}/build/3L.bin`), gmefile.game3LbinariesTable, gmefile.game3LbinariesTableOffset, argv.b)
        writeFileSync(argv.o, gmefile.gmeFileBuffer)
    }
}

