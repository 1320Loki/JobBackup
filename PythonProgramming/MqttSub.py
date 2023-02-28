import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("MQTT connected "+ str(rc))
    client.subscribe("ALSW/#")

def on_message(client, userdata, msg):
    print(msg.topic + "" + str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.200", 1883, 60)
client.loop_forever()
