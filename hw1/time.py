import os, sys

data = {'elapsed':[], 'user':[], 'system':[]}

for i in range(10):
    os.system('(time ./bilateralfilter < images/cat.pgm) 2> time.txt')
    text = open('time.txt').read().split()

    data[text[0][4:]].append(text[0][:4])
    data[text[1][4:]].append(text[1][:4])
    data[text[2][7:]].append(text[2][3:7])


try:
    text = open(sys.argv[1],'w')
    for i in data:
        text.write(i)
        text.write('\n')
        for j in data[i]:
            text.write(j)
            text.write(', ')
        text.write('\n')
    text.close()
except IndexError:
    print 'supply a name for a file to save the data to.'
    pass

