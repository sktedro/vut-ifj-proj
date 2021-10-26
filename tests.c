#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "scanner.c"

void debugPrint(char c, int realState, int expectedState) {
    printf("%c", c);
    printf(" : ");
    //printEnum(expectedState);
    printf(":");
    //printEnum(realState);
    printf("\n");
}

/*void printEnum(int state) {
    switch (state) {
        case 0:
            printf(" start ");
            break;
        case 1:
            printf(" dash ");
            break;
        case 2:
            printf( " comment ");
            break;
        case 3:
            printf(" unknownComment ");
            break;
        case 4:
            printf(" singleLineComment ");
            break;
        case 5:
            printf(" multiLineComment ");
            break;
        case 6:
            printf(" multiLineCommentPossibleEnd ");
            break;
        case 7:
            printf(" operator ");
            break;
        case 8:
            printf(" idOrKeyword ");
            break;
        case 9:
            printf(" integer ");
            break;
        case 10:
            printf(" number ");
            break;
        case 11:
            printf(" string ");
            break;
        case 12:
            printf(" stringEnd ");
            break;
        case 13:
            printf(" dot ");
            break;
        case 14:
            printf(" tilda ");
            break;
        case 15:
            printf(" slash ");
            break;
        case 16:
            printf(" leftParen ");
            break;
        case 17:
            printf(" rightParen ");
            break;
        case 18:
            printf(" possibleCompare ");
            break;
        default:
            printf(" ERROR ");
            break;
    }
}*/

int main() {

    //debugPrint('a',5,4);

    //testSingleLineComment();

    scanner();
    printf("\nDONE\n");
    return 0;
}