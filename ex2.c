//312531973 elad baal-tzdaka
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * ************************************************
 *  define a key numbers to avoid "magic number"  *
 * ************************************************
 **/
#define SIZE 1
#define NUMBER_OF_ELEMENTS 2
#define MAX_LENGTH_STRING 100
#define NEW_LINE_IN_UNIX "\n"
#define NEW_LINE_IN_MAC "\r"
/**
 * **************************************************
 * declaration of all the function in the program ***
 * **************************************************
 * */

void optionNumber1(char *source_file, char *destination_file);

void optionNumber2(char *source_file, char *destination_file, char *sourceOS, char *destinationOS);

void fromUnixOrMacToWin(char *replace, FILE *src, FILE *dst);

void funcFromToMacOrUnix(char *replace, char *target, FILE *src, FILE *dst);

void fromWinToUnixOrMac(char *replace, FILE *src, FILE *dst);

void optionNumber3(char *source_file, char *destination_file, char *sourceOS, char *destinationOS, char *byteOrder);

void swap(char *file);

void swapChar(char c[MAX_LENGTH_STRING]);

///end of function declaration

/**
 * **************************
 * the main of the program **
 * **************************
 **/
int main(int argc, char *argv[]) {
    int caseNumber = argc - 1;
    switch (caseNumber) {
        case 2:
            optionNumber1(argv[1], argv[2]);
            break;
        case 4:
            optionNumber2(argv[1], argv[2], argv[3], argv[4]);
            break;
        case 5:
            optionNumber3(argv[1], argv[2], argv[3], argv[4], argv[5]);
            break;
        default:
            exit(1);
    }
    return 0;
}

/**
 * ****************************************************************************************
 * This function modifies the endianness of the file if the user write the key word swap. *
 * if the user write keep so we proceed like option number 2                              *
 * ****************************************************************************************
 **/
void optionNumber3(char *source_file, char *destination_file, char *sourceOS, char *destinationOS, char *byteOrder) {
    if (strstr(byteOrder, "keep") != NULL) {
        optionNumber2(source_file, destination_file, sourceOS, destinationOS);
        return;
    }
    if (strstr(byteOrder, "swap") != NULL) {
        optionNumber2(source_file, destination_file, sourceOS, destinationOS);
        swap(destination_file);
        return;
    }
    exit(1);

}

/**
  * ****************************************************
  * This function modifies the endianness of the file **
  * ****************************************************
 **/
void swap(char *file) {
    FILE *reader = NULL;
    reader = fopen(file, "rb");
    if (reader == NULL) {
        exit(1);
    }
    FILE *dst = NULL;
    dst = fopen(file, "rb+");
    if (dst == NULL) {
        fclose(reader);
        exit(1);
    }
    char c[MAX_LENGTH_STRING] = "";
    while (fread(c, SIZE, NUMBER_OF_ELEMENTS, reader)) {
        swapChar(c);
        fwrite(c, SIZE, NUMBER_OF_ELEMENTS, dst);
    }
    fclose(reader);
    fclose(dst);
}

/**
  * ************************************************************
  * this function only swap between the two call of the array **
  * ************************************************************
 **/
void swapChar(char c[MAX_LENGTH_STRING]) {
    char temp;
    temp = c[0];
    c[0] = c[1];
    c[1] = temp;
}

/**
 * *************************************************************
 * In this function create a new file where the character      *
 * symbolizes a new line is encoded according                  *
 * to what the user requests it can be Linux, Mac or Windows   *
 * *************************************************************
 **/
void optionNumber2(char *source_file, char *destination_file, char *sourceOS, char *destinationOS) {
    if (strstr(sourceOS, destinationOS) != NULL) {
        optionNumber1(source_file, destination_file);
        return;
    }
    FILE *src = NULL;
    FILE *dst = NULL;
    //check if file opening was success
    src = fopen(source_file, "rb");
    if (src == NULL) {
        exit(1);
    }
    dst = fopen(destination_file, "wb");
    if (dst == NULL) {
        fclose(src);
        exit(1);
    }
    //handling BOM
    char bom[MAX_LENGTH_STRING] = "";
    fread(bom, SIZE, NUMBER_OF_ELEMENTS, src);
    fwrite(bom, SIZE, NUMBER_OF_ELEMENTS, dst);
    if (strstr(sourceOS, "unix") != NULL) {
        if (strstr(destinationOS, "win") != NULL) {
            fromUnixOrMacToWin(NEW_LINE_IN_UNIX, src, dst);
        } else {
            funcFromToMacOrUnix(NEW_LINE_IN_UNIX, NEW_LINE_IN_MAC, src, dst);
        }
    }
    if (strstr(sourceOS, "mac") != NULL) {
        if (strstr(destinationOS, "win") != NULL) {
            fromUnixOrMacToWin(NEW_LINE_IN_MAC, src, dst);
        } else {
            funcFromToMacOrUnix(NEW_LINE_IN_MAC, NEW_LINE_IN_UNIX, src, dst);
        }
    }
    if (strstr(sourceOS, "win") != NULL) {
        if (strstr(destinationOS, "unix") != NULL) {
            fromWinToUnixOrMac(NEW_LINE_IN_UNIX, src, dst);
        } else {
            fromWinToUnixOrMac(NEW_LINE_IN_MAC, src, dst);
        }
    }
    fclose(src);
    fclose(dst);
}

/**
  * ***********************************************************
  * This function takes                                       *
  * care of the transition from a Linux file to a mac file,   *
  * and also in the other case of a Mac to windows file       *
  * \r-->\r\n or \n-->\r\n.                                   *
  * ***********************************************************
  **/
void funcFromToMacOrUnix(char *replace, char *target, FILE *src, FILE *dst) {
    char c[MAX_LENGTH_STRING] = "";
    while (fread(c, SIZE, NUMBER_OF_ELEMENTS, src)) {
        if (strcmp(c, replace) == 0) {
            fwrite(target, SIZE, NUMBER_OF_ELEMENTS, dst);
        } else {
            fwrite(c, SIZE, NUMBER_OF_ELEMENTS, dst);
        }
    }
}

/**
 * *********************************************************
 * This function takes                                     *
 * care of the transition from a Linux file to a mac file, *
 * and also in the opposite case of a Mac to Unix file     *
 * \n->\r or \r-->\n.                                      *
 * *********************************************************
 **/
void fromUnixOrMacToWin(char *replace, FILE *src, FILE *dst) {
    char c[MAX_LENGTH_STRING] = "";
    while (fread(c, SIZE, NUMBER_OF_ELEMENTS, src)) {
        if (strcmp(c, replace) == 0) {
            fwrite(NEW_LINE_IN_MAC, SIZE, NUMBER_OF_ELEMENTS, dst);
            fwrite(NEW_LINE_IN_UNIX, SIZE, NUMBER_OF_ELEMENTS, dst);
        } else {
            fwrite(c, SIZE, NUMBER_OF_ELEMENTS, dst);
        }
    }
}

/**
 *  **********************************************************
 * This function takes                                       *
 * care of the transition from a windows file to a Mac file, *
 * and also in the other case of a windows to Unix           *
 * \r\n-->\r or \r\n-->\n.                                   *
 *  **********************************************************
 * */
void fromWinToUnixOrMac(char *replace, FILE *src, FILE *dst) {
    char c[MAX_LENGTH_STRING] = "";
    while (fread(c, SIZE, NUMBER_OF_ELEMENTS, src)) {
        if (strcmp(c, "\r") == 0) {
            char indicator[MAX_LENGTH_STRING] = "";
            if (fread(indicator, SIZE, NUMBER_OF_ELEMENTS, src)) {
                if (strcmp(indicator, "\n") == 0) {
                    fwrite(replace, SIZE, NUMBER_OF_ELEMENTS, dst);
                    continue;
                } else {
                    fwrite(c, SIZE, NUMBER_OF_ELEMENTS, dst);
                    fwrite(indicator, SIZE, NUMBER_OF_ELEMENTS, dst);
                    continue;
                }
            }
        }
        fwrite(c, SIZE, NUMBER_OF_ELEMENTS, dst);
    }

}

/**
 * ***************************************************************
 * This function receives a source file and creates a copy file  *
 * ***************************************************************
 **/
void optionNumber1(char *source_file, char *destination_file) {
    FILE *src = NULL;
    FILE *dst = NULL;
    char buffer[MAX_LENGTH_STRING] = "";
    //check if file opening was success
    src = fopen(source_file, "rb");
    if (src == NULL) {
        exit(1);
    }
    dst = fopen(destination_file, "wb");
    if (dst == NULL) {
        fclose(src);
        exit(1);
    }
    //copy to a new file
    while (fread(buffer, SIZE, NUMBER_OF_ELEMENTS, src)) {
        fwrite(buffer, SIZE, NUMBER_OF_ELEMENTS, dst);
    }
    fclose(src);
    fclose(dst);
}