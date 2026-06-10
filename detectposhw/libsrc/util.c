/*
 * COPYRIGHT (C) 2013-2026 SICOM Systems Inc
 * 1684 S. Broad Street Suite 300
 * Lansdale, PA 19446
 * 215-489-2500, FAX: 215-489-2769
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "detectposhw-private.h"

#define LINELEN (4096)

void parse_cpuinfo(const char *cpuinfo, int *vendor, int *family, int *model, char *model_name, int model_name_sz) {
	FILE *fd;
	char *line;
	bool has_cpu_vendor = false, has_cpu_family = false, has_cpu_model = false, has_cpu_model_name = false, has_cpu_model_name2 = false;

	fd = fopen(cpuinfo, "r");
	if (fd == NULL)
		return;

	line = malloc(LINELEN);

	while (!feof(fd)) {
		char *result = fgets(line, LINELEN, fd);
		char *ptr;
		int len;

		if (!result)
			break;

		len = strlen(line);
		while (len && (line[len - 1] == 0x0a || line[len - 1] == 0x0d))
			line[len-- - 1] = 0x00;

		if (has_cpu_vendor == 0 && strncmp(line, CPUINFO_VENDOR_ID, sizeof(CPUINFO_VENDOR_ID) - 1) == 0) {
			has_cpu_vendor = true;
			ptr = line + sizeof(CPUINFO_VENDOR_ID) - 1;
			if (strcmp(ptr, CPUINFO_VENDOR_ID_AMD) == 0)
				*vendor = PCI_VENDOR_AMD;
			else if (strcmp(ptr, CPUINFO_VENDOR_ID_INTEL) == 0)
				*vendor = PCI_VENDOR_INTEL;
		} else if (has_cpu_family == 0 && strncmp(line, CPUINFO_CPU_FAMILY, sizeof(CPUINFO_CPU_FAMILY) - 1) == 0) {
			has_cpu_family = true;
			ptr = line + sizeof(CPUINFO_CPU_FAMILY) - 1;
			*family = atoi(ptr);
		} else if (has_cpu_model == 0 && strncmp(line, CPUINFO_CPU_MODEL, sizeof(CPUINFO_CPU_MODEL) - 1) == 0) {
			has_cpu_model = true;
			ptr = line + sizeof(CPUINFO_CPU_MODEL) - 1;
			*model = atoi(ptr);
		} else if (has_cpu_model_name == 0 && strncmp(line, CPUINFO_CPU_MODEL_NAME, sizeof(CPUINFO_CPU_MODEL_NAME) - 1) == 0) {
			has_cpu_model_name = true;
			ptr = line + sizeof(CPUINFO_CPU_MODEL_NAME) - 1;
			strncpy(model_name, ptr, model_name_sz - 1);
			model_name[model_name_sz - 1] = 0;
		} else if (has_cpu_model_name2 == 0 && strncmp(line, CPUINFO_CPU_MODEL_NAME2, sizeof(CPUINFO_CPU_MODEL_NAME2) - 1) == 0) {
			has_cpu_model_name2 = true;
			ptr = line + sizeof(CPUINFO_CPU_MODEL_NAME2) - 1;
			strncpy(model_name, ptr, model_name_sz - 1);
			model_name[model_name_sz - 1] = 0;
		}

		if ((has_cpu_vendor && has_cpu_family && has_cpu_model && has_cpu_model_name) ||
			has_cpu_model_name2)
			break;
	}

	free(line);
	fclose(fd);
}

void read_dmi_file(char *filename, char **dest) {
	int fd;
	int len;
	struct stat st;
	char *dst;

	if (stat(filename, &st)) {
		free(*dest);
		*dest = NULL;
		return;
	}

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		free(*dest);
		*dest = NULL;
		return;
	}

	dst = malloc(st.st_size + 1);
	if (!dst) {
		close(fd);
		return;
	}

	len = read(fd, dst, st.st_size);
	if (len < 0) {
		close(fd);
		free(*dest);
		*dest = NULL;
		return;
	}

	dst[len] = 0;

	while (len > 0 && (dst[len - 1] == 0x0a || dst[len - 1] == 0x0d))
		dst[len-- - 1] = 0x00;

	close(fd);

	*dest = dst;
}

void parse_dmi(char **vendor, char **product, char **board, char **bios_version, char **modalias) {
	read_dmi_file("/sys/class/dmi/id/sys_vendor", vendor);
	read_dmi_file("/sys/class/dmi/id/product_name", product);
	read_dmi_file("/sys/class/dmi/id/board_name", board);
	read_dmi_file("/sys/class/dmi/id/bios_version", bios_version);
	read_dmi_file("/sys/class/dmi/id/modalias", modalias);
}
