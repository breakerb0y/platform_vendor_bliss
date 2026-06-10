/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#ifndef __DETECTPOSHW_PRIVATE_H__
#define __DETECTPOSHW_PRIVATE_H__

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include <detectposhw.h>

/* Definitions */

#define LOCALSYM  __attribute__ ((visibility ("hidden")))

#define PRINT_HAS(x)	fprintf(stderr, "has_" #x ": %d\n", has_ ## x)

#define CPUINFO_VENDOR_ID "vendor_id\t: "
#define CPUINFO_VENDOR_ID_AMD "AuthenticAMD"
#define CPUINFO_VENDOR_ID_INTEL "GenuineIntel"

#define CPUINFO_CPU_FAMILY "cpu family\t: "
#define CPUINFO_CPU_FAMILY_22 "22"
#define CPUINFO_CPU_FAMILY_6 "6"

#define CPUINFO_CPU_MODEL "model\t\t: "

#define CPUINFO_CPU_MODEL_NAME "model name\t: "
#define CPUINFO_CPU_MODEL_NAME_GEODE	"Geode(TM) Integrated Processor by AMD PCS"
#define CPUINFO_CPU_MODEL_NAME_A4_5000 "AMD A4-5000 APU with Radeon(TM) HD Graphics"
#define CPUINFO_CPU_MODEL_NAME_GX_415GA "AMD GX-415GA SOC with Radeon(tm) HD Graphics"
#define CPUINFO_CPU_MODEL_NAME_RX_425BB "AMD RX-425BB with AMD Radeon(tm) R6 Graphics"
#define CPUINFO_CPU_MODEL_NAME_x5_z8300 "Intel(R) Atom(TM) x5-Z8300  CPU @ 1.44GHz"
#define CPUINFO_CPU_MODEL_NAME_D525_180 "Intel(R) Atom(TM) CPU D525   @ 1.80GHz"
#define CPUINFO_CPU_MODEL_NAME_D2550_186 "Intel(R) Atom(TM) CPU D2550   @ 1.86GHz"
#define CPUINFO_CPU_MODEL_NAME_ATOM_Z530 "Intel(R) Atom(TM) CPU Z530"
#define CPUINFO_CPU_MODEL_NAME_INTEL_1300	"Genuine Intel(R) processor              1300MHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_1300_2	"Intel(R) Celeron(R) M processor         1300MHz"
#define CPUINFO_CPU_MODEL_NAME_PENTIUM_1800 "Intel(R) Pentium(R) M processor 1.80GHz"
#define CPUINFO_CPU_MODEL_NAME_ATOM_N450_1660 "Intel(R) Atom(TM) CPU N450   @ 1.66GHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_N2930_1830 "Intel(R) Celeron(R) CPU  N2930  @ 1.83GHz"
#define CPUINFO_CPU_MODEL_NAME_COREI5_M520_2400 "Intel(R) Core(TM) i5 CPU       M 520  @ 2.40GHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_P4500_1870 "Intel(R) Celeron(R) CPU        P4500  @ 1.87GHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_N3350_1100 "Intel(R) Celeron(R) CPU N3350 @ 1.10GHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_J6412_2000 "Intel(R) Celeron(R) J6412 @ 2.00GHz"
#define CPUINFO_CPU_MODEL_NAME_INTEL_x6413E_1500 "Intel Atom(R) x6413E Processor @ 1.50GHz"

#define CPUINFO_CPU_MODEL_NAME2 "Model\t\t: "
#define CPUINFO_CPU_MODEL_NAME_RPI3BPLUS	"Raspberry Pi 3 Model B Plus"

#define DMI_TO_BE_FILLED			"To be filled by O.E.M."
#define DMI_NA						"NA"

#define PCI_VENDOR_AMD				0x1022
#define PCI_VENDOR_ATHEROS			0x168c
#define PCI_VENDOR_ATI				0x1002
#define PCI_VENDOR_CYRIX			0x1078
#define PCI_VENDOR_INTEL			0x8086
#define PCI_VENDOR_NVIDIA			0x10de
#define PCI_VENDOR_REALTEK			0x10ec
#define PCI_VENDOR_REDHAT			0x1b36
#define PCI_VENDOR_REDHAT2			0x1af4
#define PCI_VENDOR_VMWARE			0x15ad
#define PCI_VENDOR_INNOTEK			0x80ee
#define PCI_VENDOR_DUMMY			0x1234	/* Not actually a registered PCI vendor ID */

#define PCI_DEVICE_5530_LEGACY		0x0100
#define PCI_DEVICE_INTEL_IGPU		0x22b0
#define PCI_DEVICE_INTEL_VALLEYVIEW 0x0f31
#define PCI_DEVICE_INTEL_WIFI_3165	0x3165
#define PCI_DEVICE_INTEL_82577LM	0x10ea
#define PCI_DEVICE_INTEL_I225LM                 0x15f2
#define PCI_DEVICE_KABINI_1			0x9832
#define PCI_DEVICE_KABINI_2			0x9833
#define PCI_DEVICE_KAVERI_1			0x131d
#define PCI_DEVICE_INTEL_WIFI_3160	0x08b3
#define PCI_DEVICE_NV_ION			0x0a64
#define PCI_DEVICE_NV_GF119M		0x1050
#define PCI_DEVICE_NV_GF119			0x104a
#define PCI_DEVICE_AR9285			0x002b
#define PCI_DEVICE_INTEL_N_130		0x0896
#define PCI_DEVICE_INTEL_N_2230		0x0887
#define PCI_DEVICE_RTL8110			0x8167
#define PCI_DEVICE_RTL8111			0x8168
#define PCI_DEVICE_VIRTQXL			0x0100
#define PCI_DEVICE_VIRTIOGPU		0x1050
#define PCI_DEVICE_VIRTVGA			0x1111
#define PCI_DEVICE_VBOXSVGA			0xbeef
#define PCI_DEVICE_VMWSVGA2			0x0405

#define USB_VENDOR_ELO				0x04e7
#define USB_VENDOR_REALTEK			0x0bda
#define USB_VENDOR_DWAV				0x0eef

#define USB_DEVICE_2216_ACCUTOUCH	0x0050
#define USB_DEVICE_RTL8152			0x8152
#define USB_DEVICE_EXC3188			0xc000

/* Internal structure definitions  */

typedef bool (*check_extra_fn)(void);

typedef union sicom_poshw_checkbits_t {
	unsigned int bits;
	struct {
		unsigned int vendor:1;
		unsigned int product:1;
		unsigned int board:1;
		unsigned int extra:1;
	} checks;
} sicom_poshw_checkbits_t;

typedef struct sicom_poshw_checks_t {
	char *vendor;
	char *product;
	char *board;
	size_t vendor_len;
	size_t product_len;
	size_t board_len;
	check_extra_fn extra;
	sicom_poshw_checkbits_t checks;
} sicom_poshw_checks_t;

/* Static inline functions */

static inline bool rest_is_spaces(char *str) {
	while (*str) {
		if (!isspace(*str))
			return false;
		str++;
	}

	return true;
}

/* External references for internal variables */

extern bool has_usb_elo_touch;
extern bool has_usb_rtl8152;
extern bool has_usb_egalax_exc3188;
extern bool has_cyrix_5530;
extern bool has_kabini;
extern bool has_kaveri;
extern bool has_intel_igpu;
extern bool has_intel_wifi_3160;
extern bool has_intel_wifi_3165;
extern bool has_intel_wifi_n130;
extern bool has_intel_wifi_n2230;
extern bool has_intel_82577lm;
extern bool has_intel_i225lm;
extern bool has_atheros_wifi;
extern bool has_rtl8110;
extern bool has_rtl8111;
extern bool has_nv_ion;
extern bool has_nv_gf119m;
extern bool has_nv_gf119;
extern bool has_virtqxl;
extern bool has_virtiogpu;
extern bool has_virtvga;
extern bool has_vboxsvga;
extern bool has_vmsvga2;
extern int verbose;

/* Generic hardware detection parameters */
extern int cpu_vendor, cpu_family, cpu_model;
extern char cpu_model_name[128];
extern char dmi_empty[1];
extern char *dmi_vendor;
extern char *dmi_product;
extern char *dmi_board;
extern char *dmi_bios_version;
extern char *dmi_modalias;

extern const char *poshw_names[SICOM_POSHW_N_IDS];
extern const sicom_poshw_checks_t sicom_poshw_checks[SICOM_POSHW_N_IDS];
extern sicom_poshw_id_t detectposhw_hwid;

/* Internal utility functions */

void parse_cpuinfo(const char *cpuinfo, int *vendor, int *family, int *model, char *model_name, int model_name_sz) LOCALSYM;
void read_dmi_file(char *filename, char **dest) LOCALSYM;
void parse_dmi(char **vendor, char **product, char **board, char **bios_version, char **modalias) LOCALSYM;

/* Prototypes of internal detection functions */

/* POS hardware types */

bool sl18_check(void) LOCALSYM;
bool sl19_check(void) LOCALSYM;
bool microsws5_check(void) LOCALSYM;
bool microsws4lx_check(void) LOCALSYM;
bool microsws5a_check(void) LOCALSYM;
bool microspcws2010_check(void) LOCALSYM;
bool sl21_check(void) LOCALSYM;
bool sl21_check2(void) LOCALSYM;
bool sl21b_check(void) LOCALSYM;
bool sl21b_check2(void) LOCALSYM;
bool tosh4852_check(void) LOCALSYM;
bool xc23_check(void) LOCALSYM;

/* Mini PC hardware */

bool dli8300_check(void) LOCALSYM;
bool gbox_check(void) LOCALSYM;
bool picobox_check(void) LOCALSYM;
bool zboxid41_check(void) LOCALSYM;
bool zboxid84_check(void) LOCALSYM;
bool zboxid86_check(void) LOCALSYM;
bool kscapl_check(void) LOCALSYM;
bool mp10_check(void) LOCALSYM;
bool mp30_check(void) LOCALSYM;
bool ark1221l_check(void) LOCALSYM;

/* Big PC hardware */

bool dop3040_check(void) LOCALSYM;
bool xenialdtc_check(void) LOCALSYM;

/* Exotic hardware */

bool mp500_check(void) LOCALSYM;
bool mp500bdbc_check(void) LOCALSYM;
bool nc2100_check(void) LOCALSYM;
bool rpi3bplus_check(void) LOCALSYM;

/* Virtual hardware */

bool kvmqxl_check(void) LOCALSYM;
bool kvmvirtio_check(void) LOCALSYM;
bool kvmvga_check(void) LOCALSYM;
bool virtualbox_check(void) LOCALSYM;
bool vboxvmsvga_check(void) LOCALSYM;

#endif
