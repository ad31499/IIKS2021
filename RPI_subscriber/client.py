# python3.6

import random
import pymongo
import json
from datetime import datetime
from paho.mqtt import client as mqtt_client

#Inicializiraj mongoDB
clientDB = pymongo.MongoClient("mongodb+srv://anzedolenc:iiks2021@cluster0.6svfx.mongodb.net/Cluster0?retryWrites=true&w=majority")
mydb = clientDB["tempDB"] #Ustvari bazo z imenom tempDB" 
mytemp = mydb["temperatura"] #Ustvari kolekcijo temperatura (Enako kot tabela pri sql)

#Inicializiraj MQTT klienta in posrednika
broker = '192.168.1.160'
port = 1883
topic = "senzor/temp"
client_id = 'anzerpi'

#Poveži se na posrednika
def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Povezan na MQTT posrednik!")
        else:
            print("Povezava ni uspela! %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

#Naroči se na topic senzor/temp
def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg): #Ko dobim sporočilo od brokerja
        #Dobi čas
        today = datetime.now()
        dataTODB = {}
        dataTODB['day'] = today.day
        dataTODB['month'] = today.month
        dataTODB['year'] = today.year
        dataTODB['hour'] = today.hour
        dataTODB['minutes'] = today.minute
        dataTODB['seconds'] = today.second
        #V bazo pošlji vrednost temperature in čas, ko je bilo sporočilo poslano 
        data = msg.payload.decode()
        dataTODB['temperatura'] = float(data)
        mytemp.insert_one(dataTODB)
        print(dataTODB) #Izpiši v terminalu
            
    client.subscribe(topic)
    client.on_message = on_message

#Zaženi
def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

#Loop
if __name__ == '__main__':
    run()
