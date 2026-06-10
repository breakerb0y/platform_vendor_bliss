/*
 * COPYRIGHT (C) 2007-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

/*
 * Utility for Geode LX system with CS5536 companion chip
 * to dump every MSRs and the PIC configuration.
 *
 * Created by Zoltán Böszörményi <zboszormenyi@sicom.com>
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <pci/pci.h>

#include "cs5536.h"

int main (int argc, char **argv) {
	struct pci_access *pacc;
	int fd, i;
	uint32_t reg;
	uint64_t data;
	uint8_t ioval;

	pacc = pci_alloc();
	pci_init(pacc);
	pci_scan_bus(pacc);

	if (iopl(3)) {
		printf("NO ACCESS!! (must be run as ROOT)\n");
		pci_cleanup(pacc);
		return 0;
	}

	if (!cs5536(pacc->devices)) {
		printf("%s: CS5536 Not Detected\n", argv[0]);
		pci_cleanup(pacc);
		return 0;
	}

	fd = open("/dev/cpu/0/msr", O_RDONLY);
	if (fd < 0) {
		printf("%s: Cannot open /dev/cpu/0/msr: %s\n", argv[0], strerror(errno));
		pci_cleanup(pacc);
		return 1;
	}

	printf("Geode LX MSR values\n");

	for (i = 0; i < LXMSR_MAX; i++) {
		reg = lx_msr[i].addr;
		if (pread(fd, &data, sizeof(data), reg) == sizeof(data)) {
			printf("%s %08" PRIx32 " = 0x%016" PRIx64 "\n", lx_msr[i].name, reg, data);
		}
	}

	printf("Geode LX PIC edge/level configuration\n");
	for (i = 0; i < LXIO_MAX; i++) {
		ioval = inb(lx_io[i].io);
		printf("%s %04" PRIx32 " = 0x%02" PRIx32 "\n", lx_io[i].name, lx_io[i].io, ioval);
	}

	close(fd);

	pci_cleanup(pacc);

	return 0;
}
