# d-
An incredibly simple interpreter for a language fittingly dubbed 'd-'. Though I'm sure my genius leaks through the contributions, it should be readily apparent that this is not a language designed to be used, and is instead an exercise.

## Project Aims
- To develop a interpreter for an incredibly sparse language, containing only the most basic functionalities (in essence a turing machine a là Urban Müller's 1993 esolang 'Brainfuck').
- To do so with as compact an interpreter (as measured by the size of the compiled executable) as is possible.
- To learn and showcase interpreter design in a more intuitive, hands-on way.

## What this project is NOT aiming for
- Any degree of optimisation. Indeed, the only metric by which I'll measure success here is in bytes and not clock cycles.
- Readability: See above.
- A functional - or sensical - programming language: See above above.

## TODO:
☐ Interpreter roadmap (Quite important)

  ☐ Lexer implementation
  
  ☐ Parser implementation
  
  ☐ Interpreter implementation

☐ Language design (Less important)

  ☐ Find minimum possible space for each token to take up (likely one ASCII character to comply with most text editors).
  
  ☐ Design a language around the concept of no variable or lexeme exceeding one ASCII character - the lexer will have an easy job. 
