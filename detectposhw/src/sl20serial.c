/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc.
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 *
 * Program to setup Nuvoton or Winbond W83627UHG or NCT6627UD
 * chip properly for SL20 use.
 *
 * The utility MUST be executed as root
 *
 * created by SWT 130124
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/io.h>

#define PROG_VER  "0.13-4/17/2013"

#define CFG_BASE 0x02E   /* Winbond W83627UHG Config Base */
#define CFG_BASE2 0x04E   /* Winbond 2nd W83627UHG Config Base */

#define VERBOSE 1     /* 0-3; 0= no output, 3= max output */

void help(char * pgm) {
	printf("Usage: %s [OPTION] | [COMMAND]\n"
			" options:\n"
			"   --help    display this help and exit\n"
			"   --version output version information and exit\n"
			"   commands:\n"
			"   setio             Force SL20 serial port I/O addresses\n"
			"   setlevelirq       Force level style IRQ operation\n", pgm);
}

void version(void) {
	printf("Winbond W83627UHG SL20 serial setup program, SICOM Systems Inc. VER "PROG_VER"\n");
}


int w83627uhg(void) {
	unsigned char device_id, device_rev;
	/* Enter Extended Function Mode*/
	outb( (unsigned char) 0x87, CFG_BASE);
	outb( (unsigned char) 0x87, CFG_BASE);

	outb( (unsigned char) 0x20, CFG_BASE);	/* point to CR20 */
	device_id = inb(CFG_BASE+1);	

	outb( (unsigned char) 0x21, CFG_BASE);	/* point to CR21 */
	device_rev = inb(CFG_BASE+1);

	/* Exit Extended Function Mode */
	outb( (unsigned char) 0xaa, CFG_BASE);
#if VERBOSE > 2
	printf("Winbond1 W83627xxx  CR20 (Device ID)= 0x%2.2x\n", device_id);
	printf("                    CR21 (Device Rev)= 0x%2.2x\n", device_rev);
#endif
	if (device_id == 0xa2 && (device_rev & 0xf0) == 0x30) {
		return 1;	// W83627UHG or or NCT6627UD chip found
	}
	return 0;
}


int main (int argc, char **argv) {
	
	unsigned char g_cr24, g_cr2c, g_cr22;
	unsigned int addr, irq;
	int setio=0, setlevelirq=0;

	if (iopl(3)) {
		printf("NO ACCESS!! (must be run as ROOT)\n");
		return 0;
	}

	if (argc >= 2) {
		if (strcasecmp(argv[1], "-help") ==0 || strcasecmp(argv[1], "--help")==0) {
			help(argv[0]);
			return 0;
		}
		if (strcasecmp(argv[1], "-version") ==0 || strcasecmp(argv[1], "--version")==0) {
			version();
			return 0;
		}
		if (strcasecmp(argv[1], "-setio") ==0 || strcasecmp(argv[1], "setio")==0) {
			setio=1;
		}
		if (strcasecmp(argv[1], "-setlevelirq") ==0 || strcasecmp(argv[1], "setlevelirq")==0) {
			setlevelirq=1;
		}
	}

	if (argc >= 3) {
		if (strcasecmp(argv[2], "-setio") ==0 || strcasecmp(argv[2], "setio")==0) {
			setio=1;
		}

		if (strcasecmp(argv[2], "-setlevelirq") ==0 || strcasecmp(argv[2], "setlevelirq")==0) {
			setlevelirq=1;
		}
	}


	if (w83627uhg()==0) {
#if VERBOSE > 1
		printf("W83627UHG or NCT6627UD Not Detected\n");
#endif
		return 0;
	}

#if VERBOSE	
	printf(" W83627UHG or NCT6627UD Detected, configuring for SL20\n");
	if (setio==1) {
		printf(" Setting Serial Port I/O and IRQ values\n");
	}
	if (setlevelirq==1) {
		printf(" Setting level type IRQ mode\n");
	}
#endif

	/* Enter Extended Function Mode*/
	outb( (unsigned char) 0x87, CFG_BASE);
	outb( (unsigned char) 0x87, CFG_BASE);

	/* set bits 1-0 of CR2C to 11 to enable UART B */
	outb( (unsigned char) 0x2C, CFG_BASE);	/* point to CR2C */
	g_cr2c = inb(CFG_BASE+1);
	outb(  ((unsigned char) (g_cr2c | 0x03)), CFG_BASE+1);
	
	/* set bit 1 of global CR24 high for FDC is disabled */
	outb( (unsigned char) 0x24, CFG_BASE);	/* point to CR24 */
	g_cr24 = inb(CFG_BASE+1);
	outb( ((unsigned char) (g_cr24 | 0x02)), CFG_BASE+1);
		
	/* clear bit 0 of global CF22 to power down FDC */
	outb( (unsigned char) 0x22, CFG_BASE);	/* point to CR22 */
	g_cr22 = inb(CFG_BASE+1);
	outb( (unsigned char) g_cr22 & 0xfe, CFG_BASE+1);

	/* Select Logical Device 7 (GPIO3, GPIO4) */
	outb( (unsigned char) 0x07, CFG_BASE);
	outb( (unsigned char) 0x07, CFG_BASE+1);
	/*
	 * For device 7, write CF30 = 0 GPIO4 inactive and GPIO3 inactive
	 * Select CR30
	 */
	outb( (unsigned char) 0x30, CFG_BASE);
	outb( (unsigned char) 0x00, CFG_BASE+1);

	/* Select Logical Device 8 (GPIO5, GPIO6, GPIO Base Address */
	outb( (unsigned char) 0x07, CFG_BASE);
	outb( (unsigned char) 0x08, CFG_BASE+1);

	/*
	 * For device 8, write CF30 = 6 GPIO6 and GPIO5 active
	 * Select CR30
	 */
	outb( (unsigned char) 0x30, CFG_BASE);
	outb( (unsigned char) 0x06, CFG_BASE+1);

	/*
	 * For device 8, write CFE0 = 0xc3 so P52,P53,P54,P55 are outputs
	 * Select CRE0
	 */
	outb( (unsigned char) 0xE0, CFG_BASE);
	outb( (unsigned char) 0xc3, CFG_BASE+1);

	/*
	 * For device 8, write CFE1 = 0x30 (data register) LED1, LED2 OFF
	 * Select CRE1
	 */
	outb( (unsigned char) 0xE1, CFG_BASE);
	outb( (unsigned char) 0x30, CFG_BASE+1);

	/*
	 * For device 8, write CFE5 = 0x0f (data register)
	 * leave CASH_SOLA and CASH_SOLB high, make LED0_RDY# low
	 * Select CRE5
	 */
	outb( (unsigned char) 0xE5, CFG_BASE);
	outb( (unsigned char) 0x0F, CFG_BASE+1);

	/*
	 * For device 8, write CFE4 = 0xe3 so P62,63,64 are outputs
	 * Select CRE4
	 */
	outb( (unsigned char) 0xE4, CFG_BASE);
	outb( (unsigned char) 0xe3, CFG_BASE+1);

#if 0
	/*
	 * For device 8, write CF60,61 = 0x03F0 (GPIO I/O access base address)
	 * Select CR60
	 */
	outb( (unsigned char) 0x60, CFG_BASE);
	outb( (unsigned char) 0x03, CFG_BASE+1);
	outb( (unsigned char) 0x61, CFG_BASE);
	outb( (unsigned char) 0xf0, CFG_BASE+1);
#endif

	/* Select Logical Device 9 */
	outb( (unsigned char) 0x07, CFG_BASE);
	outb( (unsigned char) 0x09, CFG_BASE+1);
	/*
	 * For device 9, write CF30 = 0 GPIO2 and GPIO1 inactive
	 * Select CR30
	 */
	outb( (unsigned char) 0x30, CFG_BASE);
	outb( (unsigned char) 0x00, CFG_BASE+1);

	if (setio==1) {

		/* Select Logical Device 6 (UARTC) (Fuel Gauge) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x06, CFG_BASE+1);

		// UARTC
		addr = 0x2a0;
		irq = 3;
	
		/* Select CR60 */
		outb( (unsigned char) 0x60, CFG_BASE);
		outb( (unsigned char) (0x00ff & (addr>>8)), CFG_BASE+1);	// I/O MSB
		/* Select CR61 */
		outb( (unsigned char) 0x61, CFG_BASE);
		outb( (unsigned char) (0x00ff & addr), CFG_BASE+1);	// I/O LSB
		/* Select CR70 */
		outb( (unsigned char) 0x70, CFG_BASE);
		outb( (unsigned char) irq, CFG_BASE+1);	// IRQ

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device


		/* Select Logical Device D (UARTD) (touch screen) */;
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x0d, CFG_BASE+1);	

		// USRTD
		addr = 0x2a8;
		irq = 3;

		/* Select CR60 */
		outb( (unsigned char) 0x60, CFG_BASE);
		outb( (unsigned char) (0x00ff & (addr>>8)), CFG_BASE+1);	// I/O MSB
		/* Select CR61 */
		outb( (unsigned char) 0x61, CFG_BASE);
		outb( (unsigned char) (0x00ff & addr), CFG_BASE+1);	// I/O LSB
		/* Select CR70 */
		outb( (unsigned char) 0x70, CFG_BASE);
		outb( (unsigned char) irq, CFG_BASE+1);	// IRQ

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device


		/* Select Logical Device F (UARTF) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x0f, CFG_BASE+1);

		// UARTF
		addr = 0x2b0;
		irq = 3;
	
		/* Select CR60 */
		outb( (unsigned char) 0x60, CFG_BASE);
		outb( (unsigned char) (0x00ff & (addr>>8)), CFG_BASE+1);	// I/O MSB
		/* Select CR61 */
		outb( (unsigned char) 0x61, CFG_BASE);
		outb( (unsigned char) (0x00ff & addr), CFG_BASE+1);	// I/O LSB
		/* Select CR70 */
		outb( (unsigned char) 0x70, CFG_BASE);
		outb( (unsigned char) irq, CFG_BASE+1);	// IRQ

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device
	}

	if (setlevelirq==1) {
		/* Select Logical Device 2 (UARTA) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x02, CFG_BASE+1);

		/* Set level-control interrupt */
		outb( (unsigned char) 0xF0, CFG_BASE);
		outb( (unsigned char) 0x00, CFG_BASE+1);

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device

		/* Select Logical Device 3 (UARTB) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x03, CFG_BASE+1);

		/* Set level-control interrupt */
		outb( (unsigned char) 0xF0, CFG_BASE);
		outb( (unsigned char) 0x00, CFG_BASE+1);

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device

		/* Select Logical Device 6 (UARTC) (Fuel Gauge) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x06, CFG_BASE+1);

		/* Set level-control interrupt */
		outb( (unsigned char) 0xF0, CFG_BASE);
		outb( (unsigned char) 0x00, CFG_BASE+1);

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device

		/* Select Logical Device D (UARTD) (touch screen) */;
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x0d, CFG_BASE+1);	

		/* Set level-control interrupt */
		outb( (unsigned char) 0xF0, CFG_BASE);
		outb( (unsigned char) 0x00, CFG_BASE+1);

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device

		/* Select Logical Device F (UARTF) */
		outb( (unsigned char) 0x07, CFG_BASE);
		outb( (unsigned char) 0x0f, CFG_BASE+1);

		/* Set level-control interrupt */
		outb( (unsigned char) 0xF0, CFG_BASE);
		outb( (unsigned char) 0x00, CFG_BASE+1);

		/* Select CR30 */
		outb( (unsigned char) 0x30, CFG_BASE);
		outb( (unsigned char) 0x01, CFG_BASE+1);	// enable device
	}

	/* Exit Extended Function Mode */
	outb( (unsigned char) 0xaa, CFG_BASE);

	return 0;
}
