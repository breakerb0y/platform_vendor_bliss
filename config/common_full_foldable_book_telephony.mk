# Inherit mobile full common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/common_mobile_full.mk)

# Enable support of one-handed mode
PRODUCT_PRODUCT_PROPERTIES += \
    ro.support_one_handed_mode?=true

# Inherit tablet common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/tablet.mk)

$(call inherit-product, vendor/gpdroid/config/telephony.mk)

PRODUCT_PACKAGE_OVERLAYS += vendor/gpdroid/overlay/foldable_book
