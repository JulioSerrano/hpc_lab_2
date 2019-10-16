import os
import numpy as np
from matplotlib import pylab as plt

cmd = 'time -f "%E" -o file.txt ./bin/mandelbrot -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw'

def getSequentialTime():
    so = os.popen(cmd).read()
    f= open("file.txt","r")
    return float(f.readline().split(':')[1])


def getParalellTime(n):
    i = 1
    timeList = []
    cmd = 'time -f "%E" -o file.txt ./bin/mandelbrotp -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw -t '
    while i <= n:
        so = os.popen(cmd + str(i)).readline()
        f= open("file.txt","r")
        data = float(f.readline().split(':')[1])
        print(data)
        timeList.append(data)
        i += 1
    f.close()
    return timeList

def divideArray(timeList, sequentialTime):
    newList = []
    for i in timeList:
        newList.append(sequentialTime/i)
    return newList


sequentialTime = getSequentialTime()
pararellTime = getParalellTime(24)

pararellTime = divideArray(pararellTime, sequentialTime)
threads = np.arange(1,25)

figure = plt.figure()
plt.plot(threads, pararellTime)

plt.xlabel('Número de hebras')
plt.ylabel('S(n)')

figure.savefig('figure2.jpg')
exit();