debug = n

ifeq ($(debug), y)
# (1) Debug
	OPT = -g2
else
# (2) Production
	OPT = -O2
endif

CC = g++
AR = ar cru

CFLAGS = -std=c++11 $(OPT)
LDFLAGS = -lpthread -lstdc++
SOURCE = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SOURCE))
#OBJS = ListTest VectorTest

LIB = libmuduo_base.a

libmuduo_base.a: $(OBJS)
	$(AR) $@ $^

%.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $@ 

#$(OBJS) : % : %.o 
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all : $(LIB)  

clean :
	rm  *.o $(LIB)