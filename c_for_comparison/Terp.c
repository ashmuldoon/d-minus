// Basic C in aid of later interpreter design in assembly

#include <stdio.h>
#include <stdlib.h>

#define TAPE_BITS 30000 // tape length, in bits now
#define TAPE_BYTES ((TAPE_BITS + 7) / 8)

// bit helpers: cell `p` lives in byte p/8, bit position p%8
#define GET_BIT(tape, p) ((tape[(p) / 8] >> ((p) % 8)) & 1)
#define FLIP_BIT(tape, p) (tape[(p) / 8] ^= (1 << ((p) % 8)))
#define SET_BIT(tape, p, v) \
    (v ? (tape[(p) / 8] |= (1 << ((p) % 8))) : (tape[(p) / 8] &= ~(1 << ((p) % 8))))

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
    int *stack = malloc(len * sizeof(int));
    int sp = 0;
    for (long i = 0; i < len; i++)
    {                       // this is why I'm not terribly pressed over the non null-terminated string we're storing
        if (code[i] == '[') // this could never go past len, that said, we should absolutely never use any string method or whatever
            stack[sp++] = i;
        else if (code[i] == ']')
        {
            int open = stack[--sp];
            match[open] = i;
            match[i] = open;
        }
    }
    free(stack);

    unsigned char tape[TAPE_BYTES] = {0}; // bit-addressed tape
    int ptr = 0;                          // now a BIT index, not byte index

    // defining terms (i dont think i could really improve on this in terms of memory usage, right?)

    for (long pc = 0; pc < len; pc++)
    {
        switch (code[pc])
        {
        case '>':
            ptr++;
            break;
        case '<':
            ptr--;
            break;
        case '!':
            FLIP_BIT(tape, ptr);
            break; // replaces + and -
        case '.':
        {
            // pack bits [ptr .. ptr+7] into a byte, ptr = MSB
            unsigned char out = 0;
            for (int b = 0; b < 8; b++)
                out |= GET_BIT(tape, ptr + b) << (7 - b);
            putchar(out);
            break;
        }
        case ',':
        {
            // unpack one input byte into bits [ptr .. ptr+7], ptr = MSB
            int in = getchar();
            if (in == EOF)
                in = 0; // decide EOF behavior explicitly
            for (int b = 0; b < 8; b++)
                SET_BIT(tape, ptr + b, (in >> (7 - b)) & 1);
            break;
        }
        case '[':
            if (!GET_BIT(tape, ptr))
                pc = match[pc];
            break;
        case ']':
            if (GET_BIT(tape, ptr))
                pc = match[pc];
            break;
        }
    }

    free(code); // look ma ! im memory safetying
    free(match);
    return 0;
}