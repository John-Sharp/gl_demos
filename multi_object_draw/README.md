Multi-object Draw
=================

Program for experimenting with developing a graphics engine. 
Editing the scene is done in a modal way, with global, model and rotation modes
The mode is indicated by the colour of the billboards above the models with:
    blue = global mode
    yellow = model mode (with yellow billboard above active model)
    green = rotation mode (with green billboard above active model)

In global mode
-------------

Press 'm' to draw a new model at the position you are currently looking at
(puts you into model mode)

Input a number and press 'enter' to enter model mode and make the object
with that number on its billboard the active model

Use the arrow keys to look around, and 'a' to move forward in direction
you are currently looking in

In model mode
-------------

Press 'm' multiple times to cycle through various different models

Use arrow keys to move model around along its local x and y axes.
Use a/s to move along model's positive/negative z axes

Press 'r' to enter rotation mode for the currently active model

Press 'esc' to enter global mode

In rotation mode
----------------

Use arrow keys and a/s to rotate around x,y,z axes

Press 'r' to re-enter model mode

Press 'esc' to enter global mode

