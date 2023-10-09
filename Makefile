MAIN := main.cpp
LIBS := $(wildcard *.hpp)
OBJS := $(SRCS:.cpp=.o)
EXE = out

CC = g++

$(EXE):$(OBJS)
	$(CC) $(MAIN) $(OBJS) -o $(EXE)

clean:
	rm -rf $(OBJS) $(EXE)