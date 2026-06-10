/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#ifdef HAVE_PCI_ENABLED
#include <pci/pci.h>
#endif
#include <libusb.h>

#include <detectposhw.h>
#include "detectposhw-private.h"
#include "version.h"

/* Flags for devices on supported hardware */
bool has_usb_elo_touch LOCALSYM = false;
bool has_usb_rtl8152 LOCALSYM = false;
bool has_usb_egalax_exc3188 LOCALSYM = false;
bool has_cyrix_5530 LOCALSYM = false;
bool has_kabini LOCALSYM = false;
bool has_kaveri LOCALSYM = false;
bool has_intel_igpu LOCALSYM = false;
bool has_intel_wifi_3160 LOCALSYM = false;
bool has_intel_wifi_3165 LOCALSYM = false;
bool has_intel_wifi_n130 LOCALSYM = false;
bool has_intel_wifi_n2230 LOCALSYM = false;
bool has_intel_82577lm LOCALSYM = false;
bool has_intel_i225lm LOCALSYM = false;
bool has_atheros_wifi LOCALSYM = false;
bool has_rtl8110 LOCALSYM = false;
bool has_rtl8111 LOCALSYM = false;
bool has_nv_ion LOCALSYM = false;
bool has_nv_gf119m LOCALSYM = false;
bool has_nv_gf119 LOCALSYM = false;
bool has_virtqxl LOCALSYM = false;
bool has_virtiogpu LOCALSYM = false;
bool has_virtvga LOCALSYM = false;
bool has_vboxsvga LOCALSYM = false;
bool has_vmsvga2 LOCALSYM = false;
int verbose LOCALSYM;

/* Generic hardware detection parameters */
int cpu_vendor LOCALSYM = 0, cpu_family LOCALSYM = 0, cpu_model LOCALSYM = 0;
char cpu_model_name[128] LOCALSYM = "";
char dmi_empty[1] LOCALSYM = "";
char *dmi_vendor LOCALSYM = NULL;
char *dmi_product LOCALSYM = NULL;
char *dmi_board LOCALSYM = NULL;
char *dmi_bios_version LOCALSYM = NULL;
char *dmi_modalias LOCALSYM = NULL;

__attribute__((constructor))
static void initialize_libdetectposhw(void) {
	char *override, *v;
	const char *cpuinfo;
	int i;

	v = getenv("DETECTPOSHW_VERBOSE");
	verbose = (v && *v);
	if (verbose) {
		verbose = atoi(v);
		if (!verbose && (strcasecmp(v, "yes") == 0 || strcasecmp(v, "true") == 0 || strcasecmp(v, "on") == 0))
			verbose = 1;
	}

	/*
	 * Allow overriding hardware detection
	 */
	override = getenv("DETECTPOSHW_OVERRIDE");
	if (override) {
		for (i = 0; i < SICOM_POSHW_N_IDS; i++) {
			if (strcasecmp(override, poshw_names[i]) == 0) {
				if (verbose)
					fprintf(stderr, "HARDWARE DETECTION OVERRIDDEN: %s\n", override);
				detectposhw_hwid = i;
				return;
			}
		}
	}

	cpuinfo = getenv("DETECTPOSHW_CPUINFO");
	if (!cpuinfo)
		cpuinfo = "/proc/cpuinfo";

	/*
	 * We are using libusb 1.x to detect permanently present devices.
	 * We don't need libusb_open() as it would require running as root.
	 * libusb_get_device_list() works as non-root.
	 *
	 * Using libusb is faster than running lsusb and parsing
	 * its output.
	 */
	libusb_context *usb_ctx = NULL;
	libusb_device **usb_devlist = NULL;
	ssize_t usb_devcnt = 0;
#ifdef HAVE_PCI_ENABLED
	/* libpci for PCI device detection */
	struct pci_access *pacc = NULL;
	struct pci_dev *pdev;
#endif

#ifdef HAVE_PCI_ENABLED
	do {
		pacc = pci_alloc();
		if (!pacc)
			break;

		pci_init(pacc);
		pci_scan_bus(pacc);

		for (pdev = pacc->devices; pdev; pdev = pdev->next) {
			pci_fill_info(pdev, PCI_FILL_IDENT | PCI_FILL_CLASS);
			switch (pdev->vendor_id) {
			case PCI_VENDOR_CYRIX:
				if (pdev->device_id == PCI_DEVICE_5530_LEGACY)
					has_cyrix_5530 = true;
				break;

			case PCI_VENDOR_ATI:
				switch (pdev->device_id) {
				case PCI_DEVICE_KABINI_1: has_kabini = true; break;
				case PCI_DEVICE_KABINI_2: has_kabini = true; break;
				case PCI_DEVICE_KAVERI_1: has_kaveri = true; break;
				}
				break;

			case PCI_VENDOR_INTEL:
				switch (pdev->device_id) {
				case PCI_DEVICE_INTEL_IGPU: has_intel_igpu = true; break;
				case PCI_DEVICE_INTEL_WIFI_3160: has_intel_wifi_3160 = true; break;
				case PCI_DEVICE_INTEL_WIFI_3165: has_intel_wifi_3165 = true; break;
				case PCI_DEVICE_INTEL_N_130: has_intel_wifi_n130 = true; break;
				case PCI_DEVICE_INTEL_N_2230: has_intel_wifi_n2230 = true; break;
				case PCI_DEVICE_INTEL_82577LM: has_intel_82577lm = true; break;
				case PCI_DEVICE_INTEL_I225LM: has_intel_i225lm = true; break;
				}
				break;

			case PCI_VENDOR_REALTEK:
				switch (pdev->device_id) {
				case PCI_DEVICE_RTL8110: has_rtl8110 = true; break;
				case PCI_DEVICE_RTL8111: has_rtl8111 = true; break;
				}
				break;

			case PCI_VENDOR_NVIDIA:
				switch (pdev->device_id) {
				case PCI_DEVICE_NV_ION: has_nv_ion = true; break;
				case PCI_DEVICE_NV_GF119M: has_nv_gf119m = true; break;
				case PCI_DEVICE_NV_GF119: has_nv_gf119 = true; break;
				}
				break;

			case PCI_VENDOR_ATHEROS:
				if (pdev->device_id == PCI_DEVICE_AR9285)
					has_atheros_wifi = true;
				break;

			case PCI_VENDOR_REDHAT:
				if (pdev->device_id == PCI_DEVICE_VIRTQXL)
					has_virtqxl = true;
				break;

			case PCI_VENDOR_REDHAT2:
				if (pdev->device_id == PCI_DEVICE_VIRTIOGPU)
					has_virtiogpu = true;
				break;

			case PCI_VENDOR_INNOTEK:
				if (pdev->device_id == PCI_DEVICE_VBOXSVGA)
					has_vboxsvga = true;
				break;

			case PCI_VENDOR_VMWARE:
				if (pdev->device_id == PCI_DEVICE_VMWSVGA2)
					has_vmsvga2 = true;
				break;

			case PCI_VENDOR_DUMMY:
				if (pdev->device_id == PCI_DEVICE_VIRTVGA)
					has_virtvga = true;
				break;
			}
		}

		pci_cleanup(pacc);
	} while (0);
#endif

	if (verbose) {
		fprintf(stderr, "After PCI detection loop:\n");
		PRINT_HAS(cyrix_5530);
		PRINT_HAS(kabini);
		PRINT_HAS(kaveri);
		PRINT_HAS(intel_igpu);
		PRINT_HAS(intel_wifi_3160);
		PRINT_HAS(intel_wifi_3165);
		PRINT_HAS(intel_wifi_n130);
		PRINT_HAS(intel_wifi_n2230);
		PRINT_HAS(intel_82577lm);
		PRINT_HAS(intel_i225lm);
		PRINT_HAS(atheros_wifi);
		PRINT_HAS(rtl8110);
		PRINT_HAS(rtl8111);
		PRINT_HAS(nv_ion);
		PRINT_HAS(nv_gf119m);
		PRINT_HAS(nv_gf119);
		PRINT_HAS(virtqxl);
		PRINT_HAS(virtiogpu);
		PRINT_HAS(virtvga);
	}

	do {
		if (libusb_init(&usb_ctx) != 0)
			break;
		if (!usb_ctx)
			break;

		usb_devcnt = libusb_get_device_list(usb_ctx, &usb_devlist);

		/* Detect USB devices we care about */
		for (i = 0; i < usb_devcnt; i++) {
			libusb_device *dev = usb_devlist[i];
			struct libusb_device_descriptor desc;

			libusb_get_device_descriptor(dev, &desc);

			if (desc.idVendor == USB_VENDOR_ELO && desc.idProduct == USB_DEVICE_2216_ACCUTOUCH)
				has_usb_elo_touch = true;

			if (desc.idVendor == USB_VENDOR_REALTEK && desc.idProduct == USB_DEVICE_RTL8152)
				has_usb_rtl8152 = true;

			if (desc.idVendor == USB_VENDOR_DWAV && desc.idProduct == USB_DEVICE_EXC3188)
				has_usb_egalax_exc3188 = true;
		}

		libusb_free_device_list(usb_devlist, 1);
		libusb_exit(usb_ctx);
	} while (0);

	if (verbose) {
		fprintf(stderr, "After USB detection loop:\n");
		PRINT_HAS(usb_elo_touch);
		PRINT_HAS(usb_rtl8152);
		PRINT_HAS(usb_egalax_exc3188);
	}

	parse_cpuinfo(cpuinfo, &cpu_vendor, &cpu_family, &cpu_model, cpu_model_name, sizeof(cpu_model_name));
	if (verbose) {
		fprintf(stderr, "CPU vendor 0x%04x, family %d, model %d\n", cpu_vendor, cpu_family, cpu_model);
		fprintf(stderr, "CPU name: %s\n", cpu_model_name);
	}

	parse_dmi(&dmi_vendor, &dmi_product, &dmi_board, &dmi_bios_version, &dmi_modalias);
	if (!dmi_vendor)
		dmi_vendor = dmi_empty;
	if (!dmi_product)
		dmi_product = dmi_empty;
	if (!dmi_board)
		dmi_board = dmi_empty;
	if (!dmi_bios_version)
		dmi_bios_version = dmi_empty;
	if (!dmi_modalias)
		dmi_modalias = dmi_empty;
	if (verbose) {
		fprintf(stderr, "DMI vendor '%s'\nDMI product '%s'\nDMI board '%s'\nDMI BIOS version '%s'\nDMI modalias '%s'\n",
				dmi_vendor, dmi_product, dmi_board, dmi_bios_version, dmi_modalias);
	}

	for (i = 0; i < SICOM_POSHW_N_IDS; i++) {
		sicom_poshw_checkbits_t checked;

		if (!sicom_poshw_checks[i].checks.bits)
			continue;

		checked.bits = 0;

		if (sicom_poshw_checks[i].checks.checks.vendor && sicom_poshw_checks[i].vendor) {
			if (sicom_poshw_checks[i].vendor_len)
				checked.checks.vendor = (strncasecmp(dmi_vendor, sicom_poshw_checks[i].vendor, sicom_poshw_checks[i].vendor_len) == 0);
			else
				checked.checks.vendor = (strcasecmp(dmi_vendor, sicom_poshw_checks[i].vendor) == 0);
		}

		if (sicom_poshw_checks[i].checks.checks.product && sicom_poshw_checks[i].product) {
			if (sicom_poshw_checks[i].product_len)
				checked.checks.product = (strncasecmp(dmi_product, sicom_poshw_checks[i].product, sicom_poshw_checks[i].product_len) == 0);
			else
				checked.checks.product = (strcasecmp(dmi_product, sicom_poshw_checks[i].product) == 0);
		}

		if (sicom_poshw_checks[i].checks.checks.board && sicom_poshw_checks[i].board) {
			if (sicom_poshw_checks[i].board_len)
				checked.checks.board = (strncasecmp(dmi_board, sicom_poshw_checks[i].board, sicom_poshw_checks[i].board_len) == 0);
			else
				checked.checks.board = (strcasecmp(dmi_board, sicom_poshw_checks[i].board) == 0);
		}

		if (sicom_poshw_checks[i].checks.checks.extra && sicom_poshw_checks[i].extra) {
			checked.checks.extra = sicom_poshw_checks[i].extra();
		}

		if (sicom_poshw_checks[i].checks.bits == checked.bits) {
			detectposhw_hwid = i;
			break;
		}
	}

	if (verbose)
		fprintf(stderr, "LIBSICOMDETECTPOSHW CONSTRUCTOR EXECUTED\n");

	if (dmi_vendor != dmi_empty)
		free(dmi_vendor);
	if (dmi_product != dmi_empty)
		free(dmi_product);
	if (dmi_board != dmi_empty)
		free(dmi_board);
	if (dmi_bios_version != dmi_empty)
		free(dmi_bios_version);
	if (dmi_modalias != dmi_empty)
		free(dmi_modalias);
}

int detectposhw_is_SICOMsl18(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_SL18);
}

int detectposhw_is_SICOMsl19(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_SL19);
}

int detectposhw_is_SICOMsl20(void) {
	/*
	 * We don't detect the prototype SL20 anymore,
	 * this will always return false.
	 */
	return (detectposhw_hwid == SICOM_POSHW_ID_SL20);
}

int detectposhw_is_SICOMsl20P(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_SL20P);
}

/* Not currently implemented*/
int detectposhw_is_IBM500(void) {
	return 0;
}

int detectposhw_is_DLI8300(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_DLI8300);
}

int detectposhw_is_MICROSsystem5(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_MICROSWS5);
}

int detectposhw_is_MICROSsystem4LX(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_MICROSWS4LX);
}

int detectposhw_is_MICROSpcws2010(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_MICROSPCWS2010);
}

int detectposhw_is_MICROSsystem5A(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_MICROSWS5A);
}

int detectposhw_is_LiteRay(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_LITERAY);
}

int detectposhw_is_PTsp1000(void) {
	return (detectposhw_hwid == SICOM_POSHW_ID_PTSP1000);
}

sicom_poshw_id_t detectposhw(void) {
	return detectposhw_hwid;
}

const char *detectposhw_get_hw_name(sicom_poshw_id_t hwid) {
	const char *name = NULL;

	if (hwid >= 0 && hwid < sizeof(poshw_names) / sizeof(char *))
		name = poshw_names[hwid];

	return name ? name : "UNKNOWN";
}

const char *detectposhw_get_version(void) {
	return VERSION;
}
