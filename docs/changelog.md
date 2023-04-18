4/18/2023: Karl
- Runs gcc after compiling after prompt
- versioned up
- grammar issue; emailed pzydrowski, changed grammar effectively in our program

4/6/2023: All group members
- Added generator.h and generator.c
- SymbolTable init, de-init, buffer creation and expansion functions
- SymbolTable lookup, enter, checkID, generate
- Tokens; C_TranslateMap, Token_CTranslate(int token)
- generator tests to validate symbol table funcs

3/20/2023: Karl, Thomas
- Wrote the final printing stuff for compilation result

3/20/2023: Karl
- ParseError region
- Appropriate handling of parse and lex errors
- Pretty printing of errors
- Limited parse error recovery
- Full lex error recovery 
- Error summarys

3/19/2023: Karl, Thomas, Anthony
- Debugged lexical error printing

3/19/2023: Karl, Anthony
- Debugged parser
- Debugged SkipWhitespaceforNextToken in scanner
- Printing statements for parser
- Init and lifecycle stuff for parser in tompiler

3/19/2023: Karl
- Wrote parse functions

3/18/2023: Karl, Thomas
- First few parse statements, including Parse_Program, Parse_SystemGoal, StatementList, Statement
- Ensured some printing to listing file and output file were occurring
- Fixed comment issue with skipping whitespace


3/11/2023: Karl
- Added rubric, examples to /doc
- Renamed terminal.h to console.h because terminal has a different meaning in the context of parsing.
- Added CONSOLE_COLOR and CONSOLE_COLOR_DEFAULT macros in console.h
- Added Scanner_LoadFiles(input,output,listing,temp) as part of scanner lifecycle methods. *note* professor may want us to pass in the input file at each call... I prefer to do this the object oriented way but I'll want to check to make sure he won't dock us for that.
- Added Scanner_CopyBuffer(char * destination) which copies the contents of scanner's buffer to another chararray and adds a null terminator.
- Added Scanner_SkipAllWhitespaceForNextToken() which also skips newlines, which is used for Scanner_NextToken checks (because the next token may be on a new line)
- Added Scanner_NextToken() which gets the next token in the input file, then returns the file pointer to its original position (before all whitespace)
- Removed leftover dfa test that was confirming minusop read as minusop when adjacent to intliteral; we changed this just before submission to read the way he wanted (as a negative intliteral) but I never removed the test.
- Added scanner_test.c which will be used to validate Scanner_NextToken and Scanner_Match. Utility functions for creating temp input files and initing scanner. Wrote tests to validate LookAhead and Match.

2/16/2023: Karl and Thomas
- Changed DFA to result in error on strings like '99a'; identifiers must start with characters, can't start with numbers.
- Changed DFA to not automatically process negative numbers. Rationale is that we want to allow cases like 100-100 to be read as INTLITERAL MINUSOP INTLITERAL not INTLITERAL INTLITERAL. Therefore, negatizing the intliterals is a context-sensitive activity that will occur at parse time instead of scan time. 
- Reverted the intliteral and identifier changes

2/15/2023: Karl and Thomas
- Better printing for compfiles opening and closing and better print formatting.
- Negative intliterals on dfa. 
- Tompiler.h
- Brought back the expanding buffer! Using it for actual token strings now instead of an entire line.
- Brought in terminal.h for virtual terminal color sequences and ported it to C.
- Pretty printing for Hello and Goodbye. 

2/15/2023: Karl
- Finished debugging full DFA. 
- Rewrote scanner in scan.h. No more using memory allocation and TokenCatch structures. It reads from file and writes directly to the listing and output files now. 
- Deleted scanner.c, scanner.h, scanner_util.c, scanner_util.h and associated test files.
- Deleted the recognize keyword token function and the associated dfa, since the new dfa covers everything.
- If we need those files and features back, we can revert to an earlier commit. 

2/14/2023: Karl
- Created a FULL dfa planned to replace all current logic.
- Moved all test header files to one header file "test.h".

2/13/2023: Karl and thomas
- fixed listing file not loading
- fixed extractInt AND extractInt tests (they were using extractWord)!
- running error count and print errors
- fixed detect SCANEOF
- fixed null terminates at end of buffer for no overflow print
- fixed last line being ignored
- several types of token catch initializers
- token catch allocates memory; can use the tokens later in the parser
- scanner printLine fixes
- print error count
- Token_GetOpRaw
- formatting line printing

2/13/2023: All Group Members
- Extract op
- Fleshed out the switch statement for take Action
- printLine always happens at the end of populateBuffer now
- extractOperator and Scanner_ExtractOperator.... extractOperator is in Scanner not scanner_util because it is dependent 
- Token_CatchOp Token_CatchError 

2/12/2023: Karl
- Added recognizers for trueop, nullop, falseop to the state transition table, which I had missed before.
- Skipwhitespace now returns the number of characters missed. This can be useful if we extract a number and it isn't followed by a whitespace (skipwhitespace will produce 0.) This may be a cause for an error print. (Worth asking)
- Fixed extractWord errors and added Scanner_ExtractWord
- Added extractInteger and Scanner_ExtractInteger 
- Added a boundries member to Tscanner. This is a list of all boundry characters that delimit words, identifiers, and number and it includes all operators plus whitespace and EOF. See Scanner_Init() for how it's constructed.
- Token Recognize now returns ERROR if there is a non number, non alphanumeric within the tested string. It also now allows for identifiers to have numbers.
- Created Token wrapping struct called TokenCatch that encapsulates info about the token such as the recognized raw string, line number, and so forth. 
- Scanner now takes files on Scanner_Scan(files...) not on Scanner_Init()
- Init and DeInit functions in main
- Moved switch statement/dispatcher into a function Scanner_TakeAction(lookaheadResult)
- Made basic Scanner_Scan(); currently will print the listing file numbers and lines only


2/11/2023: Karl
- Added skipWhitespace general function in scanner_util and added tests for it
- Added charIn function in scanner_util which is used by extractWord function in scanner_util.

2/10/2023: Karl
- Added Scanner_populateBuffer() and tested it.
- Created Scanner_LookAhead() and put a switch statement in Scanner_Scan()


2/8/2023: Karl, Thomas, Anthony
- created scanner.h and scanner.c
- add struct to hold scanner info
- scanner lifecycle functions
- scanner buffer functions
- scanner_util , created buffer resize and refresh functions
- created tests for scanner_util

2/7/2023: Karl
- abstracted command line argument parse and calls to a new function, CompFiles_FileOpenFromCLIArgs, which also generates the Temp file.
- created Tokens_GetName, the tokens Map, the tokens enum.
- created a state transition table as a 3d array for a keyword recognizer Token_RecognizeKeyword. Used excel to design the table; .xlsx is in the /docs folder.

 -------------------------------------------------------------------------

2/1/2023 : All Group Members
 - used fileapi.h to create a getAbsolutePath function
 - created checkIfSamePaths function to compare file name actual paths
 - reworked the validate files functions to check for output/input name collisions
 - adjusted some printing
 - fixed tempfile bug

1/28/2023: Karl
 - used doxygen to generate documentation

1/27/2023: Thomas, Karl
 - wrote copy inputs to outputs function

1/26/2023 : All group members
 - refactored file_util into two files: compfiles and file_util
 - worked on logic for validating an output file name
 - auto-generate temp file
 - validate listing file in a similar way to output file
 - combined validation functions into one validate func; just pass it the command line arguments

1/25/2023 : Thomas
 - promptOutputFile()
 - Modified getString() to use realloc

1/24/2023 : All group members
 - worked on main logic
 - changed CompFiles struct to be a state machine
 - created promptFilename

1/23/2023 : Thomas and klm127
 - changed Author comment to include e-mail and class name.
 - removed old addExtension function, old promptFilename function, and closeFile function.
 - added promptFilename and getString function(not yet covered by unit tests)
 - removed all of the stdin swapping to a [separate repo](https://github.com/klm127/std-swapper/), and tested it, due to nagging bugs. 
    - NOTE: It turned out that the bug was that [dup2](https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-dup2-duplicate-open-file-descriptor-another) closes a file and `fclose` was being called afterwards.
- moved test dependencies to a sub folder `lib` and updated compilation commands to use this on the include path

1/22/2023 : thomas and klm127
 - added removeExtension function and tests
 - confirmed getchar will read an 'enter'. 
 - thomas fixed prompting function to accept alternate inputs
 - added backupFile function and tests
 - Included tests for filepaths with directories
 - redid filenameHasExtension. It now allows for filenames like ".bob" and doesnt allow filenames that end in slashes. It does allow folders to have '.'s in them.


1/21/2023 : klm127
- added #pragma region directives to header files. This is basically just markup for VSCode. Each of these regions can now be folded in Visual Studio or VSCode. This does not affect -ansi compilation on MinGW-W64 gcc; as far as I can tell. The purpose is to make the code much easier to navigate without relying on tab-based folding. [See Also: stackoverflow answer](https://stackoverflow.com/questions/63512637/what-is-pragma-region-in-c-and-vscode)
- Cleaned up comments, tab-based folding, etc.
- Fixed up the `addExtension` to use malloc to create a longer, concatenated string out of its inputs. Added unit tests for `addExtension`. 
- Refactored std swapping test utility functions. The best way to test a prompter is now to use is to call `setSTDin3`, get the value, then dont forget to call `restoreSTD3()` _before_ making a test-based assertion. 


1/20/2023 : All group members in collaboration
- created `promptUserOverwriteSelection`.
- created tests for `promptUserOverwriteSelection`. This was quite an involved task because we had to figure out how to temporarily replace stdin and stdout with alternative files so that we could test functionalities like `scanf`. Ultimately we were able to figure it out. 


1/19/2023 : klm127

- changed directory structure, added docs, src, and tests
- created changelog, included CuTest's readme in the docs
- updated tasks.json in .vscode to configure code generation
- output file is now `fileopen.exe` due to interpretation of video instructions
- added .gitignore so we can exclude executables from github
- Added the testing suite CuTest. More info [here](https://cutest.sourceforge.net/)
- Added the functions `fileExists` and `filenameHasExtension`
- Added unit tests for `fileExists` and `filenameHasExtension`


