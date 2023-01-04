lib32=/home/ricardo/temp/heliosworkspace/mereka/lib

main.cpp=main.cpp
main.pp=main.pp
main.s=main.s
main.o=main.o
main.map=main.map
EXE=main

as=/usr/bin/as
ld=/usr/bin/ld

crt1.o=${lib32}/crt1.o
crtbegin.o=${lib32}/crtbegin.o
crtend.o=${lib32}/crtend.o
crti.o=${lib32}/crti.o
crtn.o=${lib32}/crtn.o
ld-2.12.so=${lib32}/ld-2.5.so
libc_nonshared.a=${lib32}/libc_nonshared.a
libc.so.6=${lib32}/libc.so.6
libdl.so.2=${lib32}/libdl.so.2
libgcc_s.so.1=${lib32}/libgcc_s.so.1
libgmp.so.3=${lib32}/libgmp.so.3
libm.so.6=${lib32}/libm.so.6
libmpfr.so.1=${lib32}/libmpfr.so.1
libpthread.so.0=${lib32}/libpthread.so.0
librt.so.1=${lib32}/librt.so.1
libstdc++.so.6=${lib32}/libstdc++.so.6
libglib-2.0.so.0=${lib32}/libglib-2.0.so.0

cpp=${ld-2.12.so} --library-path ${lib32} ${lib32}/cpp
cc1plus=${ld-2.12.so} --library-path ${lib32} ${lib32}/cc1plus

all:
	@${cc1plus}\
		-g -m32 -std=c++98 -nostdinc -nostdinc++\
		-fdiagnostics-show-option\
		-Wall -Wno-write-strings\
		-Wno-pmf-conversions -Wno-strict-aliasing\
		-O0\
		-m32 ${main.cpp} -o ${main.s} -quiet     
	@${as} -g --32 ${main.s} -o ${main.o}
	@${ld} -g -m elf_i386 -nostdlib --dynamic-linker ${ld-2.12.so}\
		-rpath ${lib32} -rpath-link ${lib32} --export-dynamic\
		${crt1.o} ${crti.o} ${crtbegin.o}\
		${main.o}\
		${libc.so.6} ${libm.so.6} ${libc_nonshared.a} ${librt.so.1}\
		${libgcc_s.so.1} ${libstdc++.so.6} ${libpthread.so.0} ${libdl.so.2}\
		\
		${crtend.o} ${crtn.o}\
		-o ${EXE} -Map ${main.map}
	
clean:
	rm -rf ${main.s} ${main.o} ${main.map} ${main.pp} ${EXE}	

map-run:
	map
run:
	@main

run2:
	@${ld-2.12.so} --library-path ${lib32} ./main	

glib:
	@${cc1plus}\
	  -g -m32 -std=c++98 -nostdinc -nostdinc++\
		-fdiagnostics-show-option\
		-Wall -Wno-write-strings \ 
		-Wno-pmf-conversions\
		 -O0\
		-m32 ${main.cpp} -o ${main.s} -quiet     
	@${as} -g --32 ${main.s} -o ${main.o}
	@${ld} -g -m elf_i386 -nostdlib --dynamic-linker ${ld-2.12.so}\
		-rpath ${lib32} -rpath-link ${lib32} --export-dynamic\
		${crt1.o} ${crti.o} ${crtbegin.o}\
		${main.o}\
		${libc.so.6} ${libm.so.6} ${libc_nonshared.a} ${librt.so.1}\
		${libgcc_s.so.1} ${libstdc++.so.6} ${libpthread.so.0}\
		${libglib-2.0.so.0}\
		${crtend.o} ${crtn.o}\
		-o ${EXE} -Map ${main.map}
	
fedora:
	g++ -std=c++98 -m32 main.cpp -o main -lrt -fdiagnostics-show-option -Wno-write-strings -Wno-pmf-conversions -Wl,-Map main.map
	
terminal:
	g++ -m32 main.cpp -o main -Wno-pmf-conversions -Wno-write-strings -Wno-builtin-declaration-mismatch
