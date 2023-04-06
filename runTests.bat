@Echo off
REM this is handled by VSCode test task but is replicated here for those not using VSCode.
@Echo on
"C:\Program Files\CodeBlocks\MinGW\bin\gcc" -g -Isrc -Itests/lib src/parse.c src/file_util.c src/compfiles.c src/tokens.c src/generator.c src/dfa.c src/scan.c tests/lib/CuTest.c tests/lib/std_swapper.c tests/file_util_test.c tests/compfiles_test.c tests/dfa_test.c tests/main_test.c tests/tokens_test.c tests/scanner_test.c tests/generator_test.c -otests -ansi
tests