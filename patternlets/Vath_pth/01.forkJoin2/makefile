
# This makefile builds the forkJoin2 example
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
forkJoin2: forkJoin2.o
	${CXX} -oforkJoin2 forkJoin2.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f forkJoin2
