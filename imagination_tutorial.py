import tkinter as tk
import tkinter.ttk as ttk

from serial import Serial
from serial.tools.list_ports import comports

from tkinter.messagebox import showerror
from serial import Serial, SerialException

from threading import Thread, Lock # we'll use Lock later ;)
from tkinter import *

S_OK: int = 0xaa
S_ERR: int = 0xff
LED_SHIFT: int = 103
ROTATION_SHIFT: int = 101

def detached_callback(f):
    return lambda *args, **kwargs: Thread(target=f, args=args, kwargs=kwargs).start()

class LockedSerial(Serial): #child class of serial locked up in this thread, it cannot escape to a different thread, are we just not allowed to run any other threads? I thought we need this connection to com while other threads are running but don't want to 
    _lock: Lock = Lock()
    
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        
    def read(self, size=1) -> bytes:
        with self._lock: #lock created here 
            return super().read(size) #lock deleted after this is executed
        
    def write(self, b: bytes, /) -> int | None:
        with self._lock:
            super().write(b)
            
    def close(self):
        with self._lock: #
            super().close()

class App(tk.Tk):
    ser: LockedSerial

    def __init__(self):
        super().__init__()
    
        self.title("LED Blinker")
        
        self.port = tk.StringVar()  
        self.led = tk.BooleanVar()
        self.rotation = tk.BooleanVar()
        self.motor = tk.IntVar()   #adding this code so we can access motor by using .get

        ttk.Checkbutton(self, text='Toggle rotation', variable=self.rotation, command=self.update_rotation).pack()
        ttk.Checkbutton(self, text='Toggle LED', variable=self.led, command=self.update_led).pack()  #defining what each of these are doing
        ttk.Button(self, text='Send Invalid', command=self.send_invalid).pack()
        ttk.Button(self, text='Disconnect', default='active', command=self.disconnect).pack()
        # ttk.Button(self, text="Read Arduino", command=self.read_arduino).pack()
        ttk.Scale(self,from_=0,to=100,orient=HORIZONTAL,variable=self.motor,command=self.update_motor).pack()

        SerialPortal(self) # and this
        
    def write(self, b: bytes):
        try:
            self.ser.write(b)
            if int.from_bytes(self.ser.read(), 'big') == S_ERR:
                showerror('Device Error', 'The device reported an invalid command.')
        except SerialException:
            showerror('Serial Error', 'Write failed.')

    @detached_callback
    def connect(self):
        self.ser = LockedSerial(self.port.get()) #changed this to locked serial

    # @detached_callback
    # def read_arduino(self):
    #     print(self.ser.read()) #somehow putting in the python serial?

    @detached_callback
    def update_led(self):
        self.write(bytes([self.led.get() + LED_SHIFT])) #adding 103 so we can know to turn on the led 

    @detached_callback
    def update_rotation(self):
        self.write(bytes([self.rotation.get() + ROTATION_SHIFT])) #self.rotation.get() is going to be a 0 or 1 depending on toggle from button we defined

    @detached_callback
    def update_motor(self, value):
        self.write(bytes([self.motor.get()]))
        #print(int(value))
        print(self.motor.get())

    @detached_callback
    def disconnect(self):
        self.ser.close()
        SerialPortal(self) # display portal to reconnect

    @detached_callback
    def send_invalid(self):
        self.write(bytes([0x10]))
    
    def __enter__(self):
        return self
    
    def __exit__(self, *_):
         self.disconnect()

class SerialPortal(tk.Toplevel):  #does this need to be locked??? no because it is not serial type exactly
    def __init__(self, parent: App):
        super().__init__(parent)
        
        self.parent = parent
        self.parent.withdraw() # hide App until connected
        
        ttk.OptionMenu(self, parent.port, '', *[d.device for d in comports()]).pack()
        ttk.Button(self, text='Connect', command=self.connect, default='active').pack()
        
    def connect(self):
        self.parent.connect()
        self.destroy()
        self.parent.deiconify() # reveal App
        
    
if __name__ == '__main__':
    with App() as app: #we think this is saying app = App() is this correct Adin???
        app.mainloop()


# 

# with Serial('COM6', 9600) as ser:
#     ser.write(bytes([0x1]))
#     print(ser.read() == bytes([0xaa]))

#     ser.write(bytes([0x0]))
#     print(ser.read() == bytes([0xaa]))

#     ser.write(bytes([0x2]))
#     print(ser.read() == bytes([0xff]))

    # ser.write(bytes([0x1])) #send message to turn on LED
    # #time.sleep(1)
    # ser.write(bytes([0x0]))
    # while True:
    #     print(ser.readline().decode())
    #     