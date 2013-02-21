/*
 * roboshell.h
 *
 *  Created on: Feb 12, 2013
 *      Author: armin
 */

#ifndef ROBOSHELL_H_
#define ROBOSHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

#define EOL -666
#define ERR -777


typedef struct Scanner {
    char *curpos;
    unsigned int line;
} Scanner;


typedef struct Token {
    char *s;
    int n;
    int id;
} Token;

unsigned int scan(Scanner *s, Token *t);


#endif /* ROBOSHELL_H_ */
