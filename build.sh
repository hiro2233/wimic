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

WXCONFURUS=$(wx-config --version)

echo $WXCONFURUS

if [ `printf "$WXCONFURUS" | grep -ri - | wc -l` -eq 0 ] ; then
	PUSHD=$(pwd)
	cd UrusStudio && ./build-ci.sh
	cd $PUSHD
	cd ./UrusStudio/buildustd/src/plugins/contrib/wxContribItems/wxled && make -j2 && make install
	cd $PUSHD
fi

exit 0
