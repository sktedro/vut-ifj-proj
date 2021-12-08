/*
 * Built in functions
 */

/*
 * Formatting using vim macros:
 * Configure the macro:
 *   q p $ 1 0 0 a [space] [esc] 7 8 | D a \ n \ [esc] j q
 * To run, paste the code, set cursor on the first code line and while X is the 
 *   amount of lines, type "X@p"
 */

#ifndef BUILT_IN_FUNCTIONS_C
#define BUILT_IN_FUNCTIONS_C

const char *reads = "                                                              \n\
";

const char *readi = "                                                              \n\
";

const char *readn = "                                                              \n\
";

const char *write = "                                                              \n\
";

const char *tointeger = "                                                          \n\
";

const char *substr = "                                                             \n\
LABEL _$SUBSTR_                                                              \n\
                                                                             \n\
    PUSHFRAME                                                                \n\
                                                                             \n\
    DEFVAR LF@$$STRRET                                                       \n\
    MOVE LF@$$STRRET string@                                                 \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR1                                                      \n\
    MOVE LF@$$STRPAR1 LF@$$STRPARAM1                                         \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR2                                                      \n\
    MOVE LF@$$STRPAR2 LF@$$STRPARAM2                                         \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR3                                                      \n\
    MOVE LF@$$STRPAR3 LF@$$STRPARAM3                                         \n\
                                                                             \n\
    JUMPIFEQ _STR$ERROR_ LF@$$STRPAR1 nil@nil                                \n\
    JUMPIFEQ _STR$ERROR_ LF@$$STRPAR2 nil@nil                                \n\
    JUMPIFEQ _STR$ERROR_ LF@$$STRPAR3 nil@nil                                \n\
                                                                             \n\
    DEFVAR LF@$$STRHELP                                                      \n\
    MOVE LF@$$STRHELP nil@nil                                                \n\
                                                                             \n\
    DEFVAR LF@$$STRLEN                                                       \n\
    STRLEN LF@$$STRLEN LF@$$STRPAR1                                          \n\
                                                                             \n\
    # if (param3 < param2)                                                   \n\
    LT LF@$$STRHELP LF@$$STRPAR3 LF@$$STRPAR2                                \n\
    JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true                              \n\
                                                                             \n\
    # if (param2 < 1)                                                        \n\
    LT LF@$$STRHELP LF@$$STRPAR2 int@1                                       \n\
    JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true                              \n\
                                                                             \n\
                                                                             \n\
    # if (param2 > len(param1))                                              \n\
    GT LF@$$STRHELP LF@$$STRPAR2 LF@$$STRLEN                                 \n\
    JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true                              \n\
                                                                             \n\
    # if (param3 < 1)                                                        \n\
    LT LF@$$STRHELP LF@$$STRPAR3 int@1                                       \n\
    JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true                              \n\
                                                                             \n\
    # if (param3 > len(param1))                                              \n\
    GT LF@$$STRHELP LF@$$STRPAR3 LF@$$STRLEN                                 \n\
    JUMPIFEQ _STR$EMPTY_ LF@$$STRHELP bool@true                              \n\
                                                                             \n\
    #---------------------------------                                       \n\
    DEFVAR LF@$$STRINDEX                                                     \n\
    MOVE LF@$$STRINDEX int@0                                                 \n\
                                                                             \n\
    DEFVAR LF@$$STRTMP                                                       \n\
    ADD LF@$$STRPAR3 LF@$$STRPAR3 int@1                                      \n\
    LABEL _STR$WHILE_                                                        \n\
        # if (param2 < param3)                                               \n\
        LT LF@$$STRHELP LF@$$STRPAR2 LF@$$STRPAR3                            \n\
        JUMPIFEQ _STR$RET_ LF@$$STRHELP bool@false                           \n\
                                                                             \n\
        GETCHAR LF@$$STRTMP LF@$$STRPAR1 LF@$$STRPAR2                        \n\
        CONCAT LF@$$STRRET LF@$$STRRET LF@$$STRTMP                           \n\
                                                                             \n\
        ADD LF@$$STRINDEX LF@$$STRINDEX int@1                                \n\
        ADD LF@$$STRPAR2 LF@$$STRPAR2 int@1                                  \n\
                                                                             \n\
    JUMP _STR$WHILE_                                                         \n\
                                                                             \n\
    JUMP _STR$RET_                                                           \n\
                                                                             \n\
LABEL _STR$EMPTY_                                                            \n\
    MOVE LF@$$STRRET string@                                                 \n\
    JUMP _STR$RET_                                                           \n\
                                                                             \n\
LABEL _STR$ERROR_                                                            \n\
    EXIT int@8                                                               \n\
                                                                             \n\
LABEL _STR$RET_                                                              \n\
    POPFRAME                                                                 \n\
    RETURN                                                                   \n\
                                                                             \n\
";

const char *ord = "                                                                \n\
LABEL ord0                                                                   \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
                                                                             \n\
# Check if the input string or the input integer equals nil                  \n\
  JUMPIFEQ *ord_err LF@$param_0 nil@nil                                      \n\
  JUMPIFEQ *ord_err LF@$param_1 nil@nil                                      \n\
                                                                             \n\
# Return nil if !(strlen(s) > i)                                             \n\
  DEFVAR LF@len                                                              \n\
  DEFVAR LF@strlenGTi                                                        \n\
  STRLEN LF@len LF@$param_0                                                  \n\
  GT LF@strlenGTi LF@len LF@$param_1                                         \n\
  JUMPIFEQ *retnil LF@strlenGTi bool@false                                   \n\
                                                                             \n\
# Return nil if i < 0                                                        \n\
  DEFVAR LF@iLT0                                                             \n\
  LT LF@iLT0 LF@$param_1 int@0                                               \n\
  JUMPIFEQ *retnil LF@iLT0 bool@true                                         \n\
                                                                             \n\
# Get the result and return                                                  \n\
  STRI2INT LF@result LF@$param_0 LF@$param_1                                 \n\
  JUMP *ordEnd                                                               \n\
                                                                             \n\
# Exit with err                                                              \n\
LABEL *ord_err                                                               \n\
  EXIT int@8                                                                 \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *retnil                                                                \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *ordEnd                                                               \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *ordEnd                                                                \n\
  POPFRAME                                                                   \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 TF@result                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

const char *chr = "                                                                \n\
LABEL _$CHR_                                                                 \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@$$CHRRET                                                         \n\
  MOVE LF@$$CHRRET string@                                                   \n\
                                                                             \n\
  DEFVAR LF@$$CHRPAR1                                                        \n\
  MOVE LF@$$CHRPAR1 LF@$$CHRPARAM1                                           \n\
                                                                             \n\
  DEFVAR LF@$$CHRHELP                                                        \n\
                                                                             \n\
  JUMPIFEQ _CHR$ERROR_ LF@$$CHRPAR1 nil@nil                                  \n\
                                                                             \n\
  # if (param1 < 0)                                                          \n\
  LT LF@$$CHRHELP LF@$$CHRPAR1 int@0                                         \n\
  JUMPIFEQ _CHR$NILL_ LF@$$CHRHELP bool@true                                 \n\
                                                                             \n\
  # if (param1 > 255)                                                        \n\
  GT LF@$$CHRHELP LF@$$CHRPAR1 int@255                                       \n\
  JUMPIFEQ _CHR$NILL_ LF@$$CHRHELP bool@true                                 \n\
                                                                             \n\
  INT2CHAR LF@$$CHRRET LF@$$CHRPAR1                                          \n\
                                                                             \n\
JUMP _CHR$RET_                                                               \n\
                                                                             \n\
                                                                             \n\
LABEL _CHR$NILL_                                                             \n\
MOVE LF@$$CHRRET nil@nil                                                     \n\
JUMP _CHR$RET_                                                               \n\
                                                                             \n\
LABEL _CHR$ERROR_                                                            \n\
  EXIT int@8                                                                 \n\
                                                                             \n\
LABEL _CHR$RET_                                                              \n\
                                                                             \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

#endif
/* End of file built_in_functions.c */
