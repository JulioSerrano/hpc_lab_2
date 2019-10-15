import os
import numpy as np
from matplotlib import pylab as plt

cmd = 'time -f "%E" -o file.txt ./test -i 500 -a -0.748766713922161 -b 0.123640844894862 -c -0.748766707771757 -d 0.123640851045266 -s 1e-11 -f salida.raw -t 1'

def getSequentialTime():
    so = os.popen(cmd).read()
    f= open("file.txt","r")
    return float(f.readline().split(':')[1])


def getParalellTime(n):
    i = 1
    timeList = []
    cmd = 'time -f "%E" -o file.txt ./test -i 500 -a -0.748766713922161 -b 0.123640844894862 -c -0.748766707771757 -d 0.123640851045266 -s 1e-11 -f salida.raw -t '
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

figure.savefig('figure.jpg')
exit();