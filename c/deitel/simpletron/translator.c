#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpletron.h"
#include "translator.h"

#include <ctype.h>

#define DEBUG


size_t searchEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG
    puts("Searching");
#endif
    struct listEntry *lookupListPtr = program->lookupList;
    while (lookupListPtr != NULL) {
#ifdef DEBUG
        printf("Object: %c ", lookupListPtr->type);
        if (lookupListPtr->type == VAR) {
            printf("%s ", lookupListPtr->identifier.name);
        } else {
            printf("%d ", lookupListPtr->identifier.value);
        }
        printf("%ld\n", lookupListPtr->address);
#endif
        if (
            (type == VAR && strcmp(lookupListPtr->identifier.name, identifier.name) == 0)
            || ((type == CONST || type == LINE) && lookupListPtr->identifier.value == identifier.value)
        ) {
#ifdef DEBUG
            printf("Found, address: %ld\n", lookupListPtr->address);
#endif
            return lookupListPtr->address;
        }
        lookupListPtr = lookupListPtr->next;
    }
    return -1;
}

size_t addEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG
    puts("Adding");
#endif
    struct listEntry *newEntry = (struct listEntry *) malloc(sizeof(struct listEntry));
    if (newEntry == NULL) {
        puts("Error. Can't allocate memory");
        exit(1);
    }
    newEntry->identifier = identifier;
    newEntry->type = type;
    newEntry->address = program->instructionPtr;
    newEntry->next = NULL;
    if (program->lookupList == NULL) {
        program->lookupList = newEntry;
    } else {
        struct listEntry *lookupListPtr = program->lookupList;
        while (lookupListPtr->next != NULL) {
            lookupListPtr = lookupListPtr->next;
        }
        lookupListPtr->next = newEntry;
    }
#ifdef DEBUG
    printf("Added object: %c ", newEntry->type);
    if (newEntry->type == VAR) {
        printf("%s ", newEntry->identifier.name);
    } else {
        printf("%d ", newEntry->identifier.value);
    }
    printf("%ld\n", newEntry->address);
#endif
    return newEntry->address;
}


size_t searchOrAddEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG
    puts("Searching and Adding");
#endif
    const size_t address = searchEntry(program, identifier, type);
    if (address != -1) {
        return address;
    }
    return addEntry(program, identifier, type);
}

void initProgram(struct program *program) {
    program->lookupList = NULL;
    program->missingList = NULL;
    for (size_t ptr = 0; ptr < MEMORY_SIZE; ptr++) {
        program->memory[ptr] = 0;
    }
    program->instructionPtr = 0;
    program->variablePtr = MEMORY_SIZE - 1;
}

bool checkIdentifier(char identifier[]) {
    if (strlen(identifier) == 0) {
        return false;
    }
    if (!isalpha(identifier[0])) {
        return false;
    }
    for (char *ptr = identifier; *ptr != '\0'; ptr++) {
        if (!(isalnum(*ptr) || *ptr == '_')) {
            return false;
        }
    }
    return true;
}