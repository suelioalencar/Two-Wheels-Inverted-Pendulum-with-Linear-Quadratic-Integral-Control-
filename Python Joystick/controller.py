import tkinter as tk
from tkinter.ttk import *
from tkinter import messagebox
import requests
import time

url = 'http://192.168.100.93' #Endereço IP do ESP32

class Controle_wifi(tk.Frame):
    def __init__(self, *args, **kwargs):
        tk.Frame.__init__(self, *args,**kwargs)
        
        self.upA = tk.PhotoImage(file = r"up2.png")

        self.upB = tk.Button(self,image = self.upA)
        
        self.upB.grid(column = 1, row = 0,rowspan = 2)

        self.upB.bind("<ButtonPress>",self.up_press)
        self.upB.bind("<ButtonRelease>",self.released_button)

        self.leftA = tk.PhotoImage(file = r"left2.png")

        self.leftB = tk.Button(self,image = self.leftA)
        self.leftB.grid(column = 0, row = 2)
        
        self.leftB.bind("<ButtonPress>",self.left_press)
        self.leftB.bind("<ButtonRelease>",self.released_button)

        self.rightA = tk.PhotoImage(file = r"righto2.png")

        self.rightoB = tk.Button(self,image = self.rightA)
        self.rightoB.grid(column = 2, row = 2)

        self.rightoB.bind("<ButtonPress>",self.right_press)
        self.rightoB.bind("<ButtonRelease>",self.released_button)

        self.downA = tk.PhotoImage(file = r"down2.png")

        self.downB = tk.Button(self,image = self.downA)
        self.downB.grid(column = 1, row = 3)

        self.downB.bind("<ButtonPress>",self.down_press)
        self.downB.bind("<ButtonRelease>",self.released_button)

        self.labelX = Label(self, text = 'Referencia em X(cm):')
        self.labelX.grid(column = 4, row = 0)
        
        self.movX = Entry(self, validate = 'key')
        self.movX.grid(column = 5, row = 0)

        self.enviarX = tk.Button(self,text = 'Enviar posição', 
                                 command = self.enviar_posicao)
        self.enviarX.grid(column = 6, row = 0)
        
        self.labelZ = Label(self, text = 'Referencia em Z(º):')
        self.labelZ.grid(column = 4, row = 1)
        
        self.movZ = Entry(self, validate = 'key')
        self.movZ.grid(column = 5, row = 1)

        self.enviarZ = tk.Button(self,text = 'Enviar angulo',
                                 command = self.enviar_angulo)
        self.enviarZ.grid(column = 6, row = 1)
        
    def up_press(self, event):
        payloadx = {'fader1':1}
        res = requests.get(url, params=payloadx)

    def left_press(self, event):
        payloadz = {'fader2':1}
        res = requests.get(url, params=payloadz)

    def right_press(self, event):
        payloadz = {'fader2':-1}
        res = requests.get(url, params=payloadz)

    def down_press(self, event):
        payloadx = {'fader1':-1}
        res = requests.get(url, params=payloadx)

    def released_button(self, event):
        payloadz = {'fader2':'stopz'}
        res = requests.get(url, params=payloadz)
        payloadx = {'fader1':'stopx'}
        res2 = requests.get(url, params=payloadx)

    def enviar_posicao(self):
        try:
            int(self.movX.get())
        except:
            messagebox.showerror('Erro de tipo',
            'Os valores do campo de Referencia em X devem ser inteiros!')
            self.movX.delete(0, 'end')
            return
        payloadLin = {'fader3':int(self.movX.get())}
        res2 = requests.get(url, params=payloadLin)

    def enviar_angulo(self):
        try:
            int(self.movZ.get())
        except:
            messagebox.showerror('Erro de tipo'
            ,'Os valores do campo de Referencia em Z devem ser inteiros!')
            self.movZ.delete(0, 'end')
            return
        payloadLin = {'fader4':int(self.movZ.get())}
        res2 = requests.get(url, params=payloadLin)
             
   
if __name__ == "__main__":
    root = tk.Tk()
    Controle_wifi(root).grid()#.pack(side="top", fill="both", expand=True)
    root.mainloop()
   

