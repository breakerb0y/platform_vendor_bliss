#!/system/bin/sh

MODEL=$(/system/bin/detectposhw 2>/dev/null)
setprop vendor.hw.model "$MODEL"
