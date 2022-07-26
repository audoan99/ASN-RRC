INCLUDE_LIB += -I./asn/

SRC_LIB += ./asn/*.c

INCLUDE += -I./network/ \
			-I./ue/ \
			-I./function/ \
			-I./socketudp/ \
			-I./log/

SRC += ./network/*.cpp \
		./ue/*.cpp \
		./function/*.cpp \
		./socketudp/*.cpp \
		./log/*.cpp \
		./main.cpp

.PHONY: build_lib build run clean_log clean

build_lib:
	gcc -O2 -nostartfiles $(INCLUDE_LIB) $(SRC_LIB) -c
	mv ./*.o ./library/
build: build_lib
	g++ -g -pthread $(INCLUDE) $(SRC) $(INCLUDE_LIB) ./library/*.o -o main 

run:
	./main

clean_log:
	rm ./*.log

clean:
	rm ./*.log
	rm ./library/*.o
	rm main