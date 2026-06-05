# Inherit common Bliss stuff
$(call inherit-product, vendor/gpdroid/config/common.mk)

# Inherit Lineage car device tree
$(call inherit-product, device/lineage/car/lineage_car.mk)
