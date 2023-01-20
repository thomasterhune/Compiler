@Echo off
REM this is handled by VSCode test task but is replicated here for those not using VSCode.
@Echo on
gcc -Isrc src/file_util.c tests/CuTest.c tests/file_util_test.c tests/main_test.c -otests
tests