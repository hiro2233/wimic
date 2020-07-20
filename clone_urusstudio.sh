#! /bin/sh

if [ ! -e "UrusStudio" ] ; then
	git clone https://github.com/UrusTeam/UrusStudio.git &>/dev/null || true
	cd UrusStudio
	git submodule init && git submodule update modules/wxWidgets
	cd ..
fi

exit 0
