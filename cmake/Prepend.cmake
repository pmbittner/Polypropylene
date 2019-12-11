#from https://stackoverflow.com/questions/4346412/how-to-prepend-all-filenames-on-the-list-with-common-path
FUNCTION(PAXPREPEND var prefix)
    SET(listVar "")
    FOREACH(f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH(f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PAXPREPEND)
