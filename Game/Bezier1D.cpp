#include <iostream>
#include "Bezier1D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/compatibility.hpp"

Bezier1D::Bezier1D(int segNum,int res, int mode, Scene* scene, int viewport): segmentsNum(segNum), resT(res), selected_index(0), Shape()
{
    this->mode = Scene::LINE_STRIP;
    int num_of_control_points = segNum * 3 + 1;
    for (int i = 0; i < num_of_control_points; i ++){
        this->control_points.emplace_back(-(num_of_control_points) + 2 * i, 3.f ,0.f);
        int curr_control_point_index = scene->shapes.size();
        scene->AddShape(scene->Octahedron, -1, scene->TRIANGLES);
        scene->SetShapeTex(curr_control_point_index, 0);
        this->control_points_shape.push_back(scene->shapes[curr_control_point_index]);
        control_points_shape[curr_control_point_index]->MyTranslate(control_points[curr_control_point_index], 0);
        control_points_shape[curr_control_point_index]->MyScale(glm::vec3(0.5,0.5,0.5));
    }
    SetBezier1DMesh(GetLine());
    blck = (block*)scene->shapes[scene->AddBlock(glm::vec3(GetControlPoint(0,0)))];
}

IndexedModel Bezier1D::GetLine() {
    IndexedModel model;
    int num_of_res_per_segment = (resT - 1) / segmentsNum;
    for (int i = 0; i < resT; i++) {
        model.indices.push_back(i);
    }
    glm::vec4 p0 = GetControlPoint(0, 0);
    model.positions.emplace_back(p0.x, p0.y, p0.z);

    for (int i = 0; i < segmentsNum; i++) {
        for (int j = 0; j < num_of_res_per_segment; j++) {
            float t = (1.f / (float) num_of_res_per_segment) * (j + 1);
            glm::vec4 p_t = GetPointOnCurve(i, t);

            // Normal to the Curve (2D)
            glm::vec3 dt = GetVelosity(i, t);
            float sqrt_t = sqrt(pow(dt.x, 2) + pow(dt.y, 2));
            glm::vec3 normal = glm::vec3(-dt.y / sqrt_t, dt.x / sqrt_t, 0);
            model.positions.emplace_back(p_t.x, p_t.y, p_t.z);
            model.colors.emplace_back(1.f, 1.f, 1.f);
            model.normals.push_back(normal);
        }
    }
    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    return glm::vec4(control_points[segment * 3 + indx], 0.f);
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, float t)
{
    glm::vec4 b_0 = GetControlPoint(segment, 0); //p0
    glm::vec4 b_1 = GetControlPoint(segment, 1); //p1
    glm::vec4 b_2 = GetControlPoint(segment, 2); //p2
    glm::vec4 b_3 = GetControlPoint(segment, 3); //p3

    float a_0 = (float)pow(1 - t, 3);
    float a_1 = (float)(3 * pow(1 - t, 2) * t);
    float a_2 = (float)(3 * (1 - t) * pow(t, 2));
    float a_3 = (float)pow(t, 3);

    glm::vec4 b_t = a_0 * b_0 + a_1 * b_1 + a_2 * b_2 + a_3 * b_3;

    return b_t;
}

glm::vec3 Bezier1D::GetVelosity(int segment, float t)
{
    glm::vec4 b_0 = GetControlPoint(segment, 0); //p0
    glm::vec4 b_1 = GetControlPoint(segment, 1); //p1
    glm::vec4 b_2 = GetControlPoint(segment, 2); //p2
    glm::vec4 b_3 = GetControlPoint(segment, 3); //p3

    float a_0 = -3 * pow(1 - t, 2);
    float a_1 = 3 - 12 * t + 9 * pow(t, 2);
    float a_2 = 6 * t - 9 * pow(t, 2);
    float a_3 = 3 * pow(t, 2);

    glm::vec4 db_t = a_0 * b_0 + a_1 * b_1 + a_2 * b_2 + a_3 * b_3;

    return glm::vec3(db_t.x, db_t.y, db_t.z);
}

//void Bezier1D::SplitSegment(int segment, int t)
//{
//}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeSegment(int segIndx,glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx-1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx,float dy,bool preserveC1)
{
    return 0; //not suppose to reach here
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1)
{
}

void Bezier1D::ResetCurve(int segNum)
{

}

Bezier1D::~Bezier1D(void)
{

}

void Bezier1D::selectNextControlPoint() {
    selected_index = (selected_index + 1) % control_points.size();
}

void Bezier1D::selectPreviousControlPoint() {
    selected_index = (selected_index - 1) % control_points.size();
}


void Bezier1D::TranslateSelectedPoint(glm::vec3 delta) {
    control_points[selected_index].x += delta.x;
    control_points[selected_index].y += delta.y;
    control_points[selected_index].z += delta.z;
    control_points_shape[selected_index]->MyTranslate(delta, 0);
    CurveRefresh();
}


void Bezier1D::moveSelectedControlPointUp() {
    TranslateSelectedPoint(glm::vec3(0,MOVEMENT_DELTA,0));
}

void Bezier1D::moveSelectedControlPointDown() {
    TranslateSelectedPoint(glm::vec3(0,-MOVEMENT_DELTA,0));
}


void Bezier1D::moveSelectedControlPointIn() {
    TranslateSelectedPoint(glm::vec3(0,0,MOVEMENT_DELTA));
}

void Bezier1D::moveSelectedControlPointOut() {
    TranslateSelectedPoint(glm::vec3(0,0,-MOVEMENT_DELTA));
}

void Bezier1D::CurveRefresh() {
    SetBezier1DMesh(GetLine());
}

glm::mat4 get_rotation_matrix(glm::vec3 v1, glm::vec3 v2){
    v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);
    float angle = glm::dot(v1, v2);
    glm::vec3 axis = glm::cross(v1, v2);
    return glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Bezier1D::continueAnimation() {
    if (!anim.animating)
        return;
    float new_t = anim.cube_t + 0.001f * anim.direction;
    int new_segment = anim.segment;
    if (new_t <= 0) {
        new_segment = new_segment - 1;
        if (new_segment == -1) {
            new_segment = 0;
            anim.direction = 1;
            new_t = 0;
        } else {
            new_t = 1 + new_t;
        }
    }
    else if (new_t >=1) {
        new_segment = new_segment + 1;
        if (new_segment == segmentsNum) {
            new_segment = segmentsNum - 1;
            anim.direction = -1;
            new_t = 1;
        } else {
            new_t = new_t - 1;
        }
    }
    glm::vec3 newPosition = glm::vec3(GetPointOnCurve(new_segment, new_t));
//    glm::vec3 v1 = glm::normalize(GetVelosity(anim.segment, anim.cube_t));
//    glm::vec3 v2 = glm::normalize(GetVelosity(new_segment, new_t));
//    float angle = glm::dot(v1,v2);
//    glm::vec3 normal = glm::cross(v1, v2);
//    std::cout << "angle: " << angle << "\n";
//    std::cout << "normal.x: " << normal.x << "\n";
//    blck->MyRotate(angle/111.f, normal, 0);
    blck->set_position(newPosition);
    anim.segment = new_segment;
    anim.cube_t = new_t;

}

void Bezier1D::ToggleAnimation() {
    anim.animating = !anim.animating;
}

void Bezier1D::Draw(const std::vector<Shader *> shaders, const std::vector<Texture *> textures, bool isPicking) {
    Shape::Draw(shaders, textures, isPicking);
    continueAnimation();
}
