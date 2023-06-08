#!/bin/bash
#SBATCH -J quicksort
#SBATCH -o defresults/output_8k_%j.txt
#SBATCH -t 02:30
#SBATCH -n 16
#SBATCH -p allgroups
#SBATCH --mem 8G

mpicc utils.c quicksort.c main.c -o run
mpirun -np 1 ./run datasets/8k.txt
mpirun -np 2 ./run datasets/8k.txt
mpirun -np 4 ./run datasets/8k.txt
mpirun -np 8 ./run datasets/8k.txt
mpirun -np 16 ./run datasets/8k.txt

#mpirun -np 1 ./run datasets/100k.txt
#mpirun -np 2 ./run datasets/100k.txt
#mpirun -np 4 ./run datasets/100k.txt
#mpirun -np 8 ./run datasets/100k.txt
#mpirun -np 16 ./run datasets/100k.txt

#mpirun -np 1 ./run datasets/32k.txt
#mpirun -np 2 ./run datasets/32k.txt
#mpirun -np 4 ./run datasets/32k.txt
#mpirun -np 8 ./run datasets/32k.txt
#mpirun -np 16 ./run datasets/32k.txt
