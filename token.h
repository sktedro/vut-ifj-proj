#include "misc.h"

// Enumeration of the token types
enum TokenTypeEnum{
    t_idOrKeyword, // one token for both identificator and a keyword??
    t_int,
    t_num,
    t_sciNum,
    t_str,
    t_rightParen,
    t_leftParen,
    t_arithmOp,
    t_strOp,
    t_relOp,
    t_assignment,
};

// TODO
// Do we need more attributes in one token? If not, this structure is redundant
// and in Token struct, there should be just char data instead of
// TokenAttrib

// Structure defining a token attribute
typedef struct TokenAttrib TokenAttrib;
struct TokenAttrib{
    char *data;
    TokenAttrib *nextAttrib;
};

// Structure defining a token
typedef struct{
    int type;
    TokenAttrib *attrib;
} Token;


/**
 * @brief allocates memory for a new token and assigns it a type
 *
 * @param type of the new token
 *
 * @return token (pointer)
 */
Token *tokenInit(int type){
    Token *token = malloc(sizeof(Token));
    if(!token){
        return NULL;
    }
    token->type = type;
    token->attrib = NULL;
    return token;
}


/**
 * @brief add a new attribute to the token (allocate and write the data)
 *
 * @param token: token to which the new attribute should be added
 * @param data: data which are to be written to the token
 *
 * @return 0 if successful
 */
int tokenAddAttrib(Token *token, char *data){
    if(!token){
        return 1;
    }

    // Allocate an attribute
    TokenAttrib *newAttrib = malloc(sizeof(TokenAttrib));
    if(!newAttrib){
        return 1;
    }

    //TODO
    // Allocate space for data
    newAttrib->data = malloc(strlen(data) + 1);
    if(!newAttrib->data){
        return 1;
    }
    newAttrib->nextAttrib = NULL;

    // .. and write the data to the allocated space
    memcpy(newAttrib->data, data, strlen(data) + 1);

    token->attrib = newAttrib;

/*   // TODO
 *   // How to write to the nextAttrib??
 *   // Might not be needed though, so it's ok for now
 *
 *   // Attrib to which we will be writing the data
 *   TokenAttrib *attrib = token->attrib;
 *
 *   // If there is already at least one attrib, add this one at the end
 *   if(attrib){
 *     while(attrib->nextAttrib != NULL){
 *       attrib = attrib->nextAttrib;
 *     }
 *     // attrib->nextAttrib = malloc(sizeof(TokenAttrib));
 *     attrib->nextAttrib = newAttrib;
 *     if(!attrib){
 *       return false;
 *     }
 *   }
 */

    return 0;
}

/*
 * @brief Recursively free all memory allocated for attributes
 *
 * @param attrib: first attribute. All 'nextAttributes' will be freed too
 */
void tokenAttribDestroy(TokenAttrib *attrib){
    if(attrib->nextAttrib){
        tokenAttribDestroy(attrib->nextAttrib);
    }
    free(attrib->data);
    free(attrib);
}

/**
 * @brief Free all memory allocated for attributes and the token
 *
 * @param token to destroy
 */
void tokenDestroy(Token *token){
    if(token){
        if(token->attrib){
            tokenAttribDestroy(token->attrib);
        }
        free(token);
    }
}
