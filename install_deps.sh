#!/bin/sh

#if [ `id -u` != 0 ] ; then
#    printf "\n\nPlease run with root permission!\n\n"
#    return 127
#fi

#./update_fstab.sh

BASEPKG="automake libtool autoconf pkgconf libgtk2.0-dev zip libprotobuf-dev libprotobuf-c-dev libconfig-dev protobuf-compiler libboost-system-dev liblog4cpp5-dev libopus-dev libspeexdsp-dev portaudio19-dev mime-support shared-mime-info desktop-file-utils"

YESNOCMD=""
SCOWPWRCMD=""

BUILDARCH=`uname -m`
PLATOS=$(cat /etc/os-release | grep -ri - -e "ID")

if [ `printf "$PLATOS" | grep -ri - -e "raspbian" | wc -l` -gt 0 ] ; then
    SCOWPWRCMD="apt-get"
    YESNOCMD="-qy"
	BASEPKG="libssl1.0-dev "$BASEPKG
    #sudo add-apt-repository ppa:openjdk-r/ppa -y
    #sudo apt-get update $YESNOCMD
    #sudo apt-get install openjdk-8-jdk $YESNOCMD
    #sudo dpkg --add-architecture i386
    #apt-get update $YESNOCMD
    echo raspbian
elif [ `printf "$PLATOS" | grep -ri - -e "ubuntu" | wc -l` -gt 0 ] ; then
    SCOWPWRCMD="apt-get"
    YESNOCMD="-qy"
	BASEPKG="libssl-dev "$BASEPKG
    #sudo apt-get update $YESNOCMD
    #sudo apt-get install openjdk-8-jdk $YESNOCMD
    #sudo dpkg --add-architecture i386
    #apt-get update $YESNOCMD
    echo ubuntu
fi

sudo $SCOWPWRCMD $YESNOCMD update
sudo $SCOWPWRCMD $YESNOCMD install $BASEPKG
#sudo useradd -r wimic
#sudo usermod -a -G wimic $USER

#umask 022

if [ ! -d "/system/urus" ] || [ ! -e "/etc/profile.d/urusprofile.sh" ] ; then

    #export URUSINFSTAB=$(grep -sri /etc/fstab -e "/system/urus" | wc -l)

    #if [ "$URUSINFSTAB" != 0 ] ; then
    #    printf "URUS bind are present on /etc/fstab\n"
    #else
		sudo mkdir -p /system/urus
		sudo chown $USER:$USER -R /system
		sudo sh -c 'printf "#!/bin/sh\n" >> /etc/profile.d/urusprofile.sh'
        #printf "\n$(pwd)/system /system/urus none bind\n" >> /etc/fstab
        #sed -i "1i export PATH=/system/urus/bin:"$"PATH" /etc/profile
        #sed -i "1i export LD_LIBRARY_PATH=/system/urus/lib:"$"LD_LIBRARY_PATH" /etc/profile
        #sed -i "1i export ACLOCAL_FLAGS=-I/system/urus/share/aclocal:"$"ACLOCAL_FLAGS" /etc/profile
        #printf "export ACLOCAL_FLAGS=-I/system/urus/share/aclocal:"$"ACLOCAL_FLAGS\n" >> /etc/profile.d/urusprofile.sh
        #printf "export LD_LIBRARY_PATH=/system/urus/lib:"$"LD_LIBRARY_PATH\n" >> ~/.profile
        sudo sh -c 'printf "export PATH=/system/urus/bin:"''$''"PATH\n" >> /etc/profile.d/urusprofile.sh'
		#sudo sh -c 'printf /system/urus/'$BUILDARCH'-pc-linux-gnu/lib\n" >> /etc/ld.so.conf.d/$BUILDARCH-urus-linux-gnu.conf'
        sudo sh -c 'printf "/system/urus/lib\n" >> /etc/ld.so.conf.d/'$BUILDARCH'-urus-linux-gnu.conf'
        #printf "URUS path bind INSTALLED! on /etc/fstab\n"
    #fi
    #mount /system/urus
    . /etc/profile.d/urusprofile.sh
    . ~/.profile
	echo "Done"
else
	echo "File structure present"
fi

if [ ! -e "/etc/profile.d/wimic_service.sh" ] ; then
	sudo cp -f ./scripts/wimic_service.sh /etc/profile.d/
fi

if [ `printf "$PLATOS" | grep -ri - -e "raspbian" | wc -l` -gt 0 ] ; then
	if [ ! -e /etc/alsa/conf.d/10-wimic-device.conf ] ; then
		sudo mkdir -p /etc/alsa/conf.d
		sudo cp -f ./10-wimic-device.conf /etc/alsa/conf.d/10-wimic-device.conf
		echo "Installed wimic alsa device conf."
	fi
fi

if [ ! -e "/usr/share/applications/wimic.desktop" ] || [ ! -e "/usr/share/pixmaps/logo_wimic.xpm" ] ; then
sudo cp -f ./resources/wimic.desktop /usr/share/applications/
sudo cp -f ./resources/logo_wimic.xpm /usr/share/pixmaps/
sudo update-desktop-database
echo "Installed mimes"
fi

./clone_urusstudio.sh

exit 0
