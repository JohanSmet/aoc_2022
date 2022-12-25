package main

import (
	"fmt"
	"io"
)

func main() {

	var elf_total int64 = 0
	max_energy := [3]int64{0, 0, 0}

	for {
		var calory int64
		var count, err = fmt.Scanf("%d", &calory)

		if (count == 1) {
			elf_total += calory
		} else {
			if elf_total > max_energy[0] {
				max_energy[2] = max_energy[1]
				max_energy[1] = max_energy[0]
				max_energy[0] = elf_total
			} else if elf_total > max_energy[1] {
				max_energy[2] = max_energy[1]
				max_energy[1] = elf_total
			} else if elf_total > max_energy[2] {
				max_energy[2] = elf_total
			}

			elf_total = 0
		}

		if err == io.EOF {
			break
		}
	}

	fmt.Printf("Part1: %d\n", max_energy[0])
	fmt.Printf("Part2: %d\n", max_energy[0] + max_energy[1] + max_energy[2])
}
