#! /bin/sh

if [ "$NO_URUSSTUDIO_MAKE_CMD" = "no" ] ; then
	export NO_URUSSTUDIO_MAKE_CMD=""
else
	export NO_URUSSTUDIO_MAKE_CMD="yes"
fi

cd UrusStudio && ./build-ci.sh
