#!/system/bin/sh

CACHE_FILE="/data/system/sec_display_port"

PRIMARY=""
SECONDARY=""

CONNECTED_PORTS=""
ALL_PORTS=""

PORTS=$(ls /sys/class/drm/card0-*/status 2>/dev/null | sort)

for status_file in $PORTS; do
    port=$(basename $(dirname "$status_file"))
    status=$(cat "$status_file" 2>/dev/null)

    ALL_PORTS="$ALL_PORTS $port"

    if [ "$status" = "connected" ]; then
        CONNECTED_PORTS="$CONNECTED_PORTS $port"
    fi
done

for port in $CONNECTED_PORTS; do
    case "$port" in
        *eDP*|*LVDS*)
            PRIMARY="$port"
            break
            ;;
    esac
done

if [ -z "$PRIMARY" ]; then
    for port in $CONNECTED_PORTS; do
        PRIMARY="$port"
        break
    done
fi

for port in $CONNECTED_PORTS; do
    if [ "$port" != "$PRIMARY" ]; then
        SECONDARY="$port"
        break
    fi
done

if [ -z "$SECONDARY" ] && [ -f "$CACHE_FILE" ]; then
    SAVED_PORT=$(cat "$CACHE_FILE" 2>/dev/null)
    if [ -n "$SAVED_PORT" ] && [ "$SAVED_PORT" != "$PRIMARY" ]; then
        SECONDARY="$SAVED_PORT"
    fi
fi

if [ -z "$SECONDARY" ]; then
    for port in $ALL_PORTS; do
        if [ "$port" != "$PRIMARY" ]; then
            SECONDARY="$port"
            break
        fi
    done
fi

if [ -n "$SECONDARY" ]; then
    SYSFS_STATUS="/sys/class/drm/$SECONDARY/status"

    ORIG_PERM=$(stat -c "%a" "$SYSFS_STATUS" 2>/dev/null || echo "644")
    trap 'chmod "$ORIG_PERM" "$SYSFS_STATUS" 2>/dev/null' EXIT INT TERM

    chmod 666 "$SYSFS_STATUS" 2>/dev/null

    case "$1" in
        0)
            echo "Secondary display ($SECONDARY) OFF..."
            # Elmentjük a kikapcsolt port nevét a cache-be
            echo "$SECONDARY" > "$CACHE_FILE"
            echo off > "$SYSFS_STATUS"
            ;;
        1)
            echo "Setting Mirrored mode on $SECONDARY..."
            settings put global force_desktop_mode_on_external_displays 0
            echo off > "$SYSFS_STATUS"
            sleep 1
            echo detect > "$SYSFS_STATUS"
            rm -f "$CACHE_FILE" 2>/dev/null
            ;;
        2)
            echo "Setting Extended mode on $SECONDARY..."
            settings put global force_desktop_mode_on_external_displays 1
            echo off > "$SYSFS_STATUS"
            sleep 1
            echo detect > "$SYSFS_STATUS"
            rm -f "$CACHE_FILE" 2>/dev/null
            ;;
        *)
            echo "Usage: $0 {0|1|2}"
            exit 1
            ;;
    esac

else
    echo "Secondary display port not found."
    exit 1
fi

exit 0
