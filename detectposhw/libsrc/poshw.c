/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include "detectposhw-private.h"

bool sl18_check(void) {
	/* SL18, deprecated */
	return has_cyrix_5530;
}

bool sl19_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_AMD && cpu_family == 5 && cpu_model == 10 && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_GEODE) == 0);
	bool has_general_software = (strstr(dmi_modalias, "dmi:bvnGeneralSoftware") != NULL);

	return good_cpu && !has_general_software;
}

bool microsws5_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_AMD && cpu_family == 5 && cpu_model == 10 && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_GEODE) == 0);
	bool has_general_software = (strstr(dmi_modalias, "dmi:bvnGeneralSoftware") != NULL);

	return good_cpu && has_general_software && !has_usb_elo_touch;
}

bool microsws4lx_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_AMD && cpu_family == 5 && cpu_model == 10 && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_GEODE) == 0);
	bool has_general_software = (strstr(dmi_modalias, "dmi:bvnGeneralSoftware") != NULL);

	return good_cpu && has_general_software && has_usb_elo_touch;
}

bool microsws5a_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6 && cpu_model == 28 &&
		strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_ATOM_N450_1660, sizeof(CPUINFO_CPU_MODEL_NAME_ATOM_N450_1660) - 1) == 0);
	return good_cpu;
}

bool microspcws2010_check(void) {
	bool good_cpu1 = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6 && cpu_model == 9 &&
		(strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_1300, sizeof(CPUINFO_CPU_MODEL_NAME_INTEL_1300) - 1) == 0 ||
		 strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_1300_2, sizeof(CPUINFO_CPU_MODEL_NAME_INTEL_1300_2) - 1) == 0));
	bool good_cpu2 = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6 && cpu_model == 13 &&
		strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_PENTIUM_1800, sizeof(CPUINFO_CPU_MODEL_NAME_PENTIUM_1800) - 1) == 0);

	return good_cpu1 || good_cpu2;
}

bool sl21_check(void) {
	bool good_dmi = (strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS335") == 0 || strcmp(dmi_product, "SL21") == 0);
	bool has_2cd = (strstr(dmi_board, "_2CD") != NULL);

	return good_dmi && !has_usb_egalax_exc3188 && !has_2cd;
}

bool sl21_check2(void) {
	bool good_dmi = (strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS335") == 0 || strcmp(dmi_product, "SL21") == 0);
	bool has_2cd = (strstr(dmi_board, "_2CD") != NULL);

	return good_dmi && !has_usb_egalax_exc3188 && has_2cd;
}

bool sl21b_check(void) {
	bool good_dmi = (strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS335") == 0 || strcmp(dmi_product, "SL21") == 0);
	bool good_dmi2 = ((strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS455") == 0 || strcmp(dmi_product, "SL21B") == 0));
	bool has_2cd = (strstr(dmi_board, "_2CD") != NULL);

	return ((good_dmi && has_usb_egalax_exc3188) || good_dmi2) && !has_2cd;
}

bool sl21b_check2(void) {
	bool good_dmi = (strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS335") == 0 || strcmp(dmi_product, "SL21") == 0);
	bool good_dmi2 = ((strcmp(dmi_vendor, "SICOM Systems Inc.") == 0 || strcmp(dmi_vendor, "Xenial Inc.") == 0) &&
				(strcmp(dmi_product, "POS455") == 0 || strcmp(dmi_product, "SL21B") == 0));
	bool has_2cd = (strstr(dmi_board, "_2CD") != NULL);

	return ((good_dmi && has_usb_egalax_exc3188) || good_dmi2) && has_2cd;
}

bool tosh4852_check(void) {
	bool good_product = (strcmp(dmi_product, "4852E70") == 0 || strcmp(dmi_product, "4852570") == 0);

	return good_product;
}

bool xc23_check(void) {
	bool good_branded = strcmp(dmi_vendor, "Xenial Inc.") == 0 && strcmp(dmi_product, "XC23") == 0;
	bool good_branded1 = strcmp(dmi_vendor, "Global Payments") == 0 && strcmp(dmi_product, "XC23") == 0;
	bool good_unbranded = strcmp(dmi_vendor, "Insyde") == 0 && strcmp(dmi_product, "ElkhartLake") == 0 && strncmp(dmi_bios_version, "F340", 4) == 0;

	return good_branded || good_branded1 || good_unbranded;
}
