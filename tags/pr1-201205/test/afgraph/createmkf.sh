#!/bin/sh
# generate a list obj files
echo -n 'OBJS		=	'  > /tmp/tcreatmk00000001
for cppfile in *.cpp
do
	echo -n ${cppfile%.cpp}.o ' '  >> /tmp/tcreatmk00000001
done
# generate a list of programs, one for each cpp file in the current director
echo >> /tmp/tcreatmk00000001
echo -n 'PROGRAMS		=	' >> /tmp/tcreatmk00000001
for cppfile in *.cpp
do
	echo  -n ${cppfile%.cpp} ' ' >> /tmp/tcreatmk00000001
done
echo >> /tmp/tcreatmk00000001
echo 'all	:	${PROGRAMS}' >> /tmp/tcreatmk00000001

# generate .cpp .o dependencies
for cppfile in *.cpp
do
	echo ${cppfile%.cpp} '\t:'  ${cppfile%.cpp}.o >> /tmp/tcreatmk00000001
done
echo	>> /tmp/tcreatmk00000001
cat /tmp/tcreatmk00000001 makefile.in >makefile.mk
rm -f makefile
# the following will generate header file dependencies
make -f makefile.mk makefile

#for on Windows only
echo '\n#clean up all executables produced\n' >>makefile
echo 'cleanexe\t:'  >>makefile
for cppfile in *.cpp
do
	echo '\trm -f ' ${cppfile%.cpp}.exe '\'  >>makefile
done
