# Inherit mobile mini common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/common_mobile_mini.mk)

# Inherit tablet common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/tablet.mk)

$(call inherit-product, vendor/gpdroid/config/wifionly.mk)
