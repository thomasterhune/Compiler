@Echo off
REM this is handled by VSCode build task but is replicated here for those not using VSCode.
@Echo on
gcc src/main.c src/file_util.c -ansi -ofileopen
fileopen