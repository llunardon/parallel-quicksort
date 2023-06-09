import random

#r = (int(input('how many to generate?: ')))
r = 100000000
#randfile = open(f"{r}.txt", "w" )
randfile = open("tenmill.txt", "w" )

for i in range(r):
    if i == 0:
        line = str(r) + ' '
        randfile.write(line)
    if i < r - 1:
        line = str(random.randint(1, 100000)) + ' '
        randfile.write(line)
    else:
        line = str(random.randint(1, 100000))
        randfile.write(line)

randfile.close()
