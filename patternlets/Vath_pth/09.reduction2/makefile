# This makefile builds the reduction2 example
# ===========================================

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

reduction2: reduction2.o  BigInt.o
	${CXX} -oreduction2 BigInt.o reduction2.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f reduction2
 
