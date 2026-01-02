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
    char buffer[BUFFER_SIZE];
    int lineNumber = 0;
    union identifier identifier;
    size_t address;

    initProgram(&program);

    while (!feof(file)) {
        if (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            lineNumber++;
            printf("%s", buffer);
            /* Remove trailing newline symbols before tokenization */
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strip(buffer, buffer);
            printf("%s\n", buffer);
            /* Skip empty string */
            if (strlen(buffer) == 0) continue;
            parseLine(&program, buffer, lineNumber);
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
    puts("Writing program");
    FILE *textfile = fopen(argv[2], "w");
    char charinstr[WORD_BITS / 4 + 2];
    for (int instructionPtr = 0; instructionPtr < MEMORY_SIZE; instructionPtr++) {
        sprintf(charinstr, "%*X\n", WORD_BITS / 4, (uword_t) program.memory[instructionPtr]);
        fprintf(textfile, "%s", charinstr);
    }
    fclose(textfile);
    return 0;
}
