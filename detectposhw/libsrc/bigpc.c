/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include "detectposhw-private.h"

bool dop3040_check(void) {
	bool good_product = (strcmp(dmi_product, "OptiPlex 3040") == 0 || strcmp(dmi_product, "OptiPlex 3050") == 0);

	return good_product;
}

bool xenialdtc_check(void) {
	bool good_branded = (strcmp(dmi_vendor, "Xenial Inc.") == 0 && strcmp(dmi_product, "DC2.1") == 0 && strcmp(dmi_board, "51F92SV10001") == 0);
	bool good_unbranded = (strcmp(dmi_vendor, "Insyde") == 0 && strcmp(dmi_product, "CoffeeLake") == 0 && strncmp(dmi_bios_version, "F92H", 4) == 0);

	return good_branded || good_unbranded;
}
