[![Build Status](https://semaphoreci.com/api/v1/hiro2233/wimic/branches/master/badge.svg)](https://semaphoreci.com/hiro2233/wimic) [![Build Status](https://travis-ci.org/hiro2233/wimic.svg?branch=master)](https://travis-ci.org/hiro2233/wimic) ![Hits](https://visitor-badge.laobi.icu/badge?page_id=github-hiro2233-wimic) ![GitHub all releases](https://img.shields.io/github/downloads/hiro2233/wimic/total)

# WiMic Server/Client

*Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>*

**WiMic Server/Client**  is a multiplatform remote wireless microphone (when connect with client side and third party driver and as speaker without the driver) server and client for use on PC Desktop, portable devices platforms or IoTs like Raspberry Pi, GNU OS Distributions (E.g. GNU/Linux), Arduinos, and more.

Official repo:
https://github.com/hiro2233/wimic

### Download Installer

| Host | Arch | Link |
| ---- | ---- | ---- |
| win  |  x86  | [![Download-win32](https://img.shields.io/github/downloads/hiro2233/wimic/v1.0.4-pre-win32/total?style=for-the-badge)](https://github.com/hiro2233/wimic/releases/download/v1.0.4-pre-win32/wimic_setup.exe)|
| raspbian | arm32 | ![Download-raspbian32](https://img.shields.io/github/downloads/hiro2233/wimic/v1.0.4-pre-linux-raspbian-armv7l/total?style=for-the-badge) |

[See instruction at bottom for a brief about installation.](#Basic-install-steps)

<br/>

###### WiMic Server/Client on Ubuntu 16.04

![wimic_ubuntu_16_04 | 1360x768,50%](resources/wimic_ubuntu_16_04.png)

<br/>

###### WiMic Server/Client on Raspberry Pi OS

![wimic_ubuntu_16_04 | 1360x768,50%](resources/wimic_raspberry.jpg)

<br/>

### Dependencies

- opus
- speexdsp
- portaudio
- libconfig
- boost-system
- log4cpp
- openssl
- Google Protobuf: libraries and compiler

### Building

###### (Debian/Ubuntu/Rasperry Pi OS)

```bash
git clone https://github.com/hiro2233/wimic.git
cd wimic
git submodule init
git submodule update --init --recursive
./install_deps.sh
```

exit from terminal, open again terminal then:

```bash
make
./bin/release/wimic
```

### WiMic Android Client App

You can connect to the server with WiMic Android App, this was designed and optimized to work with WiMic Server/Client.

<a href='https://play.google.com/store/apps/details?id=bo.htakey.wimic&pcampaignid=pcampaignidMKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' src='resources/google-play-badge.png' width="30%"/></a>

Official repo:

https://github.com/hiro2233/wimic_android

### Basic install steps

###### win32/win64

Download the [win host setup installer](#Download-Installer), then execute and follow the instruction on the window form.

###### (Rasperry Pi OS)

On terminal execute:

```bash
$ wget https://github.com/hiro2233/wimic/raw... 
$ chmod +x install_wimic.sh
$ source install_wimic.sh
```
and wait until this finish, then reboot raspberry and WiMic will autostart in service mode. By default WiMic server will use microphone on autostart mode, but if you want to use the speakers in autostart mode you can disable this doing:

```bash
$ echo 1 > /system/urus/slotdata/wimic_mic_disabled.txt
```

### Detailed install docs

Spanish:
https://hiro2233.github.io/wimic/docs/wimic/

English:
In progress... contributions are welcome!

### Video Tutorial

<a href='https://www.youtube.com/watch?v=EjYOHEgnx3Y'><img alt='WiMic install tutorial' src='https://img.youtube.com/vi/EjYOHEgnx3Y/0.jpg' width="50%"/></a>
