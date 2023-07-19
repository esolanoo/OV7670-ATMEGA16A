import serial
import numpy as np
import tkinter as tk
from PIL import Image, ImageTk


def show():
    if not ser.is_open:
        ser.baudrate = 115200
        ser.port = 'COM5'
        ser.open()
    for i in range(120):
        for j in range(160):
            h = int.from_bytes(ser.read(), "big")
            l = int.from_bytes(ser.read(), "big")
            b = (h << 8) | l
            arr[i, j] = [np.uint8(((((b >> 11) & 0x1F) * 527) + 23) >> 6),
                         np.uint8(((((b >> 5) & 0x3F) * 259) + 33) >> 6),
                         np.uint8((((b & 0x1F) * 527) + 23) >> 6)]
    im = Image.fromarray(arr, mode="RGB")
    display = ImageTk.PhotoImage(im)
    label1.configure(image=display)
    label1.image = display
    im.save('save.jpg')
    ser.close()


root = tk.Tk()  # The whole structure to which we will attach the functions... the empty frame
arr = np.zeros((120, 160, 3), dtype=np.uint8)
ser = serial.Serial()

height = 150
width = 300
windowXpos = int(
    root.winfo_screenwidth() / 2 -150)  # screen width center
windowYpos = int(
    root.winfo_screenheight() / 3)  # screen height center
root.geometry("+{}+{}".format(windowXpos, windowYpos))  # center root

canvas = tk.Canvas(root, height=height, width=width, bg="#263D42")  # dimensions and color of the empty canvas
canvas.pack()

frame = tk.Frame(root, bg="white")
frame.place(relwidth=0.8, relheight=0.5, relx=0.1, rely=0.1)  # frame with dimensions and positions (relative to canvas)

begin = tk.Button(root, text="Take Photo", padx=10, pady=5, fg="white", bg="#263D42",  command=show)
begin.pack(fill=tk.X)
end = tk.Button(root, text="Exit", padx=10, pady=5, fg="white", bg="#263D42", command=root.destroy)
end.pack(fill=tk.X)

img_init = ImageTk.PhotoImage(image=Image.open('up.png'))
label1 = tk.Label(frame, image=img_init)
label1.pack()

root.mainloop()
