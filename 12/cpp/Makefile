CXXFLAGS += -std=c++2a

all: part1 part2

part1: part1.o
	$(CXX) $(LDFLAGS) -o $@ $<

part2: part2.o
	$(CXX) $(LDFLAGS) -o $@ $<

clean:
	$(RM) *.o part1 part2 compile_commands.json

@phony: clean all
