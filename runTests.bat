@Echo off
REM this is handled by VSCode test task but is replicated here for those not using VSCode.
@Echo on
gcc -Isrc -Itests/lib src/file_util.c tests/lib/CuTest.c tests/file_util_test.c tests/main_test.c tests/lib/std_swapper.c -otests -ansi
tests