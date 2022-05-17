from asyncio.windows_events import NULL
import serial, time
import mysql.connector

arduino = serial.Serial('COM7', 9600)
#IDmaster ="680090E7C5DA"
#IDnormal ="6600C355E818" 

while True:
    mydb = mysql.connector.connect (host="127.0.0.1",user="root",password="Amnv2010",database="rfid")

    codigo=0
    time.sleep(0.05)
    rawString = arduino.readline()
    raw_string_s = rawString.decode('utf-8').strip()
    print(raw_string_s)

    mycursor = mydb.cursor()
    mycursor.execute("select numero from rfid where idTarjeta = '"+raw_string_s+"'")
    myresult = mycursor.fetchall()
    for x in myresult:
        if myresult==NULL:
            codigo=3
        else:
            codigo=x[0]

    if codigo=="1" :
        arduino.write(b'1\n')
        #print("prueba_PY_1")

    
    elif codigo=="2":
        arduino.write(b'2\n')
        #print("prueba_PY_2")

    elif codigo=="3":
        arduino.write(b'3\n')
        #print("prueba_PY_3")
        #arduino.close()

    rawString = arduino.reset_input_buffer()
    mycursor.close()
    mydb.close()