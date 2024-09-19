# LL-1-_parse_table
C code to compute LL(1)_parse_table
# Instructions to run

## Example 1
enter the productions in the form
Enter the number of non-terminals: 3
Enter non-terminal 1: S
Enter production for S: aABb
Enter non-terminal 2: A
Enter production for A: c|#
Enter non-terminal 3: B
Enter production for B: d|#

First sets:
First(S) = { a }
First(A) = { c, # }
First(B) = { d, # }

Follow sets:
Follow(S) = { $ }
Follow(A) = { d, b }
Follow(B) = { b }

LL(1) Parsing Table:
                   a         $         c         d         b
         S      aABb                                        
         A                             c         #         #
         B                                       d         #

## Example 2
Enter the number of non-terminals: 5
Enter non-terminal 1: S
Enter production for S: AB|eDa
Enter non-terminal 2: A
Enter production for A: ab|c
Enter non-terminal 3: B
Enter production for B: dC
Enter non-terminal 4: C
Enter production for C: eC|#
Enter non-terminal 5: D
Enter production for D: fD|#

First sets:
First(S) = { a, c, e }
First(A) = { a, c }
First(B) = { d }
First(C) = { e, # }
First(D) = { f, # }

Follow sets:
Follow(S) = { $ }
Follow(A) = { d }
Follow(B) = { $ }
Follow(C) = { $ }
Follow(D) = { a }

LL(1) Parsing Table:
                   a         c         e         $         d         f
         S        AB        AB       eDa                              
         A        ab         c                                        
         B                                                dC          
         C                            eC         #                    
         D         #                                                fD
