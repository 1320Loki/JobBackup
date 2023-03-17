import tkinter as tk

# Crear una instancia de la ventana principal
window = tk.Tk()

# Configurar el título de la ventana
window.title("Mi aplicación")

# Configurar el tamaño de la ventana
window.geometry("300x200")

# Crear una etiqueta
label = tk.Label(window, text="Hola, bienvenido a mi aplicación!")
label.pack(pady=20)

# Crear un botón
button = tk.Button(window, text="Haz clic aquí!")
button.pack()

# Ejecutar el loop principal de la ventana
window.mainloop()