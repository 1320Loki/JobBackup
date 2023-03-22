import time
import paho.mqtt.client as mqtt

def enviarMQTT(Topic, Msg, Host="192.168.1.200", Port=1883):
    client = mqtt.Client()
    client.connect(Host, Port, 60)
    client.publish(Topic, Msg)
    


inicio = time.time()
#enviarMQTT("Topic", "msg")
enviarMQTT("ALSW/demo", "hola")
final = time.time()
tiempo = (final - inicio) * 1000
print(f"El código tardo {tiempo} ms")