# LIBRERIAS UTILIZADAS
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import multiprocessing as mp
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

# Functions for the buttons
def funcion1():     # Set cooking
    print("Mqtt cooking msg")

    publish.single(
        "Kitchen.1/Cooking.Station.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

def funcion2():     # Set spices
    print("Mqtt spices msg")
    
    publish.single(
        "Kitchen.1/Spices.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

def funcion3():     # Set cart
    print("Mqtt cart msg")

    publish.single(
        "Kitchen.1/Cart.1/Set",
        payload=JsonCooking ,
        qos=1,
        hostname=broker_address,
        port=port,
    )

# Crear procesos para enviar y recibir mensajes MQTT
def enviar_mensaje():
    # Conectar a un broker MQTT
    client = mqtt.Client()
    client.connect("192.168.1.200", 1883, 60)

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

    # Iniciar loop de eventos
    ui.mainloop()

def recibir_mensaje():
    # Conectar a un broker MQTT
    client = mqtt.Client()

    # Función para procesar el mensaje recibido
    def procesar_mensaje(topic, message):
        print(topic, message) # Imprimir mensaje recibido

    # Configurar funciones de devolución de llamada
    client.on_connect = lambda client, userdata, flags, rc: client.subscribe("trial")
    client.on_message = lambda client, userdata, msg: procesar_mensaje(msg.topic, msg.payload.decode())

    # Conectar al broker MQTT y esperar por nuevos mensajes
    client.connect("192.168.1.200", 1883, 60)
    client.loop_forever()

if __name__ == "__main__":
    # Crear procesos
    p1 = mp.Process(target=enviar_mensaje)
    p2 = mp.Process(target=recibir_mensaje)

    # Iniciar procesos
    p1.start()
    p2.start()

    # Esperar a que los procesos finalicen (esto nunca se alcanzará debido a los bucles infinitos en los procesos)
    p1.join()
    p2.join()