#!/bin/bash
set -euo pipefail

MNT="${MNT:-/mnt/share}"

if mountpoint -q "$MNT"; then
  echo "Unmounting $MNT..."
  if ! sudo umount "$MNT"; then
    echo "Normal unmount failed; attempting lazy unmount..."
    sudo umount -l "$MNT"
  fi
  echo "Done."
else
  echo "$MNT is not mounted."
fi