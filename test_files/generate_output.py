# @Author: Izhar Shaikh
# @Date:   2017-02-15T20:05:45-05:00
# @Email:  izharits@gmail.com
# @Filename: test_output.py
# @Last modified by:   Izhar Shaikh
# @Last modified time: 2017-02-15T21:27:55-05:00

import sys

fileToOpen = open(sys.argv[1], "rb")
output = open(str(sys.argv[1] + "_output" ), "wb")

line = ' '
accountList = dict()
fromAccount = 0
toAccount = 0
transferAmount = 0

while True:
    line = fileToOpen.readline()
    if line == '':
        break
    if line[0] != 'T':
        account, balance = (int(i) for i in line.split())
        accountList[account] = balance
        #print account, balance
    else:
        #print line
        _, fr, to, tr = line.split()
        fromAccount, toAccount, transferAmount = int(fr), int(to), int(tr)
        #print fromAccount, toAccount, transferAmount
        accountList[fromAccount] -= transferAmount
        accountList[toAccount] += transferAmount

#print accountList

outputString = ""

for k, v in accountList.items():
    outputString += (str(k) + " " + str(v) + "\n")

output.write(outputString)

fileToOpen.close()
output.close()
