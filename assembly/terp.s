.bss
.align 3
tape: .zero 256
programbuffer: .zero 256

.text
.global _main
.align 2


_main:
    ldr x0, [X1, #8]                ; read argv
    mov x1, #0                      ; read-only
    mov x16, #5                     ; read syscall
    svc #0x80

    adrp x1, programbuffer@PAGE
    add x1, x1, programbuffer@PAGEOFF

    mov x2, #256                  ; max size

    mov x16, #3
    svc #0x80

                                    ; interpreter setup and stuff, overall design:  X19 address of the program, X20 address of the tape, X21 program counter,
                                    ; X22 total length, X23 bit index, X0-X9 is whatever
    
    mov X22, X0                     ; will be used to compare to program counter, if pc >= total bytes, end
    
    adrp X19, programbuffer@PAGE
    add  X19, X19, programbuffer@PAGEOFF
    adrp X20, tape@PAGE
    add  X20, X20, tape@PAGEOFF
    

    mov X21, #0                     ; program counter
    mov X23, #0                     ; bit index (x21 shows the byte, x23 shows bit in byte

loop:
    cmp x21, x22
    b.ge end

    ldrb W0, [X19, X21]             ; load whatever byte were at

                                    ; essentially the lexer portion of this interpreter
    cmp W0, #'!'    
    b.eq flip
    cmp W0, #'<'
    b.eq move_left
    cmp W0, #'>'
    b.eq move_right
    cmp W0, #'['
    b.eq left_brack
    cmp W0, #']'
    b.eq right_brack
    cmp W0, #'.'
    b.eq read                       ; normally, i'd put a default case at the end here, but i'm squeezing every line out of this
                                

move_right:
    add X23, X23, #1
    and X23, X23, #255            ; more fancy shmancy bit manip (deals with overflow logic, can't have a runaway pointer)
    b next                          ; this works because (256)_10=(11111111)_8; 0s above 8 bits
    
move_left:
    sub X23, X23, #1
    and X23, X23, #255
    b next
flip:
    lsr X3, X23, #3                 ; get the bytes index
    and X4, X23, #7                 ; what bit are we in said byte; look at this fancy shmancy bit manip
    eor X4, X4, #7 

    ldrb W5, [X20, X3]
    mov W6, #1

    lsl W6, W6, W4                  ; get bit in position
    eor W5, W5, W6                  ; xor it
    strb W5, [X20, X3]              ; pass it back
    b next

read:
    lsr X3, X23, #3                 ; byte index
    ldrb W5, [X20, X3]              ; w5 is now the whole byte

    sub SP, SP, #16                 ; carve out a lil memory
    strb W5, [SP]
    mov X0, #1                      ; write
    mov X1, SP                      ; this data
    mov X2, #1                      ; of size 1 byte
    mov X16, #4
    svc #0x80
    add SP, SP, #16                 ; memory safety hooray
    b next

; below is a lot of logic for searching for brackets; i'm doing this at compile time to save code but its currently very unoptimised. 
; i will clean this up but for now this is what we have to deal with

; in essence the logic is: see left bracket -> start to scan forwards -> if we see another left bracket, we know theres a right bracket 
; that might give us a false endpoint, so we ignore the next right bracket we see, if theres a right bracket, and we havent 
; flagged any necessary increases in nesting depth, then we read the bit, if its one, jump back to left bracket. vice versa for right brackets.

left_brack:
    lsr X3, X23, #3
    and X4, X23, #7
    eor X4, X4, #7 
    ldrb W5, [X20, X3]
    lsr W5, W5, W4
    and W5, W5, #1
    cmp W5, #0
    b.ne next                        ; boolfuck logic
 
    mov X6, #1                       ; nesting depth, necessary for situations like [x[x]]
look_forward:
    add X21, X21, #1
    cmp X21, X22
    b.ge end                         ; rare error handling lol
    ldrb W0, [X19, X21]
    cmp W0, #'['
    b.eq found_left_brack_fwd
    cmp W0, #']'
    b.eq found_right_brack_fwd
    b look_forward

found_left_brack_fwd:
    add X6, X6, #1
    b look_forward

found_right_brack_fwd:
    sub X6, X6, #1
    cmp X6, #0
    b.eq next                      ; weve found a match
    b look_forward

right_brack:
    lsr X3, X23, #3
    and X4, X23, #7
    eor X4, X4, #7 
    ldrb W5, [X20, X3]
    lsr W5, W5, W4
    and W5, W5, #1
    cmp W5, #1
    b.ne next                       ; more boolfuck logic
 
    mov X6, #1

look_backwards:
    sub X21, X21, #1
    cmp X21, #0
    b.lt end                        ; more rare error handling
    ldrb W0, [X19, X21]
    cmp W0, #']'
    b.eq found_right_brack_bckw
    cmp W0, #'['
    b.eq found_left_brack_bckw
    b look_backwards
found_right_brack_bckw:
    add X6, X6, #1
    b look_backwards
found_left_brack_bckw:
    sub X6, X6, #1
    cmp X6, #0
    b.eq next                     ; weve found a match part 2
    b look_backwards

end:
    mov x0, #0
    mov x16, #1
    svc #0x80

next:
    add x21, x21, #1
    b loop