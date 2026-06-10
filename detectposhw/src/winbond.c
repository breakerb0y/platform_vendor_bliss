/*
 *   COPYRIGHT (C) 2011-2026 SICOM Systems Inc.
 *   1684 S. Broad Street Suite 300
 *   Lansdale, PA 19446
 *
 *	Utility to display configuration information
 * 	from the Winbond W83627 super I/O chips in
 * 	the system.
 *
 * 	For a Micros WS5, it seems different versions
 * 	of the BIOS setup the IRQ for winbond #1 UART A
 * 	differently.
 *
 * 	created by SWT 110211
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


#define CFG_BASE 0x02E   /* Winbond W83627HB Config Base */
#define CFG_BASE2 0x04E   /* Winbond 2nd W83627HB Config Base */


int main (int argc, char **argv) {

	unsigned char device_id, device_rev;
	unsigned char ld2_cr30, ld2_cr60, ld2_cr61, ld2_cr70, ld2_crf0;
	unsigned char ld3_cr30, ld3_cr60, ld3_cr61, ld3_cr70, ld3_crf0;

	if (iopl(3)) {
    	printf("NO ACCESS!! (must be run as ROOT)\n");
		return 0;
	}

	/* Enter Extended Function Mode*/
	outb( (unsigned char) 0x87, CFG_BASE);
	outb( (unsigned char) 0x87, CFG_BASE);

	outb( (unsigned char) 0x20, CFG_BASE);	/* point to CR20 */
	device_id = inb(CFG_BASE+1);	

	outb( (unsigned char) 0x21, CFG_BASE);	/* point to CR21 */
	device_rev = inb(CFG_BASE+1);	

	/* Select Logical Device 2 */
	outb( (unsigned char) 0x07, CFG_BASE);
	outb( (unsigned char) 0x02, CFG_BASE+1);

	/* Select CR30 */
	outb( (unsigned char) 0x30, CFG_BASE);
	ld2_cr30 = inb(CFG_BASE+1);
	
	/* Select CR60 */
	outb( (unsigned char) 0x60, CFG_BASE);
	ld2_cr60 = inb(CFG_BASE+1);

	/* Select CR61 */
	outb( (unsigned char) 0x61, CFG_BASE);
	ld2_cr61 = inb(CFG_BASE+1);

	/* Select CR70 */
	outb( (unsigned char) 0x70, CFG_BASE);
	ld2_cr70 = inb(CFG_BASE+1);
	
	/* Select CRF0 */
	outb( (unsigned char) 0xF0, CFG_BASE);
	ld2_crf0 = inb(CFG_BASE+1);

	/* Select Logical Device 3 */
	outb( (unsigned char) 0x07, CFG_BASE);
	outb( (unsigned char) 0x03, CFG_BASE+1);

	/* Select CR30 */
	outb( (unsigned char) 0x30, CFG_BASE);
	ld3_cr30 = inb(CFG_BASE+1);
	
	/* Select CR60 */
	outb( (unsigned char) 0x60, CFG_BASE);
	ld3_cr60 = inb(CFG_BASE+1);

	/* Select CR61 */
	outb( (unsigned char) 0x61, CFG_BASE);
	ld3_cr61 = inb(CFG_BASE+1);

	/* Select CR70 */
	outb( (unsigned char) 0x70, CFG_BASE);
	ld3_cr70 = inb(CFG_BASE+1);
	
	/* Select CRF0 */
	outb( (unsigned char) 0xF0, CFG_BASE);
	ld3_crf0 = inb(CFG_BASE+1);
	
	/* Exit Extended Function Mode */
	outb( (unsigned char) 0xaa, CFG_BASE);
	
	if (device_id == 0xff && device_rev == 0xff) {
		printf("No Winbond W83627HF devices found\n");
		return 0;
	}

	printf("Winbond1 W83627HF  CR20 (Device ID)= = 0x%2.2x\n", device_id);
	printf("                   CR21 (Device Rev)= = 0x%2.2x\n", device_rev);
	printf("W1_UART_A  CR30=0x%2.2x CRF0=x%2.2x addr=0x%2.2x%2.2x  IRQ=%2.2d\n", 
			ld2_cr30, ld2_crf0, ld2_cr60, ld2_cr61, ld2_cr70); 
	printf("W1_UART_B  CR30=0x%2.2x CRF0=x%2.2x addr=0x%2.2x%2.2x  IRQ=%2.2d\n", 
			ld3_cr30, ld3_crf0, ld3_cr60, ld3_cr61, ld3_cr70); 

	/* Enter Extended Function Mode*/
	outb( (unsigned char) 0x87, CFG_BASE2);
	outb( (unsigned char) 0x87, CFG_BASE2);

	outb( (unsigned char) 0x20, CFG_BASE2);	/* point to CR20 */
	device_id = inb(CFG_BASE2+1);	

	outb( (unsigned char) 0x21, CFG_BASE2);	/* point to CR21 */
	device_rev = inb(CFG_BASE2+1);	

	/* Select Logical Device 2 */
	outb( (unsigned char) 0x07, CFG_BASE2);
	outb( (unsigned char) 0x02, CFG_BASE2+1);

	/* Select CR30 */
	outb( (unsigned char) 0x30, CFG_BASE2);
	ld2_cr30 = inb(CFG_BASE2+1);
	
	/* Select CR60 */
	outb( (unsigned char) 0x60, CFG_BASE2);
	ld2_cr60 = inb(CFG_BASE2+1);

	/* Select CR61 */
	outb( (unsigned char) 0x61, CFG_BASE2);
	ld2_cr61 = inb(CFG_BASE2+1);

	/* Select CR70 */
	outb( (unsigned char) 0x70, CFG_BASE2);
	ld2_cr70 = inb(CFG_BASE2+1);
	
	/* Select CRF0 */
	outb( (unsigned char) 0xF0, CFG_BASE2);
	ld2_crf0 = inb(CFG_BASE2+1);


	/* Select Logical Device 3 */
	outb( (unsigned char) 0x07, CFG_BASE2);
	outb( (unsigned char) 0x03, CFG_BASE2+1);

	/* Select CR30 */
	outb( (unsigned char) 0x30, CFG_BASE2);
	ld3_cr30 = inb(CFG_BASE2+1);
	
	/* Select CR60 */
	outb( (unsigned char) 0x60, CFG_BASE2);
	ld3_cr60 = inb(CFG_BASE2+1);

	/* Select CR61 */
	outb( (unsigned char) 0x61, CFG_BASE2);
	ld3_cr61 = inb(CFG_BASE2+1);

	/* Select CR70 */
	outb( (unsigned char) 0x70, CFG_BASE2);
	ld3_cr70 = inb(CFG_BASE2+1);
	
	/* Select CRF0 */
	outb( (unsigned char) 0xF0, CFG_BASE2);
	ld3_crf0 = inb(CFG_BASE2+1);
	
	/* Exit Extended Function Mode */
	outb( (unsigned char) 0xaa, CFG_BASE2);
	
	if (device_id == 0xff && device_rev == 0xff)
		return 0;

	printf("Winbond2 W83627HF  CR20 (Device ID)= = 0x%2.2x\n", device_id);
	printf("                   CR21 (Device Rev)= = 0x%2.2x\n", device_rev);
	printf("W2_UART_A  CR30=0x%2.2x CRF0=x%2.2x addr=0x%2.2x%2.2x  IRQ=%2.2d\n", 
			ld2_cr30, ld2_crf0, ld2_cr60, ld2_cr61, ld2_cr70); 
	printf("W2_UART_B  CR30=0x%2.2x CRF0=x%2.2x addr=0x%2.2x%2.2x  IRQ=%2.2d\n", 
			ld3_cr30, ld3_crf0, ld3_cr60, ld3_cr61, ld3_cr70); 
	
	return 0;

}
