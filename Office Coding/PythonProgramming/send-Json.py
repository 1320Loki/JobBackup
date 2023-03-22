# LIBRERIAS UTILIZADAS
import time
import json
import paho.mqtt.publish as publish

# CODE ESSENCIALS
count = 0

# MQTT CONNECTION BROKER IP ADDRESS
broker_address = "192.168.1.200"
port = 1883
#username = "user"
#password = "password"

# PUBLISH MULTIPLE MQTT MESSAGES
cookingSet = {
    "Temperature": 100, 
    "Speed": 1, 
    "Time": 5
}

cartSet = {
    "Position": 2, 
    "Side": 1, 
}

messages = [
    {"topic": "test/topic1", "payload": json.dumps({"id": 1, "message": "mensaje 1"}), "qos": 1},
    {"topic": "test/topic2", "payload": json.dumps({"id": 2, "message": "mensaje 2"}), "qos": 0},
    {"topic": "test/topic3", "payload": json.dumps({"id": 3, "message": "mensaje 3"}), "qos": 2},
]

# Convertir el objeto JSON a una cadena de texto
JsonCooking = json.dumps(cookingSet)
JsonCart = json.dumps(cartSet)

##       CODE LOOP       ##

while True:

    send = input("Press (cook) to send msg to cooking station and (cart) to send msg to cart. (m) For multiple msgs     ")



    if send == "cook":        
        # Publicar mensajes mqtt 
        publish.single(
            "Kitchen.1/Cooking.Station.1/Set",
            payload=JsonCooking ,
            qos=1,
            hostname=broker_address,
            port=port,
        )

    elif send == "cart":
        # Publicar el mensaje en el topic "sensors/1"
        publish.single(
            "Kitchen.1/Cart.1/Set",
            payload=JsonCart ,
            qos=1,
            hostname=broker_address,
            port=port,
        )

    elif send == "m":
        publish.multiple(
        messages,
        hostname=broker_address,
        port=port
    )
    
    else: print("Error")

    count += 1
    print("iteration: ", count)
