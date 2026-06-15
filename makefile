name=interrupt_handler

ifndef target_device
target_device=mac
endif

ifeq (${target_device},mac)
target_os=macos
endif

ifeq (${target_device},iphone)
target_os=ios
endif

ifneq (${target_os},macos)
name:=${name}_${target_os}
endif

version_major=0
version_minor=0
version_patch=0
version_major_minor=${version_major}.${version_minor}
version=${version_major}.${version_minor}.${version_patch}

directory_objects_base=objects
directory_library_base=library

directory_library=${directory_library_base}/${target_os}/release
directory_objects=${directory_objects_base}/${target_os}/release

directory_clic3=../clic3
directory_clic3_include=${directory_clic3}/include
ifeq (${target_os},ios)
directory_clic3_library=${directory_clic3}/library/ios/release
else
directory_clic3_library=${directory_clic3}/library/macos/release
endif

ifeq (${debug}, 1)
name:=${name}_debug
directory_library:=${directory_library_base}/${target_os}/debug
directory_objects=${directory_objects_base}/${target_os}/debug
endif

directory_include=include
directory_install=/System/Volumes/Preboot/Cryptexes
directory_sources=sources

file_library_object=${directory_library}/${name}.o

name_library_dylib_major=${name}.${version_major}.dylib
file_library_dylib=${directory_library}/${name}.dylib
file_library_dylib_major=${directory_library}/${name_library_dylib_major}

file_install=${directory_install}/OS${name}.dylib
file_install_major=${directory_install}/OS${name_library_dylib_major}

name_library_dynamic_major=${name}.${version_major}.so
file_library_dynamic=${directory_library}/${name}.so
file_library_dynamic_major=${directory_library}/${name_library_dynamic_major}

file_library_static=${directory_library}/${name}.a

ifeq (${target_os},ios)
file_clic3_library_dylib=${directory_clic3_library}/clic3.${version_target_clic3}_ios.dylib
file_clic3_library_dynamic=${directory_clic3_library}/clic3_ios.so
else
file_clic3_library_dylib=${directory_clic3_library}/clic3.${version_target_clic3}.dylib
file_clic3_library_dynamic=${directory_clic3_library}/clic3.so
endif

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c, ${directory_objects}/%.o, ${files_sources}}

ifndef target_device_version
target_device_version=26.1
endif

ifeq (${target_os},macos)
target_platform=arm64-apple-macos${target_device_version}

directory_sdk=${shell xcrun --sdk macosx${target_device_version} --show-sdk-path}
endif

ifneq (${target_os},macos)
files_objects:=${patsubst ${directory_objects}/%.o,${directory_objects}/%_${target_os}.o,${files_objects}}

target_platform=arm64-apple-ios${target_iphoneos_version}

directory_sdk=${shell xcrun --sdk iphoneos${target_device_version} --show-sdk-path}
endif

cc=clang
c_flags_platform=-target ${target_platform} -isysroot ${directory_sdk}
c_flags=${c_flags_platform} -I${directory_include} -I${directory_clic3_include}

ifeq (${debug}, 1)
c_flags:=${c_flags} -O0 -g -v
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

install: ${file_library_dylib}
	mkdir -p "${directory_install}"
	dd if="${file_library_dylib_major}" of="${file_install_major}"
	ln -s "${file_install_major}" "${file_install}"

${file_library_dylib}: ${files_objects}
	mkdir -p ${directory_library}
	${cc} -dynamiclib ${c_flags_platform} -install_name ${name_library_dylib_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_objects} ${file_clic3_lbrary_dylib} -o ${file_library_dylib_major}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_dylib_major}
endif
	-rm ${file_library_dylib}
	ln -s ${name_library_dylib_major} ${file_library_dylib}

${file_library_dynamic}: ${files_objects}
	mkdir -p ${directory_library}
	${cc} -shared ${c_flags_platform} -install_name ${name_library_dynamic_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_objects} ${file_clic3_library_dynamic} -o ${file_library_dynamic_major}
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

${directory_objects}/%_${target_os}.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

clean: clean_library clean_objects

clean_library:
	if [[ -d ${directory_library_base} ]]; then rm -r ${directory_library_base}; fi

clean_objects:
	if [[ -d ${directory_objects_base} ]]; then rm -r ${directory_objects_base}; fi
