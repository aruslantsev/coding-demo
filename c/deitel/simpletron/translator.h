#pragma once
#include <stddef.h>

#include "simpletron.h"
// #include "simpletron.h"


#define IDENTIFIER_SIZE 8
#define BUFSIZE         255

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
bool checkIdentifier(char []);
void initProgram(struct program *);
void strip(char [], char []);
void addMissing(struct program *, const int, const size_t);
