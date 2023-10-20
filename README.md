# Darkness
An atmospheric third person shooter written in a Cherno project derived C++ graphical engine for my Advanced Games Technology module in my second year at university. 

# Notable Algorithms and Learnings:
## Creating primitive models and understanding how rendering works.
- I have created several primitive shapes made up of vertices which were stored in a VAO (Vertex Array Object).
- I have worked out normals of said vertices by working out the cross product of a set of vertices. The normals alongside the texcoords and vertices were stored in VBO's (Vertex Buffer Objects)
- The normals were used for the lighting in the scene.
- This was an intro to rendering which was expanded upon in the Tank-Survival project which focused on the Rendering side of an engine featuring the rendering pipeline of OpenGL.

Link: https://github.com/PatrykOwczarz/Darkness/blob/main/Darkness/game/src/example_layer.cpp

# Areas of Improvement:
- **Level design:** Not implemented at all. This was more of a "tech demo" rather than a game without the level.
- **AI:** Very simple AI based on an FSM that used proximity based behaviours. Patrol, if player in range, chase them and deduct damage upon collision. A lot of room for expansion
- **Animations and meshes:** Hitboxes are off and no animations for most models. A lot of 3D art work which was not the focus of this project.
- **Major refactoring of project:** The code works but is a nightmare to read. I learned a valuable lesson about managing a codebase in this project.

# Demo Video:
Demo: https://www.youtube.com/watch?v=98PIADZ3Cx0
