#!/bin/sh

mkdir wimic
cp -f wimic.conf wimic/
cp -f clone_urusstudio.sh wimic/
cp -f install_deps.sh wimic/
cp -f 10-wimic-device.conf wimic/

cp -rf resources wimic/
rm wimic/resources/*.jpg
rm wimic/resources/*.bmp
rm wimic/resources/*.png
rm wimic/resources/*.rc

cp -rf scripts wimic/

mkdir -p wimic/bin/Release
cp -f bin/Release/wimic wimic/bin/Release/

mkdir -p wimic/lib
cp -Pf /system/urus/lib/libwx_gtk2u_urus-2.8.so* wimic/lib/

if [ ! $(cp -pf /system/urus/lib/urusstudio/wxContribItems/libwxled.* wimic/lib/ 2>/dev/null) ] ; then
printf "System USTD isn't present\nTrying from cloned source.\n"
cp -Pf UrusStudio/buildustd/src/plugins/contrib/wxContribItems/wxled/.libs/* wimic/lib/
rm -rr wimic/lib/*.la*
fi

mkdir dist
tar -cvzf dist/wimic.tar.gz wimic
cd dist
md5sum wimic.tar.gz > wimic.tar.gz.md5
cd ..

echo Done
