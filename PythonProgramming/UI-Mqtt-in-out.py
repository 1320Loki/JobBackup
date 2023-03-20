# LIBRERIAS UTILIZADAS
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import tkinter as tk
import json

# MQTT CONNECTION BROKER IP ADDRESS
broker_address = "192.168.1.200"
port = 1883

# PUBLISH MULTIPLE MQTT MESSAGES
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

