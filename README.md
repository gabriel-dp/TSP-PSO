# Travelling Salesman Problem using Particle Swarm Optimization

&nbsp;

```c
#define SUBJECT "Inteligência Artificial"
#define WORK "Trabalho Prático 1"
#define PROFESSOR "Edimilson Batista"
#define STUDENTS [
                    "Davi dos Reis", 
                    "Gabriel de Paula", 
                    "Guilherme Francis", 
                    "Wasterman Apolinário"
                 ]
```

[`> Click here to see the documentation<`](./docs/documentation-ptbr.pdf)

&nbsp;

## 🖥 Running the program

Run the makefile to generate the compilation objects and generate the final file `out`:

```bash
make
```

To run the program you need to provide the integer number N of cities, and then the x and y coordinates of all N cities:

```bash
./bin/out
```

```txt
5
1 5
4 6
7 5
5 4
9 4
```

---
Alternatively, you can run the program with an text file:

```bash
./bin/out < {inputPath}
```
