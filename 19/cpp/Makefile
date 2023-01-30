CXXFLAGS += -std=c++2a

all: solution

solution: solution.o
	$(CXX) $(LDFLAGS) -o $@ $<

clean:
	$(RM) *.o part1 part2 solution compile_commands.json

@phony: clean all
