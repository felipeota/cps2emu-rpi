NAME = cps2run
DEVKIT = d:/Language/devkitGP2X
CONFIG = cps2_config
CROSS_COMPILE = $(DEVKIT)/bin/arm-linux-

INC_BASE = -I$(DEVKIT)/include -I$(DEVKIT)/include/SDL -I$(DEVKIT)/sysroot/usr/include
LIB_BASE = -L$(DEVKIT)/lib -L$(DEVKIT)/sysroot/usr/lib

CFLAGS =  -Wall -D__GP2X__ -D__HAYMINIMAL__ -I minimal -I readpng -I minizip $(INC_BASE)
CFLAGS += -DCONFIG_FILE="\"$(CONFIG)\""

vpath %.c src:minimal:readpng:minizip
vpath %.h src:minimal:readpng:minizip

#CC = $(CROSS_COMPILE)gcc
CC = gcc
STRIP = #$(CROSS_COMPILE)strip

# CFLAGS += -O3
LDFLAGS = $(LIB_BASE) -static
LIBS = -lz -lpng -lm

TARGET = $(NAME)
OBJS = selector.o minimal.o lang.o readpng.o unzip.o ioapi.o

ALL_TARGETS = $(TARGET) 

all : $(ALL_TARGETS)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	$(STRIP) $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
