# Magic Wand Project
## What is it?
This device is intended to be used in a presentation to ​extend the presenter's access to the computer utilities without directly control the computer using a mouse or a keyboard. By swiping the magic wand in many different ways, the presenter is able to toggle a computer's workspace (screens), switch presentation pages, and execute up to two customized system commands. It also draws interest from the audience and creates fun.
## How to use it
1. Connect to "Wand_presentation" from WIFI list in your computer GUI, enter password "Wand123456"
2. Run python client program

3. Configure first system command (the command that triggers when you swipe the magic wand two times)

4. Configure first system command (the command that triggers when you swipe the magic wand three times)

5. Swipe a few times once connected to verify the response. Your computer should switch workspace once you swipe it left or right.

## the python client
This program is used to connect to the magic wand, receive its commands, and control the computer using keystrokes. The program also allows the presenter to execute up to two OS commands.

### Control cheat sheet
| Gesture | ability |
| ----------- | ----------- |
| Left swipe | Switch workspace (screen) right |
| Right swipe | Switch workspace (screen) left |
| Up swipe | Type <space> key|
| Down swipe x1 | Type <left> key |
| Down swipe x1 | Execute custom command 1 |
| Down swipe x1 | Execute custom command 2 |
