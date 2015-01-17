import os

os.system('(time ./bilateralfilter < images/cat.pgm) 2> time.txt')
text = open('time.txt').read().split()
data = {text[0][4:]:text[0][:4],text[1][4:]:text[1][:4],text[2][7:]:text[2][2:7]}
for i in data:
    print i, data[i]

