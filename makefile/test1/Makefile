

src=$(wildcard *.c ./sub/*.c)
dir=$(notdir $(src))
obj=$(patsubst %.c,%.o,$(dir))
src_obj=$(patsubst %.c,%.o,$(src))
src_obj2=$(src:.c=.o)

all:
	@echo "src=$(src)"
	@echo "dir=$(dir)"
	@echo "obj=$(obj)"
	@echo "src_obj=$(src_obj)"
	@echo "src_obj2=$(src_obj2)"
