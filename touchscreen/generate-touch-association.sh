#!/system/bin/sh

MODEL=$(/vendor/bin/detectposhw 2>/dev/null)

setprop vendor.hw.model "$MODEL"

settings put global device_name "$MODEL"
