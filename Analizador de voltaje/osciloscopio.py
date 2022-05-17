import socket
import threading
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

gData=[]
gData.append([0])
gData.append([0])

fig = plt.figure()
ax = fig.add_subplot(111)
hl, = plt.plot(gData[0], gData[1])
plt.ylim(-10, 150)
plt.xlim(0,200)

def GetData(out_data):
    i=0
    j=0
    k=0
    z=0.0  
    s = socket.socket()         
    s.bind(('0.0.0.0', 8090 ))
    s.listen(0)                 
    
    while True:
        client_socket, addr = s.accept()
        while True:
            content = client_socket.recv(32)
            if len(content) == 0:
                break
    
            else:
                medida=str(content, "ascii")
                if i > 200:
                    z=float(i)/200
                    if (z.is_integer()):
                        j=int(z)
                        out_data[0]=[]
                        out_data[1]=[]

                    k=i-(200*j)
                    out_data[0].append(k)
                    out_data[1].append(float(medida))

                else:
                    out_data[0].append(i)
                    out_data[1].append(float(medida))
                i=i+1
                

        client_socket.close()

def update_line(num, hl, data):
    hl.set_data(data[0], data[1])
    return hl,

line_ani = animation.FuncAnimation(fig, update_line, fargs=(hl, gData),
    interval=50, blit=False)

dataCollector = threading.Thread(target = GetData, args=(gData,))
dataCollector.start()

plt.show()

dataCollector.join()