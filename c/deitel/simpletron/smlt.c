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
            /* Skip empty string */
            if (strlen(buffer) == 0) continue;

            strcpy(tmpBuffer, buffer);
            bufferStart = tmpBuffer;

            /* First token: line number */
            bufPtr = strtok(tmpBuffer, " ");
            // TODO: check that it is integer
            identifier.value = atoi(bufPtr);

            if (searchEntry(&program, identifier, LINE) != -1) {
                printf("Error in line '%d': duplicated line number %d\n", lineNumber, identifier.value);
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
                bufPtr = strtok(NULL, " ,");
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
                    printf("Missing expression after PRINT keyword in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                strncpy(identifier.name, bufPtr, IDENTIFIER_SIZE - 1);
                address = searchOrAddEntry(&program, identifier, VAR);
                if (address == -1) {
                    printf("Unknown error in line %d\n", lineNumber);
                    printf("%s\n", buffer);
                    exit(1);
                }
                strPosition = bufPtr - bufferStart + strlen(bufPtr);  // End of variable name
                for (size_t i = strPosition; buffer[i] != '=' && buffer[i] != '\0'; i++) {
                    if (buffer[i] != ' ') {
                        printf("Wrong format or multiple assignment in line %d\n", lineNumber);
                        printf("%s\n", buffer);
                    }
                }


                bufPtr = strtok(NULL, " =");
                if (bufPtr == NULL) {exit(1);} // TODO: fix message
                strPosition = bufPtr - bufferStart;
                // TODO: parse remaining expression
            }
            else if (strcmp("goto", bufPtr) == 0) {}
            else if (strcmp("if", bufPtr) == 0) {}
            else if (strcmp("end", bufPtr) == 0) {
                instruction = HALT << OPERAND_BITS;
                program.memory[program.instructionPtr++] = instruction;
            } else {
                printf("Wrong token '%s' in line %d", bufPtr, lineNumber);
                printf("s\n", buffer);
            }
        }
    }
    fclose(file);
    return 0;
}
