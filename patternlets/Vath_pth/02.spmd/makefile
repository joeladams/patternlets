
# This makefile builds the spmd example
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
spmd: spmd.o
	${CXX} -ospmd spmd.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f spmd
