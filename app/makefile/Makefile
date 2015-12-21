
#all: MAKE_TARGET
#	$(warning "run all")

#all: TEST_TARGET |DEP_TARGET
#	$(warning "run MAKE_TARGET")
	
#TEST_TARGET:
#	$(warning "run ___TEST_TARGET")
#DEP_TARGET:
#	$(warning "run DEP_TARGET")
#测试符号"|"的作用->当目标test存在时，即使fun.o被更改，也不会重新编译生成test目标(即只要test存在，即改了fun的代码，不会被编译到test中去)
test:test.o | fun.o
	gcc -o test test.o fun.o
test.o:
	gcc -c -o $(@) test.c
fun.o:
	gcc -c -o $(@) fun.c
clean:
	rm -f test *.o
