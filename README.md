# 3D-Optical-Illusion-Art-Gallery

This is a Art Gallery with the theme of Optical Illusion that is created in C++ using OpenGL API and Microsoft Visual Studio.

The scene consists of three separate AAOs which are placed in a gallery with walls. 
All the AAOs can be viewied.
AAO1 and AAO2 represent optical illusion whereas 
AAO3 is a 3D model of a road with tunnel and traffic lights. You can also control the traffic light state in the models. 
Each of these AAO’s can be viewed using the control binds and have animations. 



Controls:

↑ Arrow - Move Forwards
↓ Arrow - Move Backwards
← Arrow - Turn Left
→ Arrow - Turn Right
1 - AAO1 View (Ames Window)
2 - AAO2 View (Moire Patterns)
3 - AAO3 View (3D Road Scene)
0 - Gallery View
F1 - Move Up
F2 - Move Down
F5 - Turn Red Traffic Signal On
F6 - Turn Yellow Traffic Signal On
F7 - Turn Green Traffic Signal On


Instruction to run project in Linux:

1. Download the project zip file and drag it to your desktop.
2. Extract items from the zip. A folder called “msa280” would appear.
3. Go to terminal and change directory by using: cd Desktop/msa280
4. Now compile the file using following: g++ Assignment1.cpp -lGL -lGLU -lglut
5. A folder called a.out would appear. Run the program using: ./a.out
