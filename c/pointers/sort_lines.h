#define MAX_LINESIZE 1000
#define MAX_LINES 1000
#define MAX_ALLOC_SIZE 10000


char* alloc(unsigned long);
long unsigned int get_line(char []);
void strcopy(char [], char []);
long unsigned int get_lines(char **, long unsigned int);
void print_lines(char **, long unsigned int);
void qsort(char *[], long unsigned int, long unsigned int);
void bubblesort(char *[], long unsigned int, long unsigned int);
