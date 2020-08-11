#!/bin/sh

URUSSPATH=/system/urus/bin/
export CLONE_URUSSTUDIO=0

./install_deps.sh

cp -f ./bin/Release/wimic $URUSSPATH
cp -f ./wimic.conf  $URUSSPATH
