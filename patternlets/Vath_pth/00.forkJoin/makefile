
# This makefile builds the forkJoin example
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
forkJoin: forkJoin.o
	${CXX} -oforkJoin forkJoin.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f forkJoin
