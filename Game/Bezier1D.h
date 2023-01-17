#pragma once

//#include "Mesh.h"
#include "../Engine3D/shape.h"
#include "scene.h"
#include "block.h"

#define MOVEMENT_DELTA 0.1f
struct animation{
    int direction = 1;
    float cube_t = 0.f;
    int segment = 0;
    bool animating = false;
};

class Bezier1D : public Shape
{
	int segmentsNum;
	std::vector<glm::mat4> segments;
	int resT;
    glm::mat4 M;
    float MoveControlPoint(int segment, int indx, float dx, float dy, bool preserveC1);  //change the position of one control point. when preserveC1 is true it may affect other  control points
public:
	Bezier1D(int segNum, int res,int mode, Scene* scene, int viewport = 0);
	IndexedModel GetLine();	//generates a model for MeshConstructor Constructor with resT vertices. See Mesh.h/cpp for a IndexedModel defenition and how to intialize it
	glm::vec4 GetControlPoint(int segment, int indx) const; //returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3
    inline void SetBezier1DMesh(const IndexedModel bezier_1D_line) { this->mesh = new MeshConstructor(bezier_1D_line, false); }
	glm::vec4 GetPointOnCurve(int segment, float t); //returns point on curve in the requested segment for the value of t
	glm::vec3 GetVelosity(int segment, float t); //returns the derivative of the curve in the requested segment for the value of t
    void Draw(const std::vector<Shader *> shaders, const std::vector<Texture *> textures, bool isPicking) override;

	void AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3);  // adds a segment at the end of the curve
	void CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1 = false);  //changes the line in by using ChangeLine function of MeshConstructor and MoveControlPoint 
	void ChangeSegment(int segIndx, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3); // changes three control point of a segment
	void ResetCurve(int segNum); // to the init state
	inline int GetSegmentsNum() const { return segmentsNum; }
    void continueAnimation();
    void CurveRefresh();
    void ToggleAnimation();
    ~Bezier1D(void);

    void moveSelectedControlPointUp();
    void moveSelectedControlPointDown();
    void moveSelectedControlPointIn();
    void moveSelectedControlPointOut();
    void selectNextControlPoint();
    void selectPreviousControlPoint();
    void fixCurve();
    void Restart(int segNum, Scene *scene);
    void TranslateSelectedPoint(glm::vec3 delta);
    void toggle_continuity_state();
private:
    block* blck;
    int selected_index;
    std::vector<glm::vec3> control_points;
    std::vector<Shape*> control_points_shape;
    animation anim;

    bool continuity_state = false;
    void moveControlPoint(int point_index, glm::vec3 delta);
    void fix_velosity(int segment1, int segment2, glm::vec3 delta);
    void controlled_movement(int index, glm::vec3 delta);
    void RotatePoint(int index, float angle, glm::vec3 axis);
    void makeQuarterCircles();

    void set_block_rotation(int segment, float t);
};

