#!/bin/sh

if GIT_VERSION=`git describe --always`; then
    echo "m4_define([_PACKAGE_VERSION], [${GIT_VERSION#v}])" > m4/version.m4.$$
    mv m4/version.m4.$$ m4/version.m4
    rm -f m4/version.m4.$$
else
    echo "Error: 'git describe --always' failed"
    exit 1
fi
