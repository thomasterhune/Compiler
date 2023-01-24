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

TODO: filenameHasExtension does allow for double periods! Need to fix that. e.g "abc..in" should not work.

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


