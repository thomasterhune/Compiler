@Echo off
REM this is handled by VSCode build task but is replicated here for those not using VSCode.
mkdir bin
@Echo on
gcc src/main.c src/file_util.c src/compfiles.c src/tokens.c src/scanner.c src/scanner_util.c -ansi -obin/tompiler 
cd bin
tompiler.exe
cd ..