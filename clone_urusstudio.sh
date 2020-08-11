#! /bin/sh

if [ -e "UrusStudio" ] || [ "$CLONE_URUSSTUDIO" = 1 ] ; then
	git clone https://github.com/UrusTeam/UrusStudio.git
	cd UrusStudio
	git submodule init && git submodule update modules/wxWidgets
	cd ..
fi

echo "Clone done"

exit 0
