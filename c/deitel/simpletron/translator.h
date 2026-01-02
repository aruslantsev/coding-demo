#pragma once
#include <stddef.h>

#include "simpletron.h"
// #include "simpletron.h"


#define IDENTIFIER_SIZE     8
#define BUFFER_SIZE         255

enum entrytype {CONST = 'c', LINE = 'l', VAR = 'v'};

union identifier {
    int value;
    char name[IDENTIFIER_SIZE];
};

struct listEntry {
    union identifier    identifier;
    enum entrytype      type;
    size_t              address;
    struct listEntry    *next;
};

struct missingEntry {
    int                 label;
    size_t              address;
    struct missingEntry *next;
};

struct program {
    struct listEntry    *lookupList;
    struct missingEntry *missingList;
    word_t              memory[MEMORY_SIZE];
    size_t              instructionPtr;
    size_t              variablePtr;
};


size_t searchEntry(struct program *, const union identifier, const enum entrytype);
size_t addEntry(struct program *, const union identifier, const enum entrytype);
size_t searchOrAddEntry(struct program *, const union identifier, const enum entrytype);
void addMissing(struct program *, const int);
bool checkIdentifier(char []);
bool checkInteger(char []);
void initProgram(struct program *);
void strip(char [], char []);
void parseLine(struct program *, char [], const int);
void parseInput(struct program *, char [], const int);
void parsePrint(struct program *, char [], const int);
void parseGoto(struct program *, char [], const int);
void parseLet(struct program *, char [], const int);
void parseIf(struct program *, char [], const int);


#define TOKEN_SIZE  20

enum tokenType {
    IDENTIFIER='i', OPERATION='o', BRACE='b'
};

struct exprToken {
    char token[TOKEN_SIZE];
    char token_type;
    struct exprToken *next;
};


struct exprToken *parseExpression(char []);
bool transformExpression(struct exprToken *);

int cmpOperations(char, char);
bool isArithmeticOp(const char);
bool isBrace(const char);
