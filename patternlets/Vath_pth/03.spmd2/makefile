
# This makefile builds the spmd2 example
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
spmd2: spmd2.o
	${CXX} -ospmd2 spmd2.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f spmd2
