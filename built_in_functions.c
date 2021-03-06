/**
 * VUT FIT - IFJ 2021
 *
 * @file built_in_functions.c
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
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

const char *reads = "                                                        \n\
LABEL reads0                                                                 \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
  READ LF@result string                                                      \n\
                                                                             \n\
  JUMPIFEQ *retnil LF@result string@EOF                                      \n\
                                                                             \n\
  JUMP *reads_end                                                            \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *retnil                                                                \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *reads_end                                                            \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *reads_end                                                             \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 LF@result                                                   \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

const char *readi = "                                                        \n\
LABEL readi0                                                                 \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
  READ LF@result int                                                         \n\
                                                                             \n\
  JUMPIFEQ *readi_retnil LF@result int@-1                                    \n\
                                                                             \n\
  JUMP *readi_end                                                            \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *readi_retnil                                                          \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *readi_end                                                            \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *readi_end                                                             \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 LF@result                                                   \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

const char *readn = "                                                        \n\
LABEL readn0                                                                 \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
  READ LF@result float                                                       \n\
                                                                             \n\
  JUMPIFEQ *readn_retnil LF@result float@-0x1p+0                             \n\
                                                                             \n\
  JUMP *readn_end                                                            \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *readn_retnil                                                          \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *readn_end                                                            \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *readn_end                                                             \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 LF@result                                                   \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

const char *tointeger = "                                                    \n\
LABEL tointeger0                                                             \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@type                                                             \n\
  TYPE LF@type LF@$param__0                                                  \n\
  JUMPIFNEQ *tointeger_retnil LF@type string@float                           \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
  FLOAT2INT LF@result LF@$param__0                                           \n\
                                                                             \n\
  JUMP *tointeger_end                                                        \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *tointeger_retnil                                                      \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *tointeger_end                                                        \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *tointeger_end                                                         \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 LF@result                                                   \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

// Not working
const char *substr = "                                                       \n\
LABEL substr0                                                                \n\
                                                                             \n\
    PUSHFRAME                                                                \n\
                                                                             \n\
    DEFVAR LF@!ret_0                                                         \n\
    MOVE LF@!ret_0 string@                                                   \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR1                                                      \n\
    MOVE LF@$$STRPAR1 LF@$param__0                                           \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR2                                                      \n\
    MOVE LF@$$STRPAR2 LF@$param__1                                           \n\
                                                                             \n\
    DEFVAR LF@$$STRPAR3                                                      \n\
    MOVE LF@$$STRPAR3 LF@$param__2                                           \n\
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
        CONCAT LF@!ret_0 LF@!ret_0 LF@$$STRTMP                               \n\
                                                                             \n\
        ADD LF@$$STRINDEX LF@$$STRINDEX int@1                                \n\
        ADD LF@$$STRPAR2 LF@$$STRPAR2 int@1                                  \n\
                                                                             \n\
    JUMP _STR$WHILE_                                                         \n\
                                                                             \n\
    JUMP _STR$RET_                                                           \n\
                                                                             \n\
LABEL _STR$EMPTY_                                                            \n\
    MOVE LF@!ret_0 string@                                                   \n\
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

const char *ord = "                                                          \n\
LABEL ord0                                                                   \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@result                                                           \n\
                                                                             \n\
# Check if the input string or the input integer equals nil                  \n\
  JUMPIFEQ *ord_err LF@$param__0 nil@nil                                     \n\
  JUMPIFEQ *ord_err LF@$param__1 nil@nil                                     \n\
                                                                             \n\
# Return nil if !(strlen(s) > i)                                             \n\
  DEFVAR LF@len                                                              \n\
  DEFVAR LF@strlenGTi                                                        \n\
  STRLEN LF@len LF@$param__0                                                 \n\
  GT LF@strlenGTi LF@len LF@$param__1                                        \n\
  JUMPIFEQ *ord_retnil LF@strlenGTi bool@false                               \n\
                                                                             \n\
# Return nil if i < 0                                                        \n\
  DEFVAR LF@iLT0                                                             \n\
  LT LF@iLT0 LF@$param__1 int@0                                              \n\
  JUMPIFEQ *ord_retnil LF@iLT0 bool@true                                     \n\
                                                                             \n\
# Get the result and return                                                  \n\
  STRI2INT LF@result LF@$param__0 LF@$param__1                               \n\
  JUMP *ord_end                                                              \n\
                                                                             \n\
# Exit with err                                                              \n\
LABEL *ord_err                                                               \n\
  EXIT int@8                                                                 \n\
                                                                             \n\
# Return nil                                                                 \n\
LABEL *ord_retnil                                                            \n\
  MOVE LF@result nil@nil                                                     \n\
  JUMP *ord_end                                                              \n\
                                                                             \n\
# Function end                                                               \n\
LABEL *ord_end                                                               \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 LF@result                                                   \n\
  POPFRAME                                                                   \n\
  RETURN                                                                     \n\
                                                                             \n\
";

const char *chr = "                                                          \n\
LABEL chr0                                                                   \n\
  PUSHFRAME                                                                  \n\
                                                                             \n\
  DEFVAR LF@!ret_0                                                           \n\
  MOVE LF@!ret_0 string@                                                     \n\
                                                                             \n\
  DEFVAR LF@$$CHRPAR1                                                        \n\
  MOVE LF@$$CHRPAR1 LF@$param__0                                             \n\
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
  INT2CHAR LF@!ret_0 LF@$$CHRPAR1                                            \n\
                                                                             \n\
JUMP _CHR$RET_                                                               \n\
                                                                             \n\
                                                                             \n\
LABEL _CHR$NILL_                                                             \n\
MOVE LF@!ret_0 nil@nil                                                       \n\
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
/* end of file built_in_functions.c */
