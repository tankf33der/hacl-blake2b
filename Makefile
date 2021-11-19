all:
	clang -I. -I kremlib/dist/minimal *.c && ./a.out
