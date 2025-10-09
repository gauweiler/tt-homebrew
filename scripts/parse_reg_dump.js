import { readFileSync } from "fs"

const file = readFileSync(process.argv[2])


const check_change = (offset) => {
    for (let i = 0x200; i < file.length; i = i + 0x200) {
        const oldValue = file.readUint32LE(i + offset - 0x200)
        const newValue = file.readUint32LE(i + offset)
        if (oldValue != newValue) {
            console.log(`Value 0x${offset.toString(16)} changed from 0x${oldValue.toString(16)} to 0x${newValue.toString(16)} on block ${i / 0x200}`)
        }
    }
}

for (let i = 0; i < 0x200; i = i + 4) {
    check_change(i)
}