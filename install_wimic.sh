#!/bin/sh

URUSSPATH=/system/urus/bin/
export CLONE_URUSSTUDIO=0

BITARCH=`uname -m`
OSKERNEL=`uname -s | awk '{print tolower($0)}'`
PLATOS=`cat /etc/os-release | grep -rwi - -e "ID" | cut -f2 -d=`
URLDOWNLOAD=https://github.com/hiro2233/wimic/releases/download
URLCHANGELOG=https://raw.githubusercontent.com/hiro2233/wimic/master/CHANGELOG

if [ ! -n $PLATOS ] || [ "x$PLATOS" != "x" ] ; then
TRIPLETNAME=$OSKERNEL-$PLATOS-$BITARCH
echo $TRIPLETNAME
else
TRIPLETNAME=$OSKERNEL$BITARCH
echo $TRIPLETNAME
fi

rm -f CHANGELOG.txt
wget -O CHANGELOG.txt $URLCHANGELOG
grep -ri CHANGELOG.txt -e "version pre-$TRIPLETNAME:" | awk '{print $3}' | cut -f2 -d: | head -n1 | xargs printf %s > version.txt
wget -c $URLDOWNLOAD/$(cat version.txt)/wimic.tar.gz
wget -c $URLDOWNLOAD/$(cat version.txt)/wimic.tar.gz.md5

if [ `md5sum -c wimic.tar.gz.md5 | grep -ri - -e 'ok' | wc -l` -eq 0 ] ; then
printf "MD5SUM mismatch!\nInstall stopped.\n\n"
return
fi

tar -xvzf wimic.tar.gz 2>/dev/null
cp -rf wimic/* . 2>/dev/null
rm -rf wimic 2>/dev/null

. install_deps.sh

mkdir -p $URUSSPATH

cp -f ./bin/Release/wimic $URUSSPATH
cp -f ./wimic.conf  $URUSSPATH
cp -rP lib $URUSSPATH/../

sudo ldconfig
