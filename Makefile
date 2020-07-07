
WORKDIR = `pwd`
URUSSTOOL_LIB=/system/urus/lib
WORKSPACEDIR=$(WORKDIR)
URUSSTOOL_BASE=/system/urus

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = 

INC = -I$(HOME)/boost_1_72_0 -Imodules/mumpi/deps/mumlib/include -Imodules/mumpi/include -I$(URUSSTOOL_BASE)/include/urusstudio/wxContribItems/wxled/led/include -Imodules/mumpi/deps/mumlib -I$(WORKSPACEDIR) -I../wimic
CFLAGS = -Wall -Wno-unused-variable -Wno-reorder -Wno-sign-compare -Wno-unused-local-typedefs -Wno-format -Wno-sequence-point -fpermissive -Wpointer-arith -Wno-missing-field-initializers -Wno-unused-parameter -Wno-redundant-decls -Wno-unknown-pragmas -Wno-trigraphs -ffunction-sections -fdata-sections -Wno-write-strings -D__URUSSTUDIO__ -D__LIB_URUSSTUDIO__ -DINPUT_STREAM_DISABLED -DPCM_FRAME=640
RESINC = 
LIBDIR = -L$(URUSSTOOL_LIB)/urusstudio/wxContribItems
LIB = -lportaudio -lconfig
LDFLAGS = 

INC_RELEASE_UNIX = $(INC)
CFLAGS_RELEASE_UNIX = $(CFLAGS) -O2 `wx-config --static=no --unicode=yes --debug=no --cflags` -std=gnu++11 -DBOOST_ERROR_CODE_HEADER_ONLY
RESINC_RELEASE_UNIX = $(RESINC)
RCFLAGS_RELEASE_UNIX = $(RCFLAGS)
LIBDIR_RELEASE_UNIX = $(LIBDIR) -Lmodules/mumpi/bin/Release -Lmodules/umurmur/bin/Release -Lmodules/mumpi/deps/mumlib/bin/Release
LIB_RELEASE_UNIX = $(LIB) -lumurmur -lwxled -lportaudio -llog4cpp -lconfig -lprotobuf-c -lprotobuf -lopus -lspeexdsp -lssl -lcrypto
LDFLAGS_RELEASE_UNIX =  -s `wx-config  --version=2.8 --static=no --unicode=yes --debug=no --libs` -Wl,-rpath=.:/system/urus/lib/urusstudio/wxContribItems:/system/urus/lib $(LDFLAGS)
OBJDIR_RELEASE_UNIX = obj/Release
DEP_RELEASE_UNIX = 
OUT_RELEASE_UNIX = bin/Release/wimic

OBJ_RELEASE_UNIX = $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/mumlib.o $(OBJDIR_RELEASE_UNIX)/wimic_Callback.o $(OBJDIR_RELEASE_UNIX)/wimicMain.o $(OBJDIR_RELEASE_UNIX)/wimicApp.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/main.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/RingBuffer.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/MumpiCallback.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/VarInt.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Transport.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/CryptState.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Callback.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Audio.o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/Mumble.pb.o

all: umurmur_unix release_unix

clean: clean_umurmur_unix clean_release_unix

before_release_unix: 
	protoc --cpp_out=. modules/mumpi/deps/mumlib/Mumble.proto
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src || mkdir -p $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src
	test -d $(OBJDIR_RELEASE_UNIX) || mkdir -p $(OBJDIR_RELEASE_UNIX)
	test -d $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src || mkdir -p $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src
	test -d $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib || mkdir -p $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib

after_release_unix: 

release_unix: before_release_unix out_release_unix after_release_unix

umurmur_unix:
	$(shell cd modules/umurmur/ && make -f Makefile.lib)

out_release_unix: before_release_unix $(OBJ_RELEASE_UNIX) $(DEP_RELEASE_UNIX)
	$(LD) $(LIBDIR_RELEASE_UNIX) -o $(OUT_RELEASE_UNIX) $(OBJ_RELEASE_UNIX)  $(LDFLAGS_RELEASE_UNIX) $(LIB_RELEASE_UNIX)

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/mumlib.o: modules/mumpi/deps/mumlib/src/mumlib.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/mumlib.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/mumlib.o

$(OBJDIR_RELEASE_UNIX)/wimic_Callback.o: wimic_Callback.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c wimic_Callback.cpp -o $(OBJDIR_RELEASE_UNIX)/wimic_Callback.o

$(OBJDIR_RELEASE_UNIX)/wimicMain.o: wimicMain.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c wimicMain.cpp -o $(OBJDIR_RELEASE_UNIX)/wimicMain.o

$(OBJDIR_RELEASE_UNIX)/wimicApp.o: wimicApp.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c wimicApp.cpp -o $(OBJDIR_RELEASE_UNIX)/wimicApp.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/main.o: modules/mumpi/src/main.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/src/main.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/main.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/RingBuffer.o: modules/mumpi/src/RingBuffer.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/src/RingBuffer.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/RingBuffer.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/MumpiCallback.o: modules/mumpi/src/MumpiCallback.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/src/MumpiCallback.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src/MumpiCallback.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/VarInt.o: modules/mumpi/deps/mumlib/src/VarInt.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/VarInt.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/VarInt.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Transport.o: modules/mumpi/deps/mumlib/src/Transport.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/Transport.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Transport.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/CryptState.o: modules/mumpi/deps/mumlib/src/CryptState.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/CryptState.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/CryptState.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Callback.o: modules/mumpi/deps/mumlib/src/Callback.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/Callback.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Callback.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Audio.o: modules/mumpi/deps/mumlib/src/Audio.cpp
	$(CXX) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/src/Audio.cpp -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src/Audio.o

$(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/Mumble.pb.o: modules/mumpi/deps/mumlib/Mumble.pb.cc
	$(CC) $(CFLAGS_RELEASE_UNIX) $(INC_RELEASE_UNIX) -c modules/mumpi/deps/mumlib/Mumble.pb.cc -o $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/Mumble.pb.o

clean_release_unix: 
	rm -f $(OBJ_RELEASE_UNIX) $(OUT_RELEASE_UNIX)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib/src
	rm -rf $(OBJDIR_RELEASE_UNIX)
	rm -rf $(OBJDIR_RELEASE_UNIX)/modules/mumpi/src
	rm -rf $(OBJDIR_RELEASE_UNIX)/modules/mumpi/deps/mumlib

clean_umurmur_unix:
	 $(shell cd modules/umurmur/ && make -f Makefile.lib clean)

.PHONY: umurmur_unix before_release_unix after_release_unix clean_release_unix clean_umurmur_unix
