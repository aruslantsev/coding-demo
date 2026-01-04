#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpletron.h"
#include "translator.h"
#include "evaluate.h"

#include <ctype.h>

#define DEBUG_LT
#define DEBUG_MT
#define DEBUG

// #undef DEBUG_LT
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
            (type == VAR && type == lookupListPtr->type && strcmp(lookupListPtr->identifier.name, identifier.name) == 0)
            || ((type == CONST || type == LINE)  && type == lookupListPtr->type && lookupListPtr->identifier.value == identifier.value)
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
        if (type == CONST) {
            program->memory[program->variablePtr] = identifier.value;
#ifdef DEBUG_LT
            printf(
                "Writing constant value %d (%0X) at %ld\n", 
                identifier.value, (word_t) identifier.value, program->variablePtr
            );
#endif
        }
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


void addMissing(struct program *program, const int identifier) {
#ifdef DEBUG_MT
    printf("Adding missing reference to row number %d for address %ld\n", identifier, program->instructionPtr);
#endif
    struct missingEntry *entry = (struct missingEntry *) malloc(sizeof(struct missingEntry)), *missingPtr;
    *entry = (struct missingEntry) {.label=identifier, .address=program->instructionPtr, .next=NULL};
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
    if (strlen(identifier) == 0) return false;
    if (!isalpha(identifier[0]) && identifier[0] != '_') return false;
    for (char *ptr = identifier; *ptr != '\0'; ptr++) {
        if (!(isalnum(*ptr) || *ptr == '_')) {
            return false;
        }
    }
    return true;
}


bool checkInteger(char value[]) {
    if (strlen(value) == 0) return false;
    const char *ptr = value;
    bool has_digits = false;
    if (*ptr == '-' || *ptr == '+') ptr++;
    for (; *ptr != '\0'; ptr++) {
        if (!isdigit(*ptr)) return false;
        else has_digits = true;
    }
    return has_digits;
}


void strip(char s1[], char s2[]) {
    /* Remove leading and trailing spaces and replace duplicated spaces in s2, save result in s1 */
    /* s2 stays unmodified */
    char buffer[BUFFER_SIZE];
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
        s1[j - 1] = '\0';
    }
}


void parseLine(struct program *program, char line[], const int lineNumber) {
    char buffer[BUFFER_SIZE];
    union identifier identifier;
    strcpy(buffer, line);

    /* First token: line number */
    char *token = strtok(buffer, " ");
    if (!checkInteger(token)) {
        printf("Wrong line number in line %d\n", lineNumber);
        printf("%s\n", line);
    }
    identifier.value = atoi(token);
    if (identifier.value < 0) {
        printf("Line number %d should be non-negative in line %d\n", identifier.value, lineNumber);
        printf("%s\n", line);
        exit(1);
    }

    if (searchEntry(program, identifier, LINE) != -1) {
        printf("Error: duplicated line number '%d' in line %d\n", identifier.value, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    const size_t address = addEntry(program, identifier, LINE);
#ifdef DEBUG
    printf("Added entry for line %d in table, address %ld\n", identifier.value, address);
#endif

    /* Second token: rem, let, input, print, goto, if ... goto, end */
    token = strtok(NULL, " ");
    if (token == NULL) return;  // Empty line
    if (strcmp("rem", token) == 0) return;  // Skip comments
    if (strcmp("input", token) == 0) parseInput(program, line, lineNumber);
    else if (strcmp("print", token) == 0) parsePrint(program, line, lineNumber);
    else if (strcmp("let", token) == 0) parseLet(program, line, lineNumber);
    else if (strcmp("goto", token) == 0) parseGoto(program, line, lineNumber);
    else if (strcmp("if", token) == 0) parseIf(program, line, lineNumber);
    else if (strcmp("end", token) == 0) {
        const word_t instruction = HALT << OPERAND_BITS;
        program->memory[program->instructionPtr++] = instruction;
    } else {
        printf("Wrong token '%s' in line %d\n", token, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
}


void parseInput(struct program *program, char line[], const int lineNumber) {
    word_t instruction;
    char buffer[BUFFER_SIZE];
    union identifier identifier;
    size_t address;

    strcpy(buffer, line);
    strtok(buffer, " ");  // Skip line number
    strtok(NULL, " ");  // Skip keyword
    char *token = strtok(NULL, " ,");  // Tokenize remaining string by space of comma
    if (token == NULL) {
        printf("Missing variable name after INPUT keyword in line %d", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    while (token != NULL) {
        if (!checkIdentifier(token)) {
            printf("Wrong variable name %s in line %d", token, lineNumber);
            printf("%s\n", line);
            exit(1);
        }
        strncpy(identifier.name, token, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
        printf("Adding variable %s\n", identifier.name);
#endif
        address = searchOrAddEntry(program, identifier, VAR);
        if (address == -1) {
            printf("Unknown error in line %d\n", lineNumber);
            printf("%s\n", line);
            exit(1);
        }
        instruction = READ << OPERAND_BITS | address;
        program->memory[program->instructionPtr++] = instruction;
        token = strtok(NULL, " ,");
    }
}


void parsePrint(struct program *program, char line[], const int lineNumber) {
    word_t instruction;
    char buffer[BUFFER_SIZE];
    union identifier identifier;
    size_t address;

    strcpy(buffer, line);
    strtok(buffer, " ");  // Skip line number
    strtok(NULL, " ");  // Skip keyword
    char *token = strtok(NULL, " ,");  // Tokenize remaining string by space of comma
    if (token == NULL) {
        printf("Missing variable name after PRINT keyword in line %d", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    while (token != NULL) {
        if (!checkIdentifier(token)) {
            printf("Wrong variable name %s in line %d", token, lineNumber);
            printf("%s\n", line);
            exit(1);
        }
        strncpy(identifier.name, token, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
        printf("Adding variable %s\n", identifier.name);
#endif
        address = searchOrAddEntry(program, identifier, VAR);
        if (address == -1) {
            printf("Unknown error in line %d\n", lineNumber);
            printf("%s\n", line);
            exit(1);
        }
        instruction = WRITE << OPERAND_BITS | address;
        program->memory[program->instructionPtr++] = instruction;
        token = strtok(NULL, " ,");
    }
}


void parseGoto(struct program *program, char line[], const int lineNumber) {
    word_t instruction;
    char buffer[BUFFER_SIZE];
    union identifier identifier;
    size_t address;

    strcpy(buffer, line);
    strtok(buffer, " ");  // Skip line number
    strtok(NULL, " ");  // Skip keyword
    char *token = strtok(NULL, " ");
    if (token == NULL) {
        printf("Missing line number after GOTO keyword in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    if (!checkInteger(token)) {
        printf("Bad line number in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    identifier.value = atoi(token);
    if (identifier.value < 0) {
        printf("Line number %d should be non-negative in line %d\n", identifier.value, lineNumber);
        printf("%s\n", line);
        exit(1);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        printf("Error: multiple line numbers after GOTO keyword in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    if ((address = searchEntry(program, identifier, LINE)) == -1) {
        addMissing(program, identifier.value);
        address = 0;
    }
#ifdef DEBUG
    printf("GOTO line %d\n", identifier.value);
#endif
    instruction = BRANCH << OPERAND_BITS | address;
    program->memory[program->instructionPtr++] = instruction;
}


void parseLet(struct program *program, char line[], const int lineNumber) {
    char buffer[BUFFER_SIZE];
    union identifier identifier;

    strcpy(buffer, line);
    char *bufferStart = buffer;
    strtok(buffer, " ");  // Skip line number
    strtok(NULL, " ");  // Skip keyword

    char *token = strtok(NULL, " =");
    if (!checkIdentifier(token)) {
        printf("Wrong variable name %s in line %d", token, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    if (token == NULL) {
        printf("Missing expression after LET keyword in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    strncpy(identifier.name, token, IDENTIFIER_SIZE - 1);
#ifdef DEBUG
    printf("Adding variable %s\n", identifier.name);
#endif
    size_t left_address = searchOrAddEntry(program, identifier, VAR);
    if (left_address == -1) {
        printf("Unknown error in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    size_t strPtr = token - bufferStart + strlen(token);  // End of variable name
    /* Check space between variable name and '='. Should be only spaces */
    for (; line[strPtr] != '='; strPtr++) {
        /* End of line before assignment */
        if (line[strPtr] == '\0') {
            printf("Unexpected end of line in line %d\n", lineNumber);
            printf("%s\n", line);
            exit(1);
        }
        /* Non-space symbol */
        if (line[strPtr] != ' ') {
            printf("Wrong format or multiple assignment in line %d\n", lineNumber);
            printf("%s\n", line);
            exit(1);
        }
    }
    strPtr++; /* At '=' now */
    /* Copy expression to evaluate */
    strcpy(buffer, &line[strPtr]);
    strip(buffer, buffer);
#ifdef DEBUG
    printf("Validating '%s'\n", buffer);
#endif
    if (!checkExpression(buffer)) {
        printf("Invalid expression '%s' on line %d\n", buffer, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    
    if (!evalExpression(buffer, program)) {
        printf("Invalid expression '%s' in line %d\n", buffer, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    word_t instruction = STORE << OPERAND_BITS | left_address;
    program->memory[program->instructionPtr++] = instruction;

}


void parseIf(struct program *program, char line[], const int lineNumber) {
    word_t instruction;
    char buffer[BUFFER_SIZE];
    union identifier identifier;
    size_t address;

    strcpy(buffer, line);
    const char *bufferStart = buffer;
    strtok(buffer, " ");  // Skip line number
    char *token = strtok(NULL, " ");  // Skip keyword

    strcpy(buffer, &line[token - bufferStart + strlen(token)]); // Copy from start of left expression
    strip(buffer, buffer);
#ifdef DEBUG
    printf("Parsing condition: %s\n", buffer);
#endif
    char *conditionEnd = strstr(buffer, "goto");
    if (conditionEnd == NULL) {
        printf("Missing GOTO in IF statement in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    char expr[BUFFER_SIZE];
    strncpy(expr, buffer, conditionEnd - buffer);
    expr[conditionEnd - buffer] = '\0';
    conditionEnd += 4;  // Skip GOTO keyword
    token = strtok(conditionEnd, " ");
    if (token == NULL) {
        printf("Missing line number after IF..GOTO keyword in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    if (!checkInteger(token)) {
        printf("Bad line number in line %d\n", lineNumber);
        printf("%s\n", line);
    }
    identifier.value = atoi(token);
    if (identifier.value < 0) {
        printf("Line number %d should be non-negative in line %d\n", identifier.value, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    bool add_missing = false;
    if ((address = searchEntry(program, identifier, LINE)) == -1) {
        add_missing = true;
        address = 0;
    }
#ifdef DEBUG
    printf("Condition: '%s', goto row: '%d'\n", expr, identifier.value);
#endif
    token = strtok(NULL, " ");
    if (token != NULL) {
        printf("Error: multiple line numbers after IF..GOTO keyword in line %d\n", lineNumber);
        printf("%s\n", line);
        exit(1);
    }

    char leftExpr[BUFFER_SIZE], rightExpr[BUFFER_SIZE], transformed[BUFFER_SIZE];
    char *conditionPosition;
    enum comparison comparison;

    if ((conditionPosition = strstr(expr, "<=")) != NULL) {
        comparison = LE;
    } else if ((conditionPosition = strstr(expr, ">=")) != NULL) {
        comparison = GE;
    } else if ((conditionPosition = strstr(expr, "<")) != NULL) {
        comparison = LT;
    } else if ((conditionPosition = strstr(expr, ">")) != NULL) {
        comparison = GT;
    } else if ((conditionPosition = strstr(expr, "==")) != NULL) {
        comparison = EQ;
    } else if ((conditionPosition = strstr(expr, "!=")) != NULL) {
        comparison = NE;
    } else {
        printf("Can't find condition in expression '%s' in line %d\n", expr, lineNumber);
        printf("%s\n", line);
        exit(1);
    }

    strncpy(leftExpr, expr, conditionPosition - expr);
    leftExpr[conditionPosition - expr] = '\0';
    strcpy(rightExpr, conditionPosition + ((comparison == LT || comparison == GT)? 1: 2));

    switch (comparison) {
        case LE:
        case LT:
        case EQ:
        case NE:
            sprintf(transformed, "(%s) - (%s)", leftExpr, rightExpr);
            break;
        case GE:
        case GT:
            sprintf(transformed, "(%s) - (%s)", rightExpr, leftExpr);
            break;
        default:
            break;
    }

    printf("%s\n", transformed);
    if (!checkExpression(transformed)) {
        printf("Invalid expression '%s' on line %d\n", buffer, lineNumber);
        printf("%s\n", line);
        exit(1);
    }
    
    if (!evalExpression(transformed, program)) {
        printf("Invalid expression '%s' in line %d\n", buffer, lineNumber);
        printf("%s\n", line);
        exit(1);
    }

    switch (comparison) {
        case LE:
        case GE:
            if (add_missing) addMissing(program, identifier.value);
            instruction = BRANCHNEG << OPERAND_BITS | address;
            program->memory[program->instructionPtr++] = instruction;
            if (add_missing) addMissing(program, identifier.value);
            instruction = BRANCHZERO << OPERAND_BITS | address;
            program->memory[program->instructionPtr++] = instruction;
            break;
        case LT:
        case GT:
            if (add_missing) addMissing(program, identifier.value);
            instruction = BRANCHNEG << OPERAND_BITS | address;
            program->memory[program->instructionPtr++] = instruction;
            break;
        case EQ:
            if (add_missing) addMissing(program, identifier.value);
            instruction = BRANCHZERO << OPERAND_BITS | address;
            program->memory[program->instructionPtr++] = instruction;
            break;
        case NE:
            size_t fakeaddr = program->instructionPtr + 2;
            instruction = BRANCHZERO << OPERAND_BITS | fakeaddr;
            program->memory[program->instructionPtr++] = instruction;
            if (add_missing) addMissing(program, identifier.value);
            instruction = BRANCH << OPERAND_BITS | address ;
            program->memory[program->instructionPtr++] = instruction;
            break;
        default:
            break;
    }
}


bool evalExpression(char buffer[], struct program *program) {
    /* Evaluates expression and places result into accumulator */
    word_t instruction;
    /* Tokenizing */
#ifdef DEBUG
    puts("Tokenizing expression and converting to postfix notation...");
#endif
    struct exprToken *expr_start = tokenizeExpression(buffer);
    if (expr_start == NULL) {
        printf("Invalid expressoin '%s'\n", buffer);
        return false;
    }

    struct exprToken *current_token = expr_start;
#ifdef DEBUG
    puts("Generating code...");
    puts("First pass. Allocating memory for variables and constants");
#endif
    while (current_token != NULL) {
#ifdef DEBUG
        printf("Token %s %c\n", current_token->token, current_token->token_type);
#endif
        if (current_token->token_type == IDENTIFIER) {
            union identifier identifier;
            if (checkInteger(current_token->token)) {
                identifier.value = atoi(current_token->token);
                searchOrAddEntry(program, identifier, CONST);
            } else if (checkIdentifier(current_token->token)) {
                strcpy(identifier.name, current_token->token);
                searchOrAddEntry(program, identifier, VAR);
            } else {
                printf("%s is not a valid identifiers\n", current_token->token);
                return false;
            }
        }
        current_token = current_token->next;
    }

#ifdef DEBUG
    puts("Second pass. Generating code");
#endif
    /* Generate code */
    word_t currentStackPointer = program->variablePtr;
    current_token = expr_start;
    
    size_t address;
    while (current_token != NULL) {
        printf("Token %s\n", current_token->token);
        if (current_token->token_type == IDENTIFIER) {
            // Load to stack
            union identifier identifier;
            if (checkInteger(current_token->token)) {
                identifier.value = atoi(current_token->token);
                address = searchEntry(program, identifier, CONST);
            } else if (checkIdentifier(current_token->token)) {
                strcpy(identifier.name, current_token->token);
                address = searchEntry(program, identifier, VAR);
            } else {
                printf("%s is not a valid identifier\n", current_token->token);
                return false;
            }
            instruction = LOAD << OPERAND_BITS | address;
            printf("FROM MEM %X\n", (word_t) instruction);
            program->memory[program->instructionPtr++] = instruction;
            address = program->variablePtr--;
            instruction = STORE << OPERAND_BITS | address;
            printf("TO STACK %X\n", (word_t) instruction);
            program->memory[program->instructionPtr++] = instruction;
        } else if (current_token->token_type == OPERATION) {
            instruction = LOAD << OPERAND_BITS | ++(program->variablePtr);
            printf("FROM STACK %X\n", (word_t) instruction);
            program->memory[program->instructionPtr++] = instruction;
            switch (current_token->token[0]) {
                case '+':
                    instruction = ADD;
                    break;
                case '-':
                    instruction = SUBTRACT;
                    break;
                case '*':
                    instruction = MULTIPLY;
                    break;
                case '/':
                    instruction = DIVIDE;
                    break;
                case '%':
                    instruction = REMAINDER;
                    break;
                default:
                    printf("Unknown operation '%c'\n", current_token->token[0]);
                    return false;
            }
            instruction = instruction << OPERAND_BITS | ++(program->variablePtr);
            printf("ARITH_OP %X\n", (word_t) instruction);
            program->memory[program->instructionPtr++] = instruction;
            instruction = STORE << OPERAND_BITS | program->variablePtr--;
            printf("TO STACK %X\n", (word_t) instruction);
            program->memory[program->instructionPtr++] = instruction;
        } else {
            printf("Wrong token %s\n", current_token->token);
            return false;
        }
        current_token = current_token->next;
    }
    /* Cleanup */
    current_token = expr_start;
    while (current_token != NULL) {
        expr_start = current_token->next;
        free(current_token);
        current_token = expr_start;
    }

    instruction = LOAD << OPERAND_BITS | ++(program->variablePtr);
    program->memory[program->instructionPtr++] = instruction;
#ifdef DEBUG
    if (program->variablePtr != currentStackPointer) {
        puts("Stack is in dirty state");
    }
#endif
    return true;
}


bool checkExpression (const char buffer[]) {
    bool notEmpty = false;
    for (const char *c = buffer; *c != '\0'; c++) {
        if (isalnum(*c)) notEmpty = true;
        if (*c == '=' || *c == '!' || *c == '<' || *c == '>') {
            printf("Illegal character '%c' in expression '%s'\n", *c, buffer);
            return false;
        }
    }
    if (!notEmpty) {
        puts("Empty expression.");
        return false;
    }
    return true;
}
