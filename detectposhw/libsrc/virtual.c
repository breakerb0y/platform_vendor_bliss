/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <stdbool.h>
#include <string.h>

#include "detectposhw-private.h"

bool kvmqxl_check(void) {
	bool good_product1 = (strcmp(dmi_vendor, "Red Hat") == 0 && strcmp(dmi_product, "KVM") == 0);
	bool good_product2 = (strcmp(dmi_vendor, "QEMU") == 0);
	return (good_product1 || good_product2) && has_virtqxl && !has_virtiogpu && !has_virtvga;
}

bool kvmvirtio_check(void) {
	bool good_product1 = (strcmp(dmi_vendor, "Red Hat") == 0 && strcmp(dmi_product, "KVM") == 0);
	bool good_product2 = (strcmp(dmi_vendor, "QEMU") == 0);
	return (good_product1 || good_product2) && !has_virtqxl && has_virtiogpu && !has_virtvga;
}

bool kvmvga_check(void) {
	bool good_product1 = (strcmp(dmi_vendor, "Red Hat") == 0 && strcmp(dmi_product, "KVM") == 0);
	bool good_product2 = (strcmp(dmi_vendor, "QEMU") == 0);
	return (good_product1 || good_product2) && !has_virtqxl && !has_virtiogpu && has_virtvga;
}

bool virtualbox_check(void) {
	bool good_branded = strcmp(dmi_vendor, "innotek GmbH") == 0 && strcmp(dmi_product, "VirtualBox") == 0;

	return good_branded && has_vboxsvga;
}

bool vboxvmsvga_check(void) {
	bool good_branded = strcmp(dmi_vendor, "innotek GmbH") == 0 && strcmp(dmi_product, "VirtualBox") == 0;

	return good_branded && has_vmsvga2;
}
