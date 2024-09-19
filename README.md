# LL-1-_parse_table
C code to compute LL(1)_parse_table
# Instructions to run

## Example 1
enter the productions in the form<br/>
Enter the number of non-terminals: 3<br/>
Enter non-terminal 1: S<br/>
Enter production for S: aABb<br/>
Enter non-terminal 2: A<br/>
Enter production for A: c|#<br/>
Enter non-terminal 3: B<br/>
Enter production for B: d|#<br/><br/>

First sets:<br/>
First(S) = { a }<br/>
First(A) = { c, # }<br/>
First(B) = { d, # }<br/>
<br/>
Follow sets:<br/>
Follow(S) = { $ }<br/>
Follow(A) = { d, b }<br/>
Follow(B) = { b }<br/>

LL(1) Parsing Table:<br/>
|       | a     | $     | c     | d     | b     |
|-------|-------|-------|-------|-------|-------|
| **S** | aABb  |       |       |       |       |
| **A** |       |       | c     | #     | #     |
| **B** |       |       |       | d     | #     |

## Example 2<br/>
Enter the number of non-terminals: 5<br/>
Enter non-terminal 1: S<br/>
Enter production for S: AB|eDa<br/>
Enter non-terminal 2: A<br/>
Enter production for A: ab|c<br/>
Enter non-terminal 3: B<br/>
Enter production for B: dC<br/>
Enter non-terminal 4: C<br/>
Enter production for C: eC|#<br/>
Enter non-terminal 5: D<br/>
Enter production for D: fD|#<br/>
<br/>
First sets:<br/>
First(S) = { a, c, e }<br/>
First(A) = { a, c }<br/>
First(B) = { d }<br/>
First(C) = { e, # }<br/>
First(D) = { f, # }<br/>
<br/>
Follow sets:<br/>
Follow(S) = { $ }<br/>
Follow(A) = { d }<br/>
Follow(B) = { $ }<br/>
Follow(C) = { $ }<br/>
Follow(D) = { a }<br/>
<br/>
LL(1) Parsing Table:<br/>
|       | a     | c     | e     | $     | d     | f     |
|-------|-------|-------|-------|-------|-------|-------|
| **S** | AB    | AB    | eDa   |       |       |       |
| **A** | ab    | c     |       |       |       |       |
| **B** |       |       |       |       | dC    |       |
| **C** |       |       | eC    | #     |       |       |
| **D** | #     |       |       |       |       | fD    |

