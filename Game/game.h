#pragma once
#include "scene.h"
#include "rubik.h"
#include "Bezier1D.h"

struct animation {
    int cube_shape_index;
    bool forward_direction = true;
    int cube_segment = 0;
    float cube_t = 0;
    float cube_step_size = 0.0001;
};

class Game : public Scene
{
public:

	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
    void keyListener(int key) override;
    void scrollCallback(double xoffset, double yoffset);
    bool S_mode = false;
    int picked_shape_index = -1;
    void MouseScrolling(glm::vec3 delta, int mode);
    void NumberOfSegmentsToDisplay(int segNum);
    bool HasLeft(int index);
    bool HasRight(int index);
    int NextShape();
    int PreviousShape();
    bool C_state = false;
private:
    animation anim;

    Bezier1D *bezier_1D;
    int first_point_index, last_point_index;
    int number_of_octahedrons = (6 * 3) + 1; // Max_num_of_segments * 3 + 1
    vector<vector<glm::vec3>> bezier_configs;
    void CreateRoute3DBezier1D(int segNum,int res,int mode);
    void AnimateCubeMovement(bool animate);
    void UpdateCurveByShapes();
    bool OnCurvePoint(int index);

    void BuildAllShapes();

    void AddBezier1DShape(Shape *bezier_1D_line, int parent);
};

