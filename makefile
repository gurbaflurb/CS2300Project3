generate:
	@echo "Compiling source.cpp to main.exe..."
	@g++ source.cpp -std=c++11 -o main
debug:
	@echo "Compiling source.cpp to main.exe in debug mode..."
	@g++ source.cpp -std=gnu++11 -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -ansi -o main