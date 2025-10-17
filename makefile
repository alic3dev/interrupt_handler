name=interrupt_handler

version_major=0
version_minor=0
version_patch=0
version_major_minor=${version_major}.${version_minor}
version=${version_major}.${version_minor}.${version_patch}

directory_objects_base=objects
directory_library=library
directory_library_debug=${directory_library}_debug

directory_objects=${directory_objects_base}/release

ifeq (${debug}, 1)
	name:=${name}_debug
	directory_objects=${directory_objects_base}/debug
	directory_library:=${directory_library_debug}
endif

directory_include=include
directory_sources=sources

file_library_object=${directory_library}/${name}.o

name_library_dylib_major=${name}.${version_major}.dylib
file_library_dylib=${directory_library}/${name}.dylib
file_library_dylib_major=${directory_library}/${name_library_dylib_major}

name_library_dynamic_major=${name}.${version_major}.so
file_library_dynamic=${directory_library}/${name}.so
file_library_dynamic_major=${directory_library}/${name_library_dynamic_major}

file_library_static=${directory_library}/${name}.a

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c, ${directory_objects}/%.o, ${files_sources}}

cc=gcc
c_flags=-I${directory_include}

ifeq (${debug}, 1)
	c_flags:=${c_flags} -O0 -g -v -da -Q
else
	c_flags:=${c_flags} -O3
endif

ar=ar
ar_flags=cqS

ld=ld
ld_flags=

strip=strip
strip_flags=-x

${name}: ${file_library_dylib} ${file_library_dynamic} ${file_library_object} ${file_library_static}

${name}_objects: ${files_objects}

${name}_dylib: ${file_library_dylib}
${name}_dynamic: ${file_library_dynamic}
${name}_object: ${file_library_object}
${name}_static: ${file_library_static}

${file_library_dylib}: ${files_objects}
	mkdir -p ${directory_library}
	${cc} -dynamiclib -install_name ${name_library_dylib_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_objects} -o ${file_library_dylib_major}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_dylib_major}
endif
	-rm ${file_library_dylib}
	ln -s ${name_library_dylib_major} ${file_library_dylib}

${file_library_dynamic}: ${files_objects}
	mkdir -p ${directory_library}
	${cc} -shared -install_name ${name_library_dynamic_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_objects} -o ${file_library_dynamic_major}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_dynamic_major}
endif
	-rm ${file_library_dynamic}
	ln -s ${name_library_dynamic_major} ${file_library_dynamic}

${file_library_object}: ${files_objects}
	mkdir -p ${directory_library}
	${ld} ${ld_flags} -r ${files_objects} -o ${file_library_object}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_object}
endif

${file_library_static}: ${files_objects}
	mkdir -p ${directory_library}
	${ar} ${ar_flags} ${file_library_static} ${files_objects}

${directory_objects}/%.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

clean: clean_library clean_library_debug clean_objects

clean_library:
	-rm -r ${directory_library} 2> /dev/null

clean_library_debug:
	-rm -r ${directory_library_debug} 2> /dev/null

clean_objects:
	-rm -r ${directory_objects_base} 2> /dev/null
