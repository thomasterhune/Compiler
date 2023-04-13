@Echo off
REM this is handled by VSCode build task but is replicated here for those not using VSCode.
mkdir bin
@Echo on
"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" -g src/compfiles.c src/main.c src/file_util.c src/parse.c src/tokens.c src/scan.c src/dfa.c src/tompiler.c src/generator.c -ansi -obin/tompiler
cd bin
tompiler.exe
cd ..