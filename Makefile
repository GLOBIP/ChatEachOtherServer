
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
FLAGS = -g  -std=c++20 -lncurses
COMPILER = g++

%.o: %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@
all: ${OBJS}
	${COMPILER} ${OBJS} -o main ${FLAGS}


run: all
	./main

.PHONY : clean
clean :
	-rm -f *.o $(OBJS)
