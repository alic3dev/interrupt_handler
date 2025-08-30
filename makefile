name=interrupt_handler

directory_include=include
directory_library=library
directory_objects=objects
directory_sources=sources

file_library=${directory_library}/${name}.o

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c, ${directory_objects}/%.o, ${files_sources}}

cc=gcc
c_flags=-O3 -I${directory_include}

ld=ld
ld_flags=

strip=strip
strip_flags=-x

${file_library}: ${files_objects}
	mkdir -p ${directory_library}
	${ld} ${ld_flags} -r $^ -o ${file_library}
	${strip} ${strip_flags} ${file_library}

${directory_objects}/%.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

clean:
	-rm ${file_library} ${directory_library}/*.o ${directory_objects}/*.o 2> /dev/null

