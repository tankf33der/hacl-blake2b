all:
	clang -O3 -I. -I kremlib/dist/minimal *.c && ./a.out
clean:
	rm -rf *.out *.o
