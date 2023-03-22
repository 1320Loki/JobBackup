# LIBRERIAS UTILIZADAS
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import tkinter as tk
import json

# MQTT CONNECTION BROKER IP ADDRESS
broker_address = "192.168.1.200"
port = 1883

# CREATION OF MQTT MESAGGES
cookingSet = {
    "Temperature": 100, 
    "Speed": 1, 
    "Time": 5
}

spicesSet = {
    "Position": 2, 
    "Side": 1, 
}

cartSet = {
    "Position": 2, 
    "Side": 1, 
}

# Convertir el objeto JSON a una cadena de texto
JsonCooking = json.dumps(cookingSet)
JsonSpices = json.dumps(spicesSet)
JsonCart = json.dumps(cartSet)

# Definicion de coneccion y topicos
def on_connect(client, userdata, flags, rc):
    print("MQTT connected "+ str(rc))
    client.subscribe("Kitchen.1/Cooking.Station.1")
    client.subscribe("Kitchen.1/Spices.1")
    client.subscribe("Kitchen.1/Cart.1")

# Definicion de mensajes recibidos mqtt
def on_message(client, userdata, msg):
    print(msg.topic + "" + str(msg.payload))

# Set cooking
def funcion1():     
    print("Mqtt cooking msg")

    publish.single(
        "Kitchen.1/Cooking.Station.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

# Set spices
def funcion2():     
    print("Mqtt spices msg")
    
    publish.single(
        "Kitchen.1/Spices.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

# Set cart
def funcion3():     
    print("Mqtt cart msg")

    publish.single(
        "Kitchen.1/Cart.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

# Crear ventana
ui = tk.Tk()

# Configurar ventana
ui.geometry("300x200")
ui.title("Grafical interface")

# Crear botones
bt1 = tk.Button(ui, text="Cooking pot", command=funcion1)
bt2 = tk.Button(ui, text="Spices", command=funcion2)
bt3 = tk.Button(ui, text="Cart", command=funcion3)

# Posicionar botones
bt1.pack( padx=10, pady=10)
bt2.pack( padx=10, pady=10)
bt3.pack( padx=10, pady=10)



# PARTE DE RECIBIR MENSAJES
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.200", 1883, 60)
#client.loop_forever()


# Iniciar loop de eventos
ui.mainloop()
