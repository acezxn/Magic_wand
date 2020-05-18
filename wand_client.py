import socket
import pyautogui
import time
import os
import threading

# Hostname and port of the TCP server
rhost = '192.168.4.1'
rport = 5555


custom_command = input('Insert custom command (swipe the wand down twice to trigger): \n>> ')
custom_command2 = input('Insert custom command (swipe the wand down tree times to trigger): \n>> ')
mode = "I"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((rhost, rport))
s.send(mode.encode())




def custom(command): # Execute custom command
    print(f'executing: {command}')
    os.system(command)

def switch_screen(direction): # Screen switch utility
    pyautogui.keyUp('ctrl')
    pyautogui.keyDown('ctrl')
    pyautogui.press(direction)
    pyautogui.keyUp('ctrl')



print('connected')

while True:
    msg = ''
    msg = s.recv(1024).strip().decode()
    print(msg)

    if msg == 'left':
        switch_screen('right')

    elif msg == 'up':
        pyautogui.press('space')

    elif msg == "right":
        switch_screen('left')


    elif msg == "down":
        pyautogui.press('left')
    elif msg == "custom":
        t = threading.Thread(target = custom, args = (custom_command, ))
        t.start()
    elif msg == "custom2":
        t = threading.Thread(target = custom, args=(custom_command2, ))
        t.start()

s.close()
