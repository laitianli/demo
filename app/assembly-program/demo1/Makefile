all: main

main: question_answer.o
	ld -o main question_answer.o

question_answer.o:
	as --32 -o question_answer.o question_answer.s

clean:
	rm -rf question_answer.o main
