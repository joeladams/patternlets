# This makefile builds the raceCondition example =================

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
raceCondition: raceCondition.o
	${CXX} -oraceCondition raceCondition.o ${PATHV} -lvath ${LBASE}   
	rm *.o
	
clean:
	rm -f raceCondition
