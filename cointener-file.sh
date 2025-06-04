if [ $# -lt 2 ]; then
    echo "Usage: $0 conf-file command [args...]"
    exit 1
fi

CONF_FILE=$1
COMMAND=$2
WORKDIR=$(mktemp -d)

cleanup() {
    for mount in $(mount | grep "$WORKDIR" | awk '{print $3}' | sort -r); do
        umount "$mount"
    done
    rm -rf "$WORKDIR"
}

trap cleanup EXIT

while IFS= read -r line || [[ -n "$line" ]]; do
    [[ -z "$line" || "$line" =~ ^# ]] && continue
    
    ORIGIN=$(echo "$line" | awk '{print $1}')
    DEST="$WORKDIR$(echo "$line" | awk '{print $2}')"
    
    mkdir -p "$(dirname "$DEST")"
    
    if [ -d "$ORIGIN" ]; then
        mkdir -p "$DEST"
        bindfs --no-allow-other "$ORIGIN" "$DEST"
    elif [ -f "$ORIGIN" ]; then
        cp "$ORIGIN" "$DEST"
        chmod +x "$DEST" 2>/dev/null  
    else
        echo "Warning: $ORIGIN not found, skipping..."
    fi
done < "$CONF_FILE"

fakechroot chroot "$WORKDIR" "${@:2}"
