CPPFLAGS = -std=c++11 -Wall -g -c
SRCS = main.cpp firstFit.cpp firstFitDecreasing.cpp backtracking.cpp packet_comp.cpp bestFit.cpp bestFitDecreasing.cpp
OBJS = $(SRCS:.cpp=.o)

all: main benchmark

main: $(OBJS)
	g++ $^ -o bin_packing.exe

%.o: %.cpp
	g++ $(CPPFLAGS) $< -o $@

benchmark:
	g++ bench.cpp $(filter-out main.o, $(OBJS)) -std=c++11 -DBENCHMARK_STATIC_DEFINE -isystem benchmark/include \
	-Lbenchmark/build_mingw/src -lbenchmark -lshlwapi -lpthread -o bench.exe

clean:
	rm -f *.o *.exe

ben:
	.\bench.exe
run:
	.\bin_packing.exe