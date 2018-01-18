## CNC Plotter - Art Machine ## 
What initially started as a "square drawer" evolved into the art drawing machine as shown:

<link to image/video>

**Hardware used:** Arduino UNO, breadboard, wires. As well as:
- Two optical drives were disassembled and act as rails to move the plotter in the x and y direction. 
- Two motors independently power the movement in the x direction and y direction.
- A microservo necessary to lift up the pen. 

As for the software itself, I implemented image transformations and manipulations using OpenCV, a computer vision library, as well as an algorithm (python) to optimize the planned path of the pen (~30% faster compared to normal CNC printing order). Finally, I bridged the communication between the Arduino serial input and the python image processing program, to reduce the Arduino's processing consumption by ~500%.

Here are some of it's masterpieces!

<insert images here>
