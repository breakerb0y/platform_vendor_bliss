# Copyright (C) 2017 Unlegacy-Android
# Copyright (C) 2017,2020 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# -----------------------------------------------------------------
# BlissRoms OTA update package

BLISS_TARGET_PACKAGE := $(PRODUCT_OUT)/$(BLISS_BUILD_ZIP).zip

SHA256 := prebuilts/build-tools/path/$(HOST_PREBUILT_TAG)/sha256sum

.PHONY: blissify
blissify: $(DEFAULT_GOAL) $(INTERNAL_OTA_PACKAGE_TARGET)
	$(hide) ln -f $(INTERNAL_OTA_PACKAGE_TARGET) $(BLISS_TARGET_PACKAGE)
	$(hide) $(SHA256) $(BLISS_TARGET_PACKAGE) | sed "s|$(PRODUCT_OUT)/||" > $(BLISS_TARGET_PACKAGE).sha256
	
	@echo -e ${CL_CYN}" _______      ______     ______    ______     ______    ________   ______      "${CL_CYN}
	@echo -e ${CL_CYN}"/______/\    /_____/\   /_____/\  /_____/\   /_____/\  /_______/\ /_____/\     "${CL_CYN}
	@echo -e ${CL_CYN}"\::::__\/__  \:::_ \ \  \:::_ \ \ \:::_ \ \  \:::_ \ \ \__.::._\/ \:::_ \ \    "${CL_CYN}
	@echo -e ${CL_CYN}" \:\  /____/\ \:(_) \ \  \:\ \ \ \ \:(_) ) )_ \:\ \ \ \   \::\ \   \:\ \ \ \   "${CL_CYN}
	@echo -e ${CL_CYN}"  \:\:\_  _\/  \: ___\/   \:\ \ \ \ \: __  \ \ \:\ \ \ \  _\::\ \__ \:\ \ \ \  "${CL_CYN}
	@echo -e ${CL_CYN}"   \:\_\ \ \    \ \ \      \:\/.:| | \ \  \ \ \ \:\_\ \ \/__\::\__/\ \:\/.:| | "${CL_CYN}
	@echo -e ${CL_CYN}"    \_____\/     \_\/       \____/_/  \_\/ \_\/  \_____\/\________\/  \____/_/ "${CL_CYN}
	
	@echo -e ${CL_CYN}""${CL_CYN}
	@echo -e ${CL_CYN}"===========-GP Package Complete-==========="${CL_RST}
	@echo -e ${CL_CYN}"Zip: "${CL_MAG} $(BLISS_TARGET_PACKAGE)${CL_RST}
	@echo -e ${CL_CYN}"SHA256: "${CL_MAG}" `cat $(BLISS_TARGET_PACKAGE).sha256 | cut -d ' ' -f 1`"${CL_RST}
	@echo -e ${CL_CYN}"Size:"${CL_MAG}" `ls -lah $(BLISS_TARGET_PACKAGE) | cut -d ' ' -f 5`"${CL_RST}
	@echo -e ${CL_CYN}"==============================================="${CL_RST}
	@echo -e ""
