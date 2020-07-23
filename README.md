[![Build Status](https://semaphoreci.com/api/v1/hiro2233/wimic/branches/master/badge.svg)](https://semaphoreci.com/hiro2233/wimic) [![Build Status](https://travis-ci.org/hiro2233/wimic.svg?branch=master)](https://travis-ci.org/hiro2233/wimic) ![Hits](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2Fhiro2233%2Fwimic)

# WiMic Server/Client

*Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>*

**WiMic Server/Client**  is a multiplatform remote wireless microphone (when connect with client side and third party driver and as speaker without the driver) server and client for use on PC Desktop, portable devices platforms or IoTs like Raspberry Pi, GNU OS Dstributions (E.g. Linux), Arduinos, and more.

![](resources/wimic_ubuntu_16_04.png)

###### Dependencies

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
