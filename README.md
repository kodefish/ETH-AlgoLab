# Algorithms Lab 2018
This repo contains my solutions to the algorithms lab of ETH Zürich. The lab consists of problems every week that must be solved via an algorithm. 

The primary language to solve the challenges is C++. I strive as much as possible to document my code and my algorithms. The description of the problems may be wonky, so I suggest that you give the associated PDF a read for a detailed explanation of the problem.

## Compilation and Running
To compile the cpp files:
```bash
g++ -Wall -O3 filename.cpp -o filename.o
```

To run the file with a given input and redirect output to a file
```bash
./filename.o < testsets/test{i}.in > test{i}.out
```

To compare with the expected output, use `diff`
```bash
diff test{i}.out testsets/test{i}.out
```

Alternatively, you can do the previous in one line
```bash
diff <(./filename.o < testsets/test{i}.in) testsets/test{i}.out
```
