/**
 * VUT FIT - IFJ 2021
 *
 * @file scanner.h
 *
 * @author Patrik Skaloš (xskalo01)
 * @author Jana Kováčiková (xkovac59)
 * @author Alexaner Okrucký (xokruc00)
 * @author Jiřina Frýbortová (xfrybo01)
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "dynamic_char_array.h"
#include "misc.h"
#include "token.h"

/**
 * @brief Main scanner function - returns the next token based on lexical
 * analysis of characters from the standard input
 *
 * @param token: address to memory where the next token should be written
 *
 * @return 0 if successful, errcode otherwise
 */
int scanner(Token **token) ForceRetUse;

/**
 * @brief Stash a token (to be returned on next scanner() call)
 *
 * @param token to be stashed
 * 
 * @return 0 if successful, errcode otherwise
 */
int stashToken(Token **token) ForceRetUse;

/**
 * @brief returns true if c is a number
 *
 * @param char
 *
 * @return true if char is a number from 0 to 9
 */
bool isNum(char c);

/**
 * @brief returns true if c is a letter
 *
 * @param char
 *
 * @return true if char is a letter (upper or lowercase) from a to z
 */
bool isLetter(char c);

/**
 * @brief returns true if c is an operator
 *
 * @param char
 *
 * @return true if char is an operator (list: . ( ) + - / * ~ < = > #)
 */
bool isOperator(char c);

/**
 * @param char
 *
 * @return true if char is a space, newline or tabulator
 */
bool isWhitespace(char c);

/**
 * @brief Returns a token (writes it to the provided pointer)
 *
 * @param token: pointer to address where the new token should be written
 * @param type: type of the token that is to be returned
 * @param buf: buffer from which the token data should be read
 *
 * @return true if char is a space, newline or tabulator
 */
int returnToken(Token **token, int type, DynamicCharArray *buf);

/**
 * @brief Pretty much just calling ungetc(), but if the character is a newline,
 * also decrement the global LOCCount
 *
 * @param c: character to be returned
 */
void returnCharacterToStdin(char c);

#endif
/* end of file scanner.h */
