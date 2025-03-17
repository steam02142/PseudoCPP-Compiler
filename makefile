flags = -Wall -Wextra -pedantic
cc =g++
build = build
source = src
include = include

all: pseudoCPPx

pseudoCPPx: ${build}/pseudoCPP.o ${build}/tokenizing.o ${build}/parsing.o
	${cc} ${flags} $< ${build}/tokenizing.o ${build}/parsing.o -o $@

${build}/pseudoCPP.o: ${source}/pseudoCPP.cpp ${include}/tokenizing.h ${include}/parsing.h
	${cc} ${flags} -c $< -o $@

${build}/tokenizing.o: ${source}/tokenizing.cpp ${include}/tokenizing.h
	${cc} ${flags} -c $< -o $@

${build}/parsing.o: ${source}/parsing.cpp ${include}/parsing.h ${include}/tokenizing.h
	${cc} ${flags} -c $< -o $@

clean:
	rm -f ${build}/pseudoCPP.o ${build}/tokenizing.o ${build}/parsing.o pseudoCPPx