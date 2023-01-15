#pragma once
#include "scene.h"
#include "rubik.h"
#include "Bezier1D.h"

class Game : public Scene
{
public:

	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
    virtual void MouseProccessing(int button) override;
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
    void keyListener(int key) override;
    void scrollCallback(double xoffset, double yoffset);
    void AddBezier1DShape(Shape* bezier_1D_line, int parent);
    Bezier1D* bezierCurve;

};

