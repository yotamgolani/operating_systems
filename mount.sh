#!/bin/bash
set -euo pipefail

# Config (override with env vars if you want)
URL="${URL:-http://127.0.0.1:9843}"
MNT="${MNT:-/mnt/share}"
UID_NAME="${UID_NAME:-student}"
GID_NAME="${GID_NAME:-student}"

# Ensure mount point exists
sudo mkdir -p "$MNT"

# Start the WebDAV helper if it's not up
if ! systemctl is-active --quiet spice-webdavd; then
  sudo systemctl start spice-webdavd
fi

# Skip if already mounted
if mountpoint -q "$MNT"; then
  echo "$MNT is already mounted."
  exit 0
fi

# Mount the Spice shared folder via davfs2
sudo mount -t davfs -o "uid=${UID_NAME},gid=${GID_NAME}" "$URL" "$MNT"
echo "Mounted $URL -> $MNT"