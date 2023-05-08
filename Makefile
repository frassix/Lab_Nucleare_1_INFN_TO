CC=g++
ROOTFLAGS=`/CERN/root-build/bin/root-config --cflags --libs`
# FLAGS=-I. -l CAENVME -l CAENComm ${ROOTFLAGS} -D LINUX -Wall -g -fno-omit-frame-pointer
FLAGS=-I /usr/include -I. -I /CERN/root-build/include/ -l CAENVME ${ROOTFLAGS} -D LINUX 
BLD=build_files

build_files/%.o: %.c %.h
	$(CC) -c -o $@ $< $(FLAGS)

Acquisition.exe: build_files/read_all_no_verbose_pattern_V1718.o build_files/VME_function.o

.PHONY: clean all

all: Acquisition.exe


clean:
	rm -f build_files/*.o Acquisition.exe