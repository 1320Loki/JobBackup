import json
import time
import paho.mqtt.publish as publish

# Configurar los parámetros de conexión
broker_address = "192.168.1.200"
port = 1883
#username = "user"
#password = "password"


# Definir los mensajes a enviar
message1 = {
    "temperature": 25,
    "humidity": 60,
    "pressure": 1013
}

messages = [
    {"topic": "test/topic1", "payload": "mensaje 1", "qos": 1},
    {"topic": "test/topic2", "payload": "mensaje 2", "qos": 0},
    {"topic": "test/topic3", "payload": "mensaje 3", "qos": 2},
]



# Enviar los mensajes continuamente
while True:
    publish.multiple(
        messages,
        hostname=broker_address,
        port=port
    )

    print("ola")

    time.sleep(5) # Esperar 5 segundos antes de enviar los mensajes nuevamente
