# Inherit mobile full common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/common_mobile_full.mk)

# Inherit tablet common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/tablet.mk)

$(call inherit-product, vendor/gpdroid/config/wifionly.mk)
