// Basic C in aid of later interpreter design in assembly

#include <stdio.h>
#include <stdlib.h>

#define TAPE_SIZE 30000

int main(int argc, char **argv)
{
    FILE *f = fopen(argv[1], "rb"); // this never actually checks if argv[1] is a file, which is
                                    // horrifically bad practice, but all in the name of smoll
    fseek(f, 0, SEEK_END);
    long len = ftell(f); // find length
    rewind(f);

    char *code = malloc(len);
    fread(code, 1, len, f); // this is so unsafe its almost funny
    fclose(f);

    int *match = malloc(len * sizeof(int));
    int stack[len], sp = 0;

    for (long i = 0; i < len; i++) // this is why I'm not terribly pressed over the non null-terminated string we're storing
    {                              // this could never go past len, that said, we should absolutely never use any string method or whatever
        if (code[i] == '[')
            stack[sp++] = i;
        else if (code[i] == ']')
        {
            int open = stack[--sp]; // recursion check
            match[open] = i;
            match[i] = open;
        }
    }

    unsigned char tape[TAPE_SIZE] = {0};
    int ptr = 0;

    for (long pc = 0; pc < len; pc++)
    {
        switch (code[pc])
        {
        case '>': // defining terms (i dont think i could really improve on this in terms of memory usage, right?)
            ptr++;
            break;
        case '<':
            ptr--;
            break;
        case '+':
            tape[ptr]++;
            break;
        case '-':
            tape[ptr]--;
            break;
        case '.':
            putchar(tape[ptr]);
            break;
        case ',':
            tape[ptr] = getchar();
            break;
        case '[':
            if (!tape[ptr])
                pc = match[pc];
            break;
        case ']':
            if (tape[ptr])
                pc = match[pc];
            break;
        }
    }

    free(code); // look ma ! im memory safetying
    free(match);
    return 0;
}