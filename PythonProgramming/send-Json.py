import time
import json
import paho.mqtt.publish as publish

# Configurar los parámetros de conexión
broker_address = "192.168.1.200"
port = 1883

# Definir los mensajes a enviar como objetos JSON
messages = [
    {"topic": "test/topic1", "payload": json.dumps({"id": 1, "message": "mensaje 1"}), "qos": 1},
    {"topic": "test/topic2", "payload": json.dumps({"id": 2, "message": "mensaje 2"}), "qos": 0},
    {"topic": "test/topic3", "payload": json.dumps({"id": 3, "message": "mensaje 3"}), "qos": 2},
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
