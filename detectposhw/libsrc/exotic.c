/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include "detectposhw-private.h"

bool mp500_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6
			&& (strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_N2930_1830) == 0)
			&& has_rtl8111);
	return good_cpu;
}

bool mp500bdbc_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6
			&& (strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_COREI5_M520_2400) == 0)
			&& has_intel_82577lm);
	return good_cpu;
}

bool nc2100_check(void) {
	bool good_cpu = (cpu_vendor == PCI_VENDOR_INTEL && cpu_family == 6
			&& (strcmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_INTEL_P4500_1870) == 0)
			&& has_intel_82577lm);
	return good_cpu;
}

bool rpi3bplus_check(void) {
	/*
	 * Raspberry Pi 3 Model B Plus, all revisions
	 */
	bool good_cpu = (strncmp(cpu_model_name, CPUINFO_CPU_MODEL_NAME_RPI3BPLUS, sizeof(CPUINFO_CPU_MODEL_NAME_RPI3BPLUS) - 1) == 0);

	return good_cpu;
}
