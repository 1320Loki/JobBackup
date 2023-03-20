import multiprocessing as mp
import paho.mqtt.client as mqtt
import tkinter as tk

# Crear procesos para enviar y recibir mensajes MQTT
def enviar_mensaje():
    # Conectar a un broker MQTT
    client = mqtt.Client()
    client.connect("192.168.1.200", 1883, 60)

    # Función para enviar mensaje MQTT
    def enviar():
        message = entry.get() # Obtener mensaje del Entry
        client.publish("chatroom", message) # Publicar mensaje en el tema "chatroom"
        entry.delete(0, tk.END) # Limpiar el Entry después de enviar el mensaje

    # Crear interfaz de usuario para enviar mensajes MQTT
    window = tk.Tk()
    window.title("Enviar Mensajes MQTT")

    label = tk.Label(window, text="Mensaje:")
    label.pack()

    entry = tk.Entry(window)
    entry.pack()

    button = tk.Button(window, text="Enviar", command=enviar)
    button.pack()

    window.mainloop()

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



""" 

este ejemplo de multitasking

"""