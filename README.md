# d-

An incredibly simple interpreter for a language fittingly dubbed 'd-'. Though I'm sure my genius leaks through the contributions, it should be readily apparent that this is not a language designed to be used, and is instead an exercise.

This is effectively a BF interpreter by any other name, only with the added limitation of space optimization. There are, obviously, many ways to "cheat" this goal. As such, I am disallowing myself from using libSystem (I run MacOS) wherever possible. This may lead to instability, as Apple truly hates syscalls, but I will try my best.

Current size on disk: 37KB

This is effectively the minimum size for a file on MacOS. I am not satisfied with this result, but improving upon it will take time to port to Windows. I will be back.

## Project Aims

- To develop a interpreter for an incredibly sparse language, containing only the most basic functionalities (in essence a turing machine a là Urban Müller's 1993 esolang 'Brainfuck' or a very basic interpretation of Forth).
- To do so with as compact an interpreter (as measured by the size of the compiled executable) as is possible.
- To learn and showcase interpreter design in a more intuitive, hands-on way.

## What this project is NOT aiming for

- Any degree of optimisation for speed. Indeed, the only metric by which I'll measure success here is in bytes and not clock cycles.
- Readability: See above.
- A functional - or sensical - programming language: See above above.

## TODO:

☑ Interpreter roadmap (Quite important)
|
|- ☑ Lexer implementation (unnecessary, i was clearly overprepared lol)
|
|- ☑ Parser implementation
|  
|- ☑ Interpreter implementation

☑ Language design (Less important)
|
|- ☑ Find minimum possible space for each token to take up (likely one ASCII character to comply with most text editors).
| |
| |- ☑ Investigate possible alternatives to ascii (none were satisfying)
|
|- ☑ Design a language around the concept of no variable or lexeme exceeding one ASCII character - the lexer will have an easy job. (Unfortunately I ended up reinventing Boolfuck, a language I know exists only after I remade it lol)

☑ Final assembly write
