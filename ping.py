import os
hostname = []
for i in range(1,10):
    hostname.append("sn336s0"+str(i)+".sci")
for i in range(10,36):
    hostname.append("sn336s"+str(i)+".sci")
response = {}
for i in range(len(hostname)):
    response[i] = os.system('ping -c 1 -q ' + hostname[i])

text = open("hosts", "w")
for i in range(len(hostname)):
    if response[i] == 0:
        print(hostname[i], 'success!')
        text.write(hostname[i]+'\n')
    else:
        print(hostname[i], 'fail :(')
text.close()
