#!/bin/sh
### BEGIN INIT INFO
# Provides:             wimic_service.sh
# Should-Start:         console-screen kbd acpid dbus hal consolekit
# Required-Start:       $local_fs $remote_fs x11-common
# Required-Stop:        $local_fs $remote_fs
# Default-Start:        S 2 3 4 5
# Default-Stop:         0 1 6
# Short-Description: 	WiMic Server/Client service
# Description:       	WiMic Server/Client service
### END INIT INFO

export PATH=/system/urus/bin:/bin:/sbin:/usr/bin:/usr/sbin:$PATH
export DISPLAY=:0


PLATOS=`cat /etc/os-release | grep -rwi - -e "ID" | cut -f2 -d=`
DEV=1
MIC_DISABLED=`cat /system/urus/slotdata/wimic_mic_disabled.txt 2>/dev/null`
#if [ "$MIC_DISABLED" = 1 ] || [ ! -n $MIC_DISABLED ] ; then
#printf "disabled %d\n" "$MIC_DISABLED"
#fi
#exit 0
DELAYSTART=10

START=$1
if [ "$1" = "/etc/profile" ] ; then
START=start
fi
echo $START > /system/urus/slotdata/log.txt
case "$START" in
#case "$1" in
	start)
	if [ "x$SSH_CLIENT" = "x" ] || [ "x$SSH_TTY" = "x" ] ; then
	printf "Starting WiMic...\n"
		if [ "$MIC_DISABLED" = 1 ] || [ ! -n $MIC_DISABLED ] ; then
			sudo rmmod snd-aloop 2>/dev/null
		else
			DEV=5
			sudo modprobe snd-aloop 2>/dev/null
		fi
		#export DISPLAY=:0
		#su -p - "$(id -nu 1000)" -c "Xorg :1 2>/dev/null & sleep $DELAYSTART && /system/urus/bin/wimic -a &>/dev/null & sleep 2 && metacity 2>/dev/null & sleep 2 && chvt 7 &" &
		#su - "$(id -nu 1000)" -lc "sleep $DELAYSTART && /system/urus/bin/wimic -a &>/dev/null &"
		if [ `printf "$PLATOS" | grep -ri - -e "raspbian" | wc -l` -gt 0 ] ; then
			sh -c "sleep $DELAYSTART && DISPLAY=:0 /system/urus/bin/wimic -d $DEV -s -a &>/dev/null &"
		elif [ `printf "$PLATOS" | grep -ri - -e "ubuntu" | wc -l` -gt 0 ] ; then
			sh -c "sleep $DELAYSTART && DISPLAY=:0 /system/urus/bin/wimic -a &>/dev/null &"
		fi
	fi
		;;
	stop)
		sudo killall -s KILL wimic
		;;
	restart)
		stop
		start
		;;
	status)
		;;
	*)
		echo "Usage: $0 {start|stop|status|restart}"
esac
