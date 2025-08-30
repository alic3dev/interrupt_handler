project_name=interrupt_handler

include_directory=include
library_directory=library
objects_directory=objects
sources_directory=sources

out_file=$(library_directory)/$(project_name).o

source_files=$(wildcard $(sources_directory)/*.c)
object_files=$(patsubst $(sources_directory)/%.c, $(objects_directory)/%.o, $(source_files))

cc=gcc
c_flags=-O3 -I$(include_directory)

ld=ld
ld_flags=

strip=strip
strip_flags=-x

$(out_file): $(object_files)
	mkdir -p $(library_directory)
	$(ld) $(ld_flags) -r $^ -o $(out_file)
	${strip} ${strip_flags} ${out_file}


$(objects_directory)/%.o: $(sources_directory)/%.c
	mkdir -p $(objects_directory)
	$(cc) $(c_flags) -c $< -o $@

clean:
	-rm $(out_file) $(library_directory)/*.o $(objects_directory)/*.o 2> /dev/null

