#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "translator.h"
#include "simpletron.h"


#define DEBUG

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        puts("Usage:");
        printf("\t%s\tFILENAME.bas OUTFILE.sml\n", argv[0]);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        puts("Error opening file");
        exit(1);
    }

    struct program program;
    char buffer[BUFSIZE], tmpBuffer[BUFSIZE], *bufPtr, *bufferStart;
    int lineNumber = 0;
    union identifier identifier;
    size_t address, strPosition;
    word_t instruction;

    initProgram(&program);

    while (!feof(file)) {
        if (fgets(buffer, BUFSIZE, file) != NULL) {
            lineNumber++;
            printf("%s", buffer);
            /* Remove trailing newline symbols before tokenization */
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strip(buffer, buffer);
            printf("%s\n", buffer);
            /* Skip empty string */
            if (strlen(buffer) == 0) continue;

            strcpy(tmpBuffer, buffer);
            bufferStart = tmpBuffer;

            /* First token: line number */
            bufPtr = strtok(tmpBuffer, " ");
            for (char *ptr = bufPtr; *ptr != '\0'; ptr++) {
                if (!isdigit(*ptr)) {
                    printf("Wrong line number in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                }
            }
            identifier.value = atoi(bufPtr);

            if (searchEntry(&program, identifier, LINE) != -1) {
                printf("Error: duplicated line number '%d' in line %d\n", identifier.value, lineNumber);
                printf("%s\n", buffer);
            }
            address = addEntry(&program, identifier, LINE);
#ifdef DEBUG
            printf("Added entry for line %d in table, address %ld\n", identifier.value, address);
#endif

            /* Second token: rem, let, input, print, goto, if ... goto, end */
            bufPtr = strtok(NULL, " ");
            if (bufPtr == NULL) continue;  // Empty line
            if (strcmp("rem", bufPtr) == 0) continue;  // Skip comments
            if (strcmp("input", bufPtr) == 0) {
                bufPtr = strtok(NULL, " ,");  // Tokenize remaining string by space of comma
                if (bufPtr == NULL) {
                    printf("Missing variable name after INPUT keyword in line %d", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                while (bufPtr != NULL) {
                    if (!checkIdentifier(bufPtr)) {
                        printf("Wrong variable name %s in line %d", bufPtr, lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                    strncpy(identifier.name, bufPtr, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
                    printf("Adding variable %s\n", identifier.name);
#endif
                    address = searchOrAddEntry(&program, identifier, VAR);
                    if (address == -1) {
                        printf("Unknown error in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                    instruction = READ << OPERAND_BITS | address;
                    program.memory[program.instructionPtr++] = instruction;
                    bufPtr = strtok(NULL, " ,");
                }
            }
            else if (strcmp("print", bufPtr) == 0) {
                bufPtr = strtok(NULL, " ,");
                if (bufPtr == NULL) {
                    printf("Missing variable name after PRINT keyword in line %d", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                while (bufPtr != NULL) {
                    if (!checkIdentifier(bufPtr)) {
                        printf("Wrong variable name %s in line %d", bufPtr, lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                    strncpy(identifier.name, bufPtr, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
                    printf("Adding variable %s\n", identifier.name);
#endif
                    address = searchOrAddEntry(&program, identifier, VAR);
                    if (address == -1) {
                        printf("Unknown error in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                    instruction = WRITE << OPERAND_BITS | address;
                    program.memory[program.instructionPtr++] = instruction;
                    bufPtr = strtok(NULL, " ,");
                }
            }
            else if (strcmp("let", bufPtr) == 0) {
                bufPtr = strtok(NULL, " =");
                if (!checkIdentifier(bufPtr)) {
                    printf("Wrong variable name %s in line %d", bufPtr, lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                if (bufPtr == NULL) {
                    printf("Missing expression after LET keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                strncpy(identifier.name, bufPtr, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
                printf("Adding variable %s\n", identifier.name);
#endif
                address = searchOrAddEntry(&program, identifier, VAR);
                if (address == -1) {
                    printf("Unknown error in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                strPosition = bufPtr - bufferStart + strlen(bufPtr);  // End of variable name
                /* Check space between variable name and '='. Should be only spaces */
                size_t strPtr;
                for (strPtr = strPosition; buffer[strPtr] != '='; strPtr++) {
                    printf("%c", buffer[strPtr]);
                    /* End of line before assignment */
                    if (buffer[strPtr] == '\0') {
                        printf("Unexpected end of line in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                    /* Non-space symbol */
                    if (buffer[strPtr] != ' ') {
                        printf("Wrong format or multiple assignment in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                        exit(1);
                    }
                }
                strPtr++; /* At '=' now */
                /* Copy expression to evaluate */
                strcpy(tmpBuffer, &buffer[strPtr]);
                strip(tmpBuffer, tmpBuffer);
#ifdef DEBUG
                printf("Evaluating '%s'\n", tmpBuffer);
#endif
                bool notEmpty = false;
                for (const char *c = tmpBuffer; *c != '\0'; c++) {
                    if (isalnum(*c)) notEmpty = true;
                    if (*c == '=') {
                        printf("Multiple assignment in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                    }
                }
                if (!notEmpty) {
                    printf("Wrong assignment. Nothing assinged in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                }
                // FIXME
                /* Expression now is in tmpBuffer. Parse and calculate */
            }
            else if (strcmp("goto", bufPtr) == 0) {
                bufPtr = strtok(NULL, " ");
                if (bufPtr == NULL) {
                    printf("Missing line number after GOTO keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                for (size_t i = 0; bufPtr[i] != '\0'; i++) {
                    if (!isdigit(bufPtr[i])) {
                        printf("Bad line number in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                    }
                }
                identifier.value = atoi(bufPtr);
                bufPtr = strtok(NULL, " ");
                if (bufPtr != NULL) {
                    printf("Error: multiple line numbers after GOTO keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                if ((address = searchEntry(&program, identifier, LINE)) == -1) {
                    addMissing(&program, identifier.value, program.instructionPtr);
                    address = 0;
                }
                instruction = BRANCH << OPERAND_BITS | address;
                program.memory[program.instructionPtr++] = instruction;
            }
            else if (strcmp("if", bufPtr) == 0) {
                strPosition = bufPtr - bufferStart + strlen(bufPtr);  // Start of left expression
                strcpy(tmpBuffer, &buffer[strPosition]);
                strip(tmpBuffer, tmpBuffer);
                printf("Remaining string: %s\n", tmpBuffer);
                char *endpos = strstr(tmpBuffer, "goto");
                if (endpos == NULL) {
                    printf("Missing GOTO in IF statement in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                char expr[BUFSIZE];
                strncpy(expr, tmpBuffer, endpos - tmpBuffer);
                bufPtr = endpos + 4;
                bufPtr = strtok(bufPtr, " ");
                if (bufPtr == NULL) {
                    printf("Missing line number after IF..GOTO keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                for (size_t i = 0; bufPtr[i] != '\0'; i++) {
                    if (!isdigit(bufPtr[i])) {
                        printf("Bad line number in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                    }
                }
                identifier.value = atoi(bufPtr);
                printf("Expression: '%s', row: '%d'\n", expr, identifier.value);
                bufPtr = strtok(NULL, " ");
                if (bufPtr != NULL) {
                    printf("Error: multiple line numbers after IF..GOTO keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                if ((address = searchEntry(&program, identifier, LINE)) == -1) {
                    addMissing(&program, identifier.value, program.instructionPtr);
                    address = 0;
                }

                /* Split comparison expression by !=<> */
                /* Validate expression */
                /* Evaluate left expression. Evaluate right expression. Transform expression and add sign */
                /* Compare values */
                /* Create GOTO instructions */

                // FIXME
                instruction = BRANCH << OPERAND_BITS | address;
                program.memory[program.instructionPtr++] = instruction;
            }
            else if (strcmp("end", bufPtr) == 0) {
                instruction = HALT << OPERAND_BITS;
                program.memory[program.instructionPtr++] = instruction;
            } else {
                printf("Wrong token '%s' in line %d", bufPtr, lineNumber);
                printf("%s\n", buffer);
            }
        }
    }

    /* Fill missing pointers */
#ifdef DEBUG
    puts("Resolving remaining references");
#endif
    const struct missingEntry *missingPtr = program.missingList;
    while (missingPtr != NULL) {

        identifier.value = missingPtr->label;
        address = searchEntry(&program, identifier, LINE);
        if (address == -1) {
            printf("Unresolved label %d\n", missingPtr->label);
            exit(1);
        }
#ifdef DEBUG
        printf(
            "Instruction at address %ld references line %d. Real address: %ld\n",
            missingPtr->address, missingPtr->label, address
        );
#endif
        program.memory[missingPtr->address] |= address;
        missingPtr = missingPtr->next;
    }
    fclose(file);

    /* Write program */
    return 0;
}
