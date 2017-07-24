# This makefile builds the parallelLoopChunksOf1 example
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
parallelLoopChunksOf1: parallelLoopChunksOf1.o
	${CXX} -oparallelLoopChunksOf1 parallelLoopChunksOf1.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f parallelLoopChunksOf1
