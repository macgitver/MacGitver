This Shell-Macro-Parser supports the following subset of the bash parameter expansion. In all examples, _parameter_ is subject to the replacement logic itself.

-   `${parameter}` or `$parameter`  (__Value get__)
    return value of _parameter_.

-   `${parameter:-word}` (__Use default value__)
    if _parameter_ is empty, call replacement logic on _word_ and return the result. Otherwise return _parameter_.

-   `${parameter:=word}` (__Use and assign default value__)
    if _parameter_ is empty, call replacement logic on _word_ and store the result in _parameter_. Then return _parameter_.

-   `${parameter:+word}` (__Use alternate value__)
    if _parameter_ is empty or unset, return empty. Otherwise, apply replacement logic to _word_ and return its result.

-   `${parameter:offset:length}` or `${parameter:offset}` (__Substring__)
    Return a substring of _parameter_ starting at _offset_. If _length_ is given, limit to that amount of characters.

-   `${#parameter}` (__Length of__)
    Return the number of characters in _parameter_ as a string.

-   `${parameter/pattern/string}` (__Replacement__)
    Replace all occurrences of _pattern_ in _parameter_ with _string_. Return the result. _string_ is subject to the replacement logic. 

-   `${parameter##word}` and `${parameter#word}` (__Prefix removal__)
    _word_ is subject to the replacement logic. If the beginning of _parameter_ matches _word_, the shortest (`#`) or longest (`##`) match is removed from _parameter_ and the result returned.

-   `${parameter%%word}` and `${parameter%word}` (__Suffix removal__)
    _word_ is subject to the replacement logic. If the tail of _parameter_ matches _word_, the shortest (`%`) or longest (`%%`) match is removed from _parameter_ and the result returned.

Any matching here refers to the (actually simplified) path name expansion method of bash:
- ? means any one character
- * means any number of characters
- [a-cg] means a, b, c or g
- [^abc] means any character but a, b or c
