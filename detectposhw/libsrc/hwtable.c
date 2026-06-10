/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <detectposhw.h>
#include "detectposhw-private.h"

const char *poshw_names[SICOM_POSHW_N_IDS] LOCALSYM = {
	[SICOM_POSHW_ID_SL18] = "SICOMsl18",
	[SICOM_POSHW_ID_SL19] = "SICOMsl19",
	[SICOM_POSHW_ID_SL20] = "SICOMsl20",
	[SICOM_POSHW_ID_SL20P] = "SICOMsl20P",
	[SICOM_POSHW_ID_IBM500] = "IBM500",
	[SICOM_POSHW_ID_DLI8300] = "DLI8300",
	[SICOM_POSHW_ID_MICROSWS4LX] = "MICROSsystem4LX",
	[SICOM_POSHW_ID_MICROSWS5] = "MICROSsystem5",
	[SICOM_POSHW_ID_MICROSWS5A] = "MICROSsystem5A",
	[SICOM_POSHW_ID_MICROSPCWS2010] = "MICROSpcws2010",
	[SICOM_POSHW_ID_LITERAY] = "LiteRay",
	[SICOM_POSHW_ID_PTSP1000] = "PTsp1000",
	[SICOM_POSHW_ID_PTSP850] = "PTsp850",
	[SICOM_POSHW_ID_GBOX] = "GBOX",
	[SICOM_POSHW_ID_ZBOXID41] = "ZBOXID41",
	[SICOM_POSHW_ID_ZBOXID84] = "ZBOXID84",
	[SICOM_POSHW_ID_ZBOXID86] = "ZBOXID86",
	[SICOM_POSHW_ID_DELLBOX] = "DELLBOX",
	[SICOM_POSHW_ID_PICOBOX] = "PICOBOX",
	[SICOM_POSHW_ID_CI325] = "CI325",
	[SICOM_POSHW_ID_CI329] = "CI329",
	[SICOM_POSHW_ID_NDIS163] = "NDIS163",
	[SICOM_POSHW_ID_MP10] = "XENIAL_MP10",
	[SICOM_POSHW_ID_NCR_P1230] = "NCR_P1230",
	[SICOM_POSHW_ID_SL20A] = "SL20A",
	/*
	 * The two Flytech variants of:
	 * - POS335
	 * - POS455
	 * are the same mostly except in the number of
	 * cash drawer wired in. Let them be handled
	 * the same way from scripts by providing the
	 * same name.
	 */
	[SICOM_POSHW_ID_FLTPOS335] = "SICOMsl21",
	[SICOM_POSHW_ID_FLTPOS335_CD2] = "SICOMsl21",
	[SICOM_POSHW_ID_SL21B] = "SICOMsl21B",
	[SICOM_POSHW_ID_SL21B_CD2] = "SICOMsl21B",
	/*
	 * Three variants of LG MP500
	 */
	[SICOM_POSHW_ID_LGMP500] = "LGMP500",
	[SICOM_POSHW_ID_LGMP500BDBC] = "LGMP500BDBC",
	[SICOM_POSHW_ID_LGNC2100] = "LGNC2100",
	/*
	 * Dell OptiPlex 3040 (Kiosk hardware)
	 */
	[SICOM_POSHW_ID_DOP3040] = "DOP3040",
	[SICOM_POSHW_ID_TOSH_4852E70] = "TOSH4852E70",
	[SICOM_POSHW_ID_SLK27] = "SICOMslK27",
	[SICOM_POSHW_ID_RPI3BPLUS] = "RPI3BPLUS",
	[SICOM_POSHW_ID_MINIX] = "MINIX",
	[SICOM_POSHW_ID_DT166] = "DT166",
	[SICOM_POSHW_ID_VIRTUALBOX] = "VIRTUALBOX",
	[SICOM_POSHW_ID_KVMQXL] = "KVMQXL",
	[SICOM_POSHW_ID_KVMVIRTIOGPU] = "KVMVIRTIOGPU",
	[SICOM_POSHW_ID_KVMVGA] = "KVMVGA",
	[SICOM_POSHW_ID_FLTK737] = "K737",
	[SICOM_POSHW_ID_XENIALDTC] = "XENIALDTC",
	[SICOM_POSHW_ID_TDQUEST10] = "TDQUEST10",
	[SICOM_POSHW_ID_XENIALKSCAPL] = "XENIALKSCAPL",
	[SICOM_POSHW_ID_XENIALXC23] = "XENIALXC23",
	[SICOM_POSHW_ID_VMWARE] = "VMWARE",
	[SICOM_POSHW_ID_MP30] = "XENIAL_MP30",
	[SICOM_POSHW_ID_VBOXVMSVGA] = "VBOXVMSVGA",
	[SICOM_POSHW_ID_ARK1221L] = "ARK1221L",
	[SICOM_POSHW_ID_GC26] = "GC26",
};

const sicom_poshw_checks_t sicom_poshw_checks[SICOM_POSHW_N_IDS] LOCALSYM = {
	[SICOM_POSHW_ID_SL18] = { .extra = sl18_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_SL19] = { .extra = sl19_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MICROSWS5] = { .extra = microsws5_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MICROSWS5A] = { .extra = microsws5a_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MICROSWS4LX] = { .extra = microsws4lx_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MICROSPCWS2010] = { .extra = microspcws2010_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_SL20P] = { .vendor = "SI", .product = "SL20", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_SL20A] = { .vendor = "SI", .product = "SL20A", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_DLI8300] = { .extra = dli8300_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_LITERAY] = { .product = "JS-925WS-05", .checks = { .checks = { .vendor = true } } },
	[SICOM_POSHW_ID_PTSP1000] = { .vendor = "Partner", .product = "SP1000", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_PTSP850] = { .vendor = "Partner", .product = "S1-J1900", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_GBOX] = { .extra = gbox_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MINIX] = { .vendor = "MINIX", .vendor_len = sizeof("MINIX") - 1,
									.board = "NEO Z83-4A", .board_len = sizeof("NEO Z83-4A") - 1,
									.checks = { .checks = { .vendor = true, .board = true } } },
	[SICOM_POSHW_ID_DT166] = { .vendor = "DTRI", .product = "DT166CR", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_LGMP500] = { .vendor = "LG Electronics", .vendor_len = sizeof("LG Electronics") - 1,
									.product = "LG SUPERSIGN", .product_len = sizeof("LG SUPERSIGN") - 1,
									.extra = mp500_check,
									.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_LGMP500BDBC] = { .vendor = "LG Electronics Inc.", .vendor_len = sizeof("LG Electronics Inc.") - 1,
									.product = "NC1100", .product_len = sizeof("NC1100") - 1,
									.extra = mp500bdbc_check,
									.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_LGNC2100] = { .vendor = "LG Electronics Inc.", .vendor_len = sizeof("LG Electronics Inc.") - 1,
									.product = "NC1100", .product_len = sizeof("NC1100") - 1,
									.extra = nc2100_check,
									.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_CI329] = { .vendor = "ZOTAC", .vendor_len = sizeof("ZOTAC") - 1, .product = "ZBOX-CI329NANO", .product_len = sizeof("ZBOX-CI329NANO") - 1,
									.checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_CI325] = { .vendor = "ZOTAC", .vendor_len = sizeof("ZOTAC") - 1, .product = "ZBOX-CI325NANO", .product_len = sizeof("ZBOX-CI325NANO") - 1,
									.checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_NDIS163] = { .board = "Cantiga", .board_len = sizeof("Cantiga") - 1, .checks = { .checks = { .board = true } } },
	[SICOM_POSHW_ID_MP10] = { .extra = mp10_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_MP30] = { .extra = mp30_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_DELLBOX] = { .vendor = "Dell Inc.", .product = "Inspiron 410", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_NCR_P1230] = { .vendor = "Radiant Systems, Inc", .product = "P1230", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_FLTPOS335] = { .extra = sl21_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_FLTPOS335_CD2] = { .extra = sl21_check2, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_SL21B] = { .extra = sl21b_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_SL21B_CD2] = { .extra = sl21b_check2, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_DOP3040] = { .vendor = "Dell Inc.", .extra = dop3040_check, .checks = { .checks = { .vendor = true, .extra = true } } },
	[SICOM_POSHW_ID_TOSH_4852E70] = { .vendor = "TOSHIBA", .extra = tosh4852_check, .checks = { .checks = { .vendor = true, .extra = true } } },
	[SICOM_POSHW_ID_SLK27] = { .vendor = "TOSHIBA", .product = "SLK27", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_RPI3BPLUS] = { .extra = rpi3bplus_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_VIRTUALBOX] = { .extra = virtualbox_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_KVMQXL] = { .extra = kvmqxl_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_KVMVIRTIOGPU] = { .extra = kvmvirtio_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_KVMVGA] = { .extra = kvmvga_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_FLTK737] = { .vendor = "Xenial Inc.", .product = "2300-TRM", .checks = { .checks = { .vendor = true, .product = true  } } },
	[SICOM_POSHW_ID_XENIALDTC] = { .extra = xenialdtc_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_TDQUEST10] = { .vendor = "Touch Dynamic Inc.", .product = "Quest 10", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_XENIALKSCAPL] = { .extra = kscapl_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_XENIALXC23] = { .extra = xc23_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_VMWARE] = { .vendor = "VMware, Inc.", .product = "VMware Virtual Platform", .checks = { .checks = { .vendor = true, .product = true } } },
	[SICOM_POSHW_ID_VBOXVMSVGA] = { .extra = vboxvmsvga_check, .checks = { .checks = { .extra = true } } },
	[SICOM_POSHW_ID_ARK1221L] = { .extra = ark1221l_check, .checks = { .checks = { .extra = true } } },

	/*
	 * Devices with nondescript DMI strings
	 *
	 * Such strings may be either "To be filled by O.E.M." or
	 * "NA" (not applicable)
	 *
	 * We can only do deep device detection that may cause
	 * false positives in the long run.
	 *
	 * ZOTAC kept the default placeholder vendor/product strings for ID41
	 * and replaced it with "NA" (Not Applicable) for ID84 and ID86. (See below.)
	 * Neither is conclusive info, so hardware detection is also necessary.
	 *
	 * If any of the ZBOX models had hardware changes during their lifetime,
	 * the current detection will fail for some of them.
	 */
	[SICOM_POSHW_ID_PICOBOX] = { .vendor = DMI_TO_BE_FILLED, .product = DMI_TO_BE_FILLED, .extra = picobox_check,
								.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_ZBOXID41] = { .vendor = DMI_TO_BE_FILLED, .product = DMI_TO_BE_FILLED, .extra = zboxid41_check,
								.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_ZBOXID84] = { .vendor = DMI_NA, .product = DMI_NA, .extra = zboxid84_check,
								.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	[SICOM_POSHW_ID_ZBOXID86] = { .vendor = DMI_NA, .product = DMI_NA, .extra = zboxid86_check,
								.checks = { .checks = { .vendor = true, .product = true, .extra = true } } },
	/*
	 * The internal SL20 POS prototype has false positive
	 * report when detectposhw is running on a ZBOX ID41
	 * because neither have their DMI strings properly set up.
	 * The SL20 prototype detection never went further than
	 * the DMI string match but this POS model was never put
	 * into production. The detection code can be deprecated.
	 * Let's start with not reporting it by the detectposhw
	 * executable. Removing the detectposhw_is_SICOMsl20()
	 * function from the library would need code changes
	 * elsewhere, too.
	 */
	[SICOM_POSHW_ID_SL20] = {},

	[SICOM_POSHW_ID_GC26] = { .vendor = "Global Payments", .product = "GC26", .board = "G99",
							.checks = { .checks = { .vendor = true, .product = true, .board = true } } },
};

sicom_poshw_id_t detectposhw_hwid LOCALSYM = SICOM_POSHW_ID_UNKNOWN;
