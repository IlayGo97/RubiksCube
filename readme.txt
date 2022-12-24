Changes made to the engine to create the rubik's cube game:
1. Created block object that inherits from Shape.
2. Created rubik object that saves blocks.
3. fixed in MovableGLM.cpp MakeTrans() and MyRotate()
4. In scene.cpp added AddBlock(vec3 pos) function.
5. make Draw() in shap.h virtual so we can override it in block.cpp to create animations.
6. added key callbacks to Game.h that controls rubik object.
