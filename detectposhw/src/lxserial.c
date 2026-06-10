/*
 * COPYRIGHT (C) 2007-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

/*
 *
 *	Utility for Geode LX system with CS5536
 *	companion chip to restore UART2 operation
 *
 *	The CS5536 muliplexes the pins for UART2
 *	with a GPIO DDC (video monitor detection)
 *	function.  Later Linux kernels (> 2.6.14)
 *	reprogram the CS5536 for the DDC function
 *	leaving UART2 disconnected.  This utility
 *	will "fix" the GPIO configuration registers
 *	so that UART2 is functional again.
 *
 *	This utility will only apply the "fix" to
 *	systems that actually contain a CS5536
 *
 *	Note: SICOM LX based SL19 uses CS5536 UART2
 *	as COM4 (/dev/ttyS3) for the touchscreen
 *
 *	Enhancement-- For Micros WS5 LX system with CS5536
 *	and General Software BIOS:
 *	1. Set GPIO6 (drives power LED) to steady ON (BIOS set to flash)
 *	2. Power Button on GPIO28, set to 4 sec fail-safe off
 *		instead of BIOS instant off, by clearing Power Button
 *		SCI enable (PWRBTN_EN) in the ACPI PM Enable 1
 *		register (PM1_EN)
 *
 *	The utility MUST be executed as root
 *
 *	created by SWT 070718
 *
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <sys/io.h>

#include <pci/pci.h>

#include "cs5536.h"

#define ADDR_GPIO 0x6100	/* SL19 CS5536 GPIO I/O Base */
#define ADDR_MFGPT 0x6200	/* SL19 CS5536 MFGPT I/O Base */
#define ADDR_ACPI 0x9c00	/* SL19 CS5536 ACPI I/O Base */
#define ADDR_PMC 0x9d00		/* SL19 CS5536 PMC I/O Base */

#define VERBOSE 3     /* 0-3; 0= no output, 3= max output */

unsigned int gpio_base, acpi_base;
//unsigned int mfgpt_base, pmc_base;

int main (int argc, char **argv) {
	struct pci_access *pacc;
	int fd;
	unsigned int gpio_base, acpi_base;
	uint32_t reg;
	uint64_t data;
#if VERBOSE > 1
	uint32_t dword;
	uint16_t word;
#endif

	pacc = pci_alloc();
	pci_init(pacc);
	pci_scan_bus(pacc);

	if (iopl(3)) {
		printf("NO ACCESS!! (must be run as ROOT)\n");
		pci_cleanup(pacc);
		return 0;
	}

	if (!cs5536(pacc->devices)) {
#if VERBOSE
		printf("%s CS5536 Not Detected\n", argv[0]);
#endif
		pci_cleanup(pacc);
		return 0;
	}

	gpio_base = ADDR_GPIO;	/* default SL19 CS5536 I/O bases */
//	mfgpt_base = ADDR_MFGPT;
	acpi_base = ADDR_ACPI;
//	pmc_base = ADDR_PMC;

/* read actual I/O base addresses from LX, CS5536, msr registers */
/* note Linux kernel must have CONFIG_X86_MSR=y for this to work */

	fd = open("/dev/cpu/0/msr", O_RDONLY);
	if (fd == -1) {
		printf("Error opening /dev/cpu/0/msr: %s\n", strerror(errno));
		return 1;
	}

	reg = lx_msr[DIVIL_LBAR_GPIO].addr;
	if (pread(fd, &data, sizeof(data), reg) == sizeof(data)) {
#if VERBOSE > 1
		printf("rdmsr(0x%" PRIx32 ") = 0x%" PRIx64 "\n", reg, data);
#endif
		if ((data >> 32) == 0xf001 ) {
			gpio_base = (unsigned int) data & 0x00000000ff00;
		}
	}

	reg = lx_msr[DIVIL_LBAR_ACPI].addr;
	if (pread(fd, &data, sizeof(data), reg) == sizeof(data)) {
#if VERBOSE > 1
		printf("rdmsr(0x%" PRIx32 ") = 0x%" PRIx64 "\n", reg, data);
#endif
		if ((data >> 32) == 0xf001 ) {
			acpi_base = (unsigned int) data & 0x00000000ff00;
		}
	}

	close(fd);

#if VERBOSE > 1
	printf("gpio_base = 0x%x\n", gpio_base);
	printf("acpi_base = 0x%x\n", acpi_base);
#endif

#if VERBOSE > 1
	dword = inl(gpio_base);
	printf("I/O Read GPIO 00h = %08x \n", dword);

	dword = inl(gpio_base + 0x04);
	printf("I/O Read GPIO 04h = %08x \n", dword);

	dword = inl(gpio_base + 0x20);
	printf("I/O Read GPIO 20h = %08x \n", dword);
#endif

/* SL19 Fixup for GPIO3, GPIO4 to enable UART2 RX and TX */

// GPIOL_OUT_VAL 00h  GPIO 4 -
	outl((unsigned int) 0x00100000, gpio_base);

// GPIOL_OUT_EN 04h  GPIO 4+ 3-
	outl((unsigned int) 0x00080010, gpio_base + 0x04);

// GPIOL_IN_EN 20h  GPIO 4 -
	outl((unsigned int) 0x00100000, gpio_base + 0x20);

#if VERBOSE	
	printf("%s CS5536 Detected, LX CS5536 UART2 fix applied\n", argv[0]);
#endif

	if (gpio_base == 0xf000 && acpi_base == 0x9c00) { /* Micros WS5 only */

#if VERBOSE > 1
		dword = inl(gpio_base);
		printf("I/O Read GPIO 00h = %08x \n", dword);

		dword = inl(gpio_base + 0x04);
		printf("I/O Read GPIO 04h = %08x \n", dword);

		dword = inl(gpio_base + 0x10);
		printf("I/O Read GPIO 10h = %08x \n", dword);

		dword = inl(gpio_base + 0x14);
		printf("I/O Read GPIO 14h = %08x \n", dword);

		dword = inl(gpio_base + 0x20);
		printf("I/O Read GPIO 20h = %08x \n", dword);

		dword = inl(gpio_base + 0x34);
		printf("I/O Read GPIO 34h = %08x \n", dword);
#endif

		/* Micros WS5 Fixup for GPIO6 to make power LED steady ON */

		// GPIOL_OUT_AUX1_SEL 10h  GPIO 6 -
		outl((unsigned int) 0x00400000, gpio_base + 0x10);
		// GPIOL_OUT_AUX2_SEL 14h  GPIO 6 -
		outl((unsigned int) 0x00400000, gpio_base + 0x14);
		// GPIOL_IN_AUX1_SEL 34h  GPIO 6 -
		outl((unsigned int) 0x00400000, gpio_base + 0x34);
		// GPIOL_IN_EN 20h  GPIO 6 +
		outl((unsigned int) 0x00000040, gpio_base + 0x20);
		// GPIOL_OUT_VAL 00h  GPIO 6 +
		outl((unsigned int) 0x00000040, gpio_base + 0x00);
		// GPIOL_OUT_EN 04h  GPIO 6 +
		outl((unsigned int) 0x00000040, gpio_base + 0x04);

#if VERBOSE > 1
		dword = inl(gpio_base);
		printf("I/O Read GPIO 00h = %08x \n", dword);

		dword = inl(gpio_base + 0x04);
		printf("I/O Read GPIO 04h = %08x \n", dword);

		dword = inl(gpio_base + 0x10);
		printf("I/O Read GPIO 10h = %08x \n", dword);

		dword = inl(gpio_base + 0x14);
		printf("I/O Read GPIO 14h = %08x \n", dword);

		dword = inl(gpio_base + 0x20);
		printf("I/O Read GPIO 20h = %08x \n", dword);

		dword = inl(gpio_base + 0x34);
		printf("I/O Read GPIO 34h = %08x \n", dword);
#endif

#if VERBOSE > 1	
		word = inw(acpi_base + 0x02);
		printf("word I/O Read ACPI 02h = %04x\n", word);
#endif

		/* Micros WS5 Fixup for Power Button on GPIO28 to enable 4 sec fail-safe off */
		/* (disables Power Button press SCI that implements instant off) */

		outw(( unsigned int) 0x0000, acpi_base + 0x02); // write 0 to PWRBTN_EN

#if VERBOSE > 1	
		word = inw(acpi_base + 0x02);
		printf("word I/O Read ACPI 02h = %04x\n", word);
#endif

#if VERBOSE	
		printf("%s WS5 Detected, CS5536 fixes applied\n", argv[0]);
#endif

	}

	pci_cleanup(pacc);

	return 0;
}
