

all: symbol callsite disassembler breakpoint ftrace 

ftrace: source/ftrace.cc 
	g++ -o ftrace source/ftrace.cc symbol.o callsite.o disassembler.o breakpoint.o -std=gnu++11 -lcapstone

breakpoint: source/breakpoint.cc 
	g++ -o breakpoint.o -c source/breakpoint.cc -std=gnu++11 

callsite: source/callsite.cc 
	g++ -o callsite.o -c source/callsite.cc -std=gnu++11 

symbol: source/symbol.cc 
	g++ -o symbol.o -c source/symbol.cc -std=gnu++11 

disassembler: source/disassembler.cc 
	g++ -o disassembler.o -c source/disassembler.cc -std=gnu++11 -lcapstone

clean:
	rm -f ftrace disassembler.o callsite.o breakpoint.o symbol.o