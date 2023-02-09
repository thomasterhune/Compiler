@Echo off
REM this is handled by VSCode test task but is replicated here for those not using VSCode.
@Echo on
gcc -Isrc -Itests/lib src/file_util.c src/compfiles.c src/tokens.c src/scanner.c src/scanner_util.c tests/lib/CuTest.c tests/lib/std_swapper.c tests/file_util_test.c tests/compfiles_test.c tests/main_test.c tests/tokens_test.c tests/scanner_test.c tests/scanner_util_test.c -otests -ansi
tests