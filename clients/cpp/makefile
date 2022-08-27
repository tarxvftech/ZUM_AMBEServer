objects=ambepassthrough

.PHONY: default
default: ${objects}

.PHONY: clean
clean: 
	-rm ${objects}

%: %.cpp
	g++ -g -o $@ $^
