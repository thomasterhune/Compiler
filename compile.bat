@Echo off
REM this is handled by VSCode build task but is replicated here for those not using VSCode.
mkdir bin
@Echo on
gcc src/main.c src/file_util.c src/compfiles.c -ansi -obin/fileopen
cd bin
fileopen.exe
cd ..