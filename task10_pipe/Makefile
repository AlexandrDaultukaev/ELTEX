.PHONY: all run_unnamed clear_unnamed

all: build_unnamed build_named

DIR_U=unnamed_pipe
DIR_N=named_pipe

build_unnamed: ${DIR_U}/unnamed_pipe.o
	gcc -o ${DIR_U}/unnamed_pipe ${DIR_U}/unnamed_pipe.o

${DIR_U}/unnamed_pipe.o: ${DIR_U}/unnamed_pipe.c
	gcc -c ${DIR_U}/unnamed_pipe.c -o ${DIR_U}/unnamed_pipe.o

run_unnamed:
	./${DIR_U}/unnamed_pipe

clear_unnamed:
	rm ${DIR_U}/*.o ${DIR_U}/unnamed_pipe


build_named: build_writer build_reader

build_writer: ${DIR_N}/writer_named_pipe.o
	gcc -o ${DIR_N}/writer_named_pipe ${DIR_N}/writer_named_pipe.o

${DIR_N}/writer_named_pipe.o: ${DIR_N}/writer_named_pipe.c
	gcc -c ${DIR_N}/writer_named_pipe.c -o ${DIR_N}/writer_named_pipe.o

build_reader: ${DIR_N}/reader_named_pipe.o
	gcc -o ${DIR_N}/reader_named_pipe ${DIR_N}/reader_named_pipe.o

${DIR_N}/reader_named_pipe.o: ${DIR_N}/reader_named_pipe.c
	gcc -c ${DIR_N}/reader_named_pipe.c -o ${DIR_N}/reader_named_pipe.o

run_named_writer:
	cd ${DIR_N} && ./writer_named_pipe

run_named_reader:
	cd ${DIR_N} && ./reader_named_pipe

clear_named:
	rm ${DIR_N}/*.o ${DIR_N}/writer_named_pipe ${DIR_N}/reader_named_pipe ${DIR_N}/myfifo