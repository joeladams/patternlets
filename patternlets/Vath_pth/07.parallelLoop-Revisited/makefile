# This makefile builds the parallelLoopRevisited example =================

#""""""""""""""""""""""""""""""""""""""""
# Some variables concerning library paths
# to find libvath
#
PATHV = -L../vath_pth
LBASE  = -lpthread

#  makefile-compiler
CXX = g++
CPPFLAGS = -I../vath_pth/include -Iinclude
CXXFLAGS = -g -O3

# ----------------< SPool codes > -------------
BigInt.o: BigInt.h

parallelLoopRevisited: parallelLoopRevisited.o  BigInt.o
	${CXX} -oparallelLoopRevisited BigInt.o parallelLoopRevisited.o ${PATHV} -lvath ${LBASE}
	rm *.o

clean:
	rm -f parallelLoopRevisited

