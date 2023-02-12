#ifndef Scanner_util_h
#define Scanner_util_h

/*!
    Takes a malloced string called bufferToDouble. Creates a new malloced string that is twice as large as the parameter. Copies the parameter into the new malloced string. Frees the parameter. Returns the new malloced string.

    \pre bufferToDouble is a malloced string.
    \param bufferToDouble the malloced string to double.
    \post bufferToDouble is freed.
*/
char* resizeBuffer(char * bufferToResize, int newSize);

/*!
    Refreshes a buffer by freeing it, then allocating a new buffer of newSize.

    \param bufferToRefresh a malloced buffer to refresh.
    \param newSize the size of the new buffer that will be returned.
    \return a malloced string to a new buffer of new size. 

*/
char* refreshBuffer(char * bufferToRefresh, int newSize);

/*!
    Moves a pointer to a buffer index to the index of the next character in the buffer that isn't a ' ' or a '\t'. 

    \pre The buffer should end in a non-whitespace (such as \n or EOF) to prevent overflows. Index should point to a valid index.
    \post index will point to the next non-whitespace character after it started
    \param buffer char* The buffer to check.
    \param index A pointer to the buffer index. Checking will start from this index. This index will be set to the next non-whitespace character, which may be the samne value it started as.
*/
void skipWhitespace(char * buffer, int * index);


#endif