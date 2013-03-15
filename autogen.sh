#!/bin/sh

set -e
# get version number
if sh ./version.sh; then
    # generate configuration files
    autoreconf -i --force
    rm -rf autom4te.cache
else
    echo "Error: 'sh ./version.sh' failed."
    exit 1
fi
