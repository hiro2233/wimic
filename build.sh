#!/bin/sh

PATH=/system/urus/bin:$PATH
export PATH

git submodule sync
git submodule update --init --recursive

NO_URUSSTUDIO_MAKE_CMD=$NO_URUSSTUDIO_MAKE_CMD

if [ "$NO_URUSSTUDIO_MAKE_CMD" = "no" ] ; then
	export NO_URUSSTUDIO_MAKE_CMD=
else
	export NO_URUSSTUDIO_MAKE_CMD="yes"
fi

PUSHD=$(pwd)
cd UrusStudio && ./build-ci.sh
cd $PUSHD
cd ./UrusStudio/buildustd/src/plugins/contrib/wxContribItems/wxled && make -j2 && make install
cd $PUSHD

exit 0
