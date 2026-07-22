// Basic C in aid of later interpreter design in assembly

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I can optimise the above away.

#define STACK_SIZE 256
static long stack[STACK_SIZE];
static int sp = 0;

// Stack based language, a choice I think is clever though: I reckon static will take a couple bytes off over dynamic in this case. Have to test that.

static void push(long v) { stack[sp++] = v; }
static long pop(void) { return stack[--sp]; }

// primitive ops

static void wadd(void)
{
    long b = pop(), a = pop();
    push(a + b);
}
static void wsub(void)
{
    long b = pop(), a = pop();
    push(a - b);
}
static void wmul(void)
{
    long b = pop(), a = pop();
    push(a * b);
}
static void wdiv(void)
{
    long b = pop(), a = pop();
    push(a / b);
}
static void wdup(void)
{
    long a = pop();
    push(a);
    push(a);
}
static void wdrop(void) { pop(); }
static void wswap(void)
{
    long b = pop(), a = pop();
    push(b);
    push(a);
}
static void wover(void)
{
    long b = pop(), a = pop();
    push(a);
    push(b);
    push(a);
}
static void wdot(void) { printf("%ld ", pop()); }

// Define the word funcs here, we won't be using a lexer I don't think so KISS.

typedef struct
{
    const char *name;
    void (*fn)(void);
} Word;

static Word dict[] = {
    {"+", wadd},
    {"-", wsub},
    {"*", wmul},
    {"/", wdiv},
    {"&", wdup},
    {"_", wdrop},
    {"~", wswap},
    {"@", wover},
    {".", wdot},
};
#define NWORDS (sizeof(dict) / sizeof(dict[0]))

// dict

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    char *tok = strtok(argv[1], " \t\n");
    while (tok)
    {
        char *end;
        long n = strtol(tok, &end, 10);
        if (*end == '\0')
        {
            push(n);
        }
        else
        {
            for (size_t i = 0; i < NWORDS; i++)
            {
                if (strcmp(tok, dict[i].name) == 0)
                {
                    dict[i].fn();
                    break;
                }
            }
        }
        tok = strtok(NULL, " \t\n");
    }
    printf("\n");
    return 0;
}

// All of the logic here fits into this neat 30 word main. I'd shed a tear if it weren't still so large, we can do better.