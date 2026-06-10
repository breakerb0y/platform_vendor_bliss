/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <stdio.h>

#include "detectposhw-private.h"

bool dli8300_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6 && cpu_model == 28 &&
		strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_ATOM_Z530, sizeof(CPUINFO_CPU_MODEL_NAME_ATOM_Z530) - 1) == 0);
	return good_cpu;
}

bool gbox_check(void) {
	bool good_dmi = (strcmp(dmi_vendor, "ZOTAC") == 0 && strcmp(dmi_product, "ZBOXNANO-AQ01") == 0);
	/*
	 * Approximate detection of the Sicom branded GBOXes.
	 *
	 * Someone at Sicom forgot to ask for branded DMI strings
	 * and should have been shot on the premises.
	 *
	 * Zotac cleared the DMI strings leaving a single whitespace,
	 * so the best we can come up with is check for any spaces in
	 * both sys_vendor and product_name and check for:
	 * - the CPU type in /proc/cpuinfo
	 * - the videocard, PCI IDs 1002:9832 or 1002:9833
	 * - the RealTek 8111
	 *   from the PCI device list.
	 * Unfortunately a lot of low const computers (possibly
	 * POSes as well) have these devices.
	 * Maybe but ONLY MAYBE the empty DMI strings allow
	 * distinguishing between them.
	 *
	 * Something called Sapphire insert is also detected by this.
	 * It a standalone computer board that can be inserted into monitors
	 * with similar form factor as the computer board in the LG MP500.
	 * The APU is also an AMD GX 415GA, the network card is identical,
	 * the DMI strings are also only contain spaces.
	 * Fortunately the video outputs are wired in an identical way.
	 * If it walks like a duck, quacks like a duck then it is a duck.
	 */
	bool is_sicom_gbox = (rest_is_spaces(dmi_vendor) && rest_is_spaces(dmi_product)
			&& cpu_vendor == PCI_VENDOR_AMD && cpu_family == 22
			&& (strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_A4_5000) == 0 || strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_GX_415GA) == 0)
			&& has_kabini && has_rtl8111);
	/*
	 * A different computer board insert made by Samsung is
	 * also treated as a GBOX.
	 */
	bool is_samsung_insert = (strcmp(dmi_vendor, "Samsung") == 0 && strcmp(dmi_product, "SBB-DA") == 0 && strcmp(dmi_board, "Ballina") == 0);
	/*
	 * If the above Samsung board is usable with the GBOX configuration
	 * then any identical machine is also usable with different DMI data,
	 * detected by the APU name and the GPU presence.
	 * We'll sort it out later if we ever have a machine using this APU
	 * and a touchscreen. Hopefully with a USB connected touchscreen or
	 * with specific DMI data.
	 */
	bool is_generic_gbox = (cpu_vendor == PCI_VENDOR_AMD && cpu_family == 21
			&& strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_RX_425BB) == 0
			&& has_kaveri);

	return good_dmi || is_sicom_gbox || is_samsung_insert || is_generic_gbox;
}

bool picobox_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6 && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_x5_z8300) == 0 &&
			has_intel_igpu && has_intel_wifi_3165 && has_usb_rtl8152);

	return good_cpu;
}

bool zboxid41_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_D525_180) == 0
				&& has_nv_ion && has_atheros_wifi && has_rtl8110);

	return good_cpu;
}

bool zboxid84_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_D2550_186) == 0);
	bool good_peripherals = (has_nv_gf119m && has_intel_wifi_n130 && has_rtl8111);

	return good_cpu && good_peripherals;
}

bool zboxid86_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_D2550_186) == 0);
	bool good_peripherals = (has_nv_gf119 && (has_intel_wifi_3160 || has_intel_wifi_n2230) && has_rtl8111);

	return good_cpu && good_peripherals;
}

bool kscapl_check(void) {
	bool good_dmi = strcmp(dmi_vendor, "Default string") == 0 && strcmp(dmi_product, "Default string") == 0 && strcmp(dmi_board, "Apollolake") == 0;
	bool good_dmi2 = strcmp(dmi_product, "Xenial N3350D") == 0;
	bool good_dmi3 = strncmp(dmi_vendor, "OEM", 3) == 0 && strcmp(dmi_product, "Nano-N3350D") == 0;
	bool good_dmi4 = strncmp(dmi_vendor, "OEM", 3) == 0 && strcmp(dmi_board, "Nano-N3350D") == 0;
	bool good_cpu = strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_N3350_1100, sizeof(CPUINFO_CPU_MODEL_NAME_INTEL_N3350_1100) - 1) == 0;

	return (good_dmi && good_cpu && has_intel_wifi_3165 && has_rtl8111) || good_dmi2 || good_dmi3 || good_dmi4;
}

bool mp10_check(void) {
	bool good_branded = strcmp(dmi_vendor, "Xenial Inc.") == 0 && strcmp(dmi_product, "MP10") == 0;
	bool good_unbranded = strcmp(dmi_vendor, "Insyde") == 0 && strcmp(dmi_product, "ApolloLake") == 0 && strncmp(dmi_bios_version, "D970", 4) == 0;

	return good_branded || good_unbranded;
}

bool mp30_check(void) {
	bool good_branded = strcmp(dmi_vendor, "Xenial Inc.") == 0 && strcmp(dmi_product, "MP30") == 0 && strncmp(dmi_bios_version, "F3A0", 4) == 0;
	bool good_unbranded = strcmp(dmi_vendor, "Insyde") == 0 && strcmp(dmi_product, "ElkhartLake") == 0 && strncmp(dmi_bios_version, "F3A0", 4) == 0;

	return good_branded || good_unbranded;
}

bool ark1221l_check(void) {
        /*
         * Advantech Ark 1221 L, all revisions
         */
        bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6
                        && (strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_x6413E_1500) == 0)
                        && has_intel_i225lm);
        return good_cpu;
}
