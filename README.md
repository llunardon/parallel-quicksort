Repository dedicated to the project for the Parallel Computing course at the University of Padua, taken by Gianfranco Bilardi
and Paolo Emilio Mazzon.

To compile and run locally, you'll need to generate a dataset using the Python script.

```
python3 generate_randoms.py
```

After, compile and run using:
```
mpicc utils.c quicksort.c main.c -o run

mpirun -np #cpus ./run dataset.txt
```
where #cpus is the number of cpus you want to use.
