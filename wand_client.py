import socket
import pyautogui
import time
import os
import threading

rhost = '192.168.4.1'
#rhost = '127.0.0.1'
rport = 5555

# def acking():
#     while True:
#         s.send('ack'.encode())
#         print('system: ack sended')
#         time.sleep(10)

custom_command = input('Insert custom command (swipe the wand down twice to trigger): \n>> ')
custom_command2 = input('Insert custom command (swipe the wand down tree times to trigger): \n>> ')
mode = "I"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((rhost, rport))
s.send(mode.encode())




def custom(command):
    print(f'executing: {command}')
    os.system(command)

def switch_screen(direction):
    pyautogui.keyUp('ctrl')
    pyautogui.keyDown('ctrl')
    pyautogui.press(direction)
    pyautogui.keyUp('ctrl')



print('connected')

while True:
    msg = ''
    msg = s.recv(1024).strip().decode()
    print(msg)

    if msg == 'yah':
        switch_screen('right')

    elif msg == 'space':
        pyautogui.press('space')

    elif msg == "nope":
        switch_screen('left')


    elif msg == "back":
        pyautogui.press('left')
    elif msg == "custom":
        t = threading.Thread(target = custom, args = (custom_command, ))
        t.start()
    elif msg == "custom2":
        t = threading.Thread(target = custom, args=(custom_command2, ))
        t.start()

s.close()
