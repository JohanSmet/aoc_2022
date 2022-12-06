all: solution

solution: solution.o
	$(CXX) $(LDFLAGS) -o $@ $<

clean:
	$(RM) *.o solution

@phony: clean all
