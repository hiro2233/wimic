#!/bin/sh

export PATH=/system/urus/bin:/bin:/sbin:/usr/bin:/usr/sbin:$PATH
export DISPLAY=:0

DEV=1
MIC_DISABLED=`cat /system/urus/slotdata/wimic_mic_disabled.txt 2>/dev/null`
#if [ "$MIC_DISABLED" = 1 ] || [ ! -n $MIC_DISABLED ] ; then
#printf "disabled %d\n" "$MIC_DISABLED"
#fi
#exit 0
DELAYSTART=10

if [ "x$SSH_CLIENT" = "x" ] || [ "x$SSH_TTY" = "x" ] ; then
printf "Starting WiMic...\n"
	if [ "$MIC_DISABLED" = 1 ] || [ ! -n $MIC_DISABLED ] ; then
		sudo rmmod snd-aloop 2>/dev/null
	else
		DEV=5
		sudo modprobe snd-aloop
	fi
sh -c "sleep $DELAYSTART && exec /system/urus/bin/wimic -d $DEV -s -a &>/dev/null" &
fi
