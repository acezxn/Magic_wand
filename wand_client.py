import socket
import pyautogui
import time
import os
import threading
import platform
# Hostname and port of the TCP server
rhost = '192.168.4.1'
rport = 5555


custom_command = input('\nInsert custom command (swipe the wand down twice to trigger): \n>> ')
custom_command2 = input('\nInsert custom command (swipe the wand down tree times to trigger): \n>> ')
mode = "I"
OS = platform.system()
print(f'Using Operating System: {OS}')
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((rhost, rport))
s.send(mode.encode())




def custom(command): # Execute custom command
    os.system(command)

def switch_screen(direction): # Screen switch utility
    if OS == 'Darwin': # OS X
        pyautogui.keyUp('ctrl')
        pyautogui.keyDown('ctrl')
        pyautogui.press(direction)
        pyautogui.keyUp('ctrl')
    elif OS == 'Windows': # Windows
        pyautogui.keyDown('win')
        pyautogui.keyDown('ctrl')
        pyautogui.press(direction)
        pyautogui.keyUp('ctrl')
        pyautogui.keyUp('win')
    elif OS == 'Linux': # Linux
        pyautogui.keyDown('ctrl')
        pyautogui.keyDown('alt')
        pyautogui.press(direction)
        pyautogui.keyUp('alt')
        pyautogui.keyUp('ctrl')
    else:
        print(f'Unsupported OS: {OS}')



print('connected')

try:
    while True:
        msg = ''
        msg = s.recv(1024).strip().decode() # receive command

        if msg == 'left': # swipe left
            switch_screen('right')

        elif msg == 'up': # swipe up
            pyautogui.press('space')

        elif msg == "right": # swipe right
            switch_screen('left')


        elif msg == "down": # swipe down
            pyautogui.press('left')
        elif msg == "custom":
            t = threading.Thread(target = custom, args = (custom_command, ))
            t.start()
        elif msg == "custom2":
            t = threading.Thread(target = custom, args=(custom_command2, ))
            t.start()
except KeyboardInterrupt:
    print('Receive KeyboardInterrupt, closing connection')
    s.close()
    print('Connection closed')
