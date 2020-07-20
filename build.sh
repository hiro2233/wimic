#! /bin/sh

PATH=/system/urus/bin:$PATH
export PATH

NO_URUSSTUDIO_MAKE_CMD=$NO_URUSSTUDIO_MAKE_CMD

if [ "$NO_URUSSTUDIO_MAKE_CMD" = "no" ] ; then
	export NO_URUSSTUDIO_MAKE_CMD=
else
	export NO_URUSSTUDIO_MAKE_CMD="yes"
fi

cd UrusStudio && ./build-ci.sh
cd UrusStudio/buildustd/src/plugins/contrib/wxContribItems/wxled/ && make -j2 && make install
