#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpletron.h"
#include "translator.h"

#include <ctype.h>

#define DEBUG_LT
#define DEBUG_MT

#undef DEBUG_LT
// #undef DEBUG_MT


size_t searchEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG_LT
    printf("Searching for %c", type);
    if (type == VAR) {
        printf(" %s\n",identifier.name);
    } else {
        printf(" %d\n", identifier.value);
    }
#endif
    struct listEntry *lookupListPtr = program->lookupList;
    while (lookupListPtr != NULL) {
#ifdef DEBUG_LT
        printf("Scanning object: %c", lookupListPtr->type);
        if (lookupListPtr->type == VAR) {
            printf(" %s", lookupListPtr->identifier.name);
        } else {
            printf(" %d", lookupListPtr->identifier.value);
        }
        printf(", address %ld\n", lookupListPtr->address);
#endif
        if (
            (type == VAR && strcmp(lookupListPtr->identifier.name, identifier.name) == 0)
            || ((type == CONST || type == LINE) && lookupListPtr->identifier.value == identifier.value)
        ) {
#ifdef DEBUG_LT
            printf("Found, address: %ld\n", lookupListPtr->address);
#endif
            return lookupListPtr->address;
        }
        lookupListPtr = lookupListPtr->next;
    }
#ifdef DEBUG_LT
    puts("Not found");
#endif
    return -1;
}

size_t addEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG_LT
    printf("Adding %c", type);
    if (type == VAR) {
        printf(" %s\n",identifier.name);
    } else {
        printf(" %d\n", identifier.value);
    }
#endif
    struct listEntry *newEntry = (struct listEntry *) malloc(sizeof(struct listEntry));
    if (newEntry == NULL) {
        puts("Error. Can't allocate memory");
        exit(1);
    }
    newEntry->identifier = identifier;
    newEntry->type = type;
    if (newEntry->type == LINE) {
        newEntry->address = program->instructionPtr;
    } else {
        newEntry->address = program->variablePtr;
        program->variablePtr--;
    }
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
#ifdef DEBUG_LT
    printf("Added object: %c", newEntry->type);
    if (newEntry->type == VAR) {
        printf(" %s", newEntry->identifier.name);
    } else {
        printf(" %d", newEntry->identifier.value);
    }
    printf(", address %ld\n", newEntry->address);
#endif
    return newEntry->address;
}


size_t searchOrAddEntry(
    struct program *program,
    const union identifier identifier,
    const enum entrytype type
) {
#ifdef DEBUG_LT
    printf("Searching and adding %c", type);
    if (type == VAR) {
        printf(" %s\n",identifier.name);
    } else {
        printf(" %d\n", identifier.value);
    }
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

void strip(char s1[], char s2[]) {
    /* Remove leading and trailing spaces and replace duplicated spaces in s2, save result in s1 */
    /* s2 stays unmodified */
    char buffer[BUFSIZE];
    strcpy(buffer, s2);
    int i = 0, j = 0;
    while (isspace(buffer[i])) i++;
    for (; buffer[i] != '\0'; i++) {
        if (isspace(buffer[i])) {
            if (j == 0 || !isspace(s1[j - 1]) ) {
                s1[j++] = ' ';
            }
        } else {
            s1[j++] = buffer[i];
        }
    }
    // Null-terminate the modified string
    s1[j] = '\0';
    // Trim a single trailing space if it exists
    if (j > 0 && isspace(s1[j - 1])) {
        s1[j-1] = '\0';
    }
}

void addMissing(struct program *program, const int identifier, const size_t address) {
#ifdef DEBUG_MT
    printf("Adding missing reference to row number %d for address %ld\n", identifier, address);
#endif
    struct missingEntry *entry = (struct missingEntry *) malloc(sizeof(struct missingEntry)), *missingPtr;
    *entry = (struct missingEntry) {.label=identifier, .address=address, .next=NULL};
    if (program->missingList == NULL) {
        program->missingList = entry;
    } else {
        missingPtr = program->missingList;
        while (missingPtr->next != NULL) {
#ifdef DEBUG_MT
            printf(
                "Found record for missing reference to row %d at address %ld\n",
                missingPtr->label, missingPtr->address
            );
#endif
            missingPtr = missingPtr->next;
        }
        missingPtr->next = entry;
#ifdef DEBUG_MT
        puts("Done.");
#endif
    }
}