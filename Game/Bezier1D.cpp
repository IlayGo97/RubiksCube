#include <iostream>
#include "Bezier1D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/compatibility.hpp"
#include "game.h"

#define PI 3.1415926535

Bezier1D::Bezier1D(int segNum,int res, int mode, Scene* scene, int viewport): segmentsNum(segNum), resT(res), selected_index(0), Shape()
{
    this->mode = Scene::LINE_STRIP;
    Restart(segNum, scene);
}

void Bezier1D::Restart(int segNum, Scene *scene) {
    scene->shapes.clear();
    control_points.clear();
    control_points_shape.clear();
    segmentsNum = segNum;
    resT = segNum * 30 + 1;
    int num_of_control_points = segNum * 3 + 1;
    for (int i = 0; i < num_of_control_points; i ++){
        control_points.emplace_back(-(num_of_control_points) + 2 * i, 3.f , 0.f);
        int curr_control_point_index = scene->shapes.size();
        scene->AddShape(scene->Octahedron, -1, scene->TRIANGLES);
        scene->SetShapeTex(curr_control_point_index, 0);
        control_points_shape.push_back(scene->shapes[curr_control_point_index]);
        control_points_shape[curr_control_point_index]->MyTranslate(control_points[curr_control_point_index], 0);
        control_points_shape[curr_control_point_index]->MyScale(glm::vec3(0.5, 0.5, 0.5));
    }
    SetBezier1DMesh(GetLine());
    makeQuarterCircles();
    blck = (block*)scene->shapes[scene->AddBlock(glm::vec3(GetControlPoint(0, 0)))];
    ((Game*)scene)->AddBezier1DShape(this, -1);
    scene->SetShapeTex(scene->shapes.size() - 1, 1);
    set_block_rotation(0,0);
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
    glm::vec4 p0 = GetControlPoint(segment, 0); //p0
    glm::vec4 p1 = GetControlPoint(segment, 1); //p1
    glm::vec4 p2 = GetControlPoint(segment, 2); //p2
    glm::vec4 p3 = GetControlPoint(segment, 3); //p3

    float a_0 = (float)pow(1 - t, 3);
    float a_1 = (float)(3 * pow(1 - t, 2) * t);
    float a_2 = (float)(3 * (1 - t) * pow(t, 2));
    float a_3 = (float)pow(t, 3);

    glm::vec4 b_t = a_0 * p0 + a_1 * p1 + a_2 * p2 + a_3 * p3;

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


void Bezier1D::moveControlPoint(int point_index, glm::vec3 delta) {
    control_points[point_index].x += delta.x;
    control_points[point_index].y += delta.y;
    control_points[point_index].z += delta.z;
    control_points_shape[point_index]->MyTranslate(delta, 0);
    CurveRefresh();
}

bool is_edge_point(int index){
    return index % 3 == 0;
}

// radians
float angle_between_vecs(glm::vec3 v1, glm::vec3 v2){
    v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);
    return glm::acos(glm::clamp(glm::dot(v1,v2 ), -1.f , 1.f));
}

float distance_from_pi(float angle){
    angle = std::abs(angle);
    return std::min((float)std::abs(angle - PI), angle);
}

void Bezier1D::controlled_movement(int index, glm::vec3 delta){
    if (index != selected_index)
        moveControlPoint(index, delta);
}

int get_delta_dimension(const glm::vec3& delta){
    if (delta.x != 0.f)
        return 0;
    else if (delta.y != 0.f)
        return 1;
    else return 2;
}

void Bezier1D::RotatePoint(int index, float angle, glm::vec3 axis) {
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,0,1));
    glm::vec3 new_point = glm::vec3(rotation_matrix * glm::vec4(control_points[index] - axis,1.0f));
    moveControlPoint(index, control_points[index] - new_point - axis);
}

void Bezier1D::TranslateSelectedPoint(glm::vec3 delta) {
    if (is_edge_point(selected_index)) {
        moveControlPoint(selected_index, delta);
        if (selected_index != 0) {
            moveControlPoint(selected_index - 1, delta);
        }
        if (selected_index != control_points.size() - 1){
            moveControlPoint(selected_index + 1, delta);
        }
    } else {
        int angle = 1;
        if (delta.y > 0) {
            angle = angle * -1;
        }
        if (selected_index % 3 == 1) {
            RotatePoint(selected_index, angle, control_points[selected_index - 1]);
            if (continuity_state && selected_index != 1){
                RotatePoint(selected_index - 2, angle, control_points[selected_index - 1]);
            }
        } else if (selected_index % 3 == 2) {
            RotatePoint(selected_index, angle, control_points[selected_index + 1]);
            if (continuity_state && selected_index != control_points.size() - 2){
                RotatePoint(selected_index + 2, angle, control_points[selected_index + 1]);
            }
        }
    }
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

void Bezier1D::set_block_rotation(int segment, float t){
    glm::mat4 new_rotation = glm::mat4(1);
    glm::vec3 v = glm::normalize(GetVelosity(segment, t));
    glm::vec3 y = glm::normalize(glm::cross(v, glm::vec3(0, 0, 1)));
    glm::vec3 z = glm::cross(v, y);
    glm::mat4 rotation_matrix = glm::mat4(v.x, y.x, z.x, 0,
                                          v.y, y.y, z.y, 0,
                                          v.z, y.z, z.z, 0,
                                          0,   0,   0,   1);
    blck->setRot(glm::mat4(1) * rotation_matrix);
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
    set_block_rotation(new_segment, new_t);
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

void Bezier1D::fixCurve() {
    if (!is_edge_point(selected_index) || selected_index == 0 || selected_index == control_points.size() - 1) {
        return;
    }
    glm::vec3 p0 = control_points[selected_index];
    glm::vec3 p1 = control_points[selected_index + 1];
    glm::vec3 p2 = control_points[selected_index - 1];
    glm::vec3 p0p1 =  glm::normalize(p0 - p1);
    float t = glm::distance(p2, p0);
    glm::vec3 p2tag = p0p1 * t + p0;
    moveControlPoint(selected_index - 1, p2tag - p2);
}

void Bezier1D::toggle_continuity_state() {
    continuity_state = !continuity_state;
}

glm::vec3 get_delta(glm::vec3 original, glm::vec3 destination){
    return destination - original;
}

// according to: https://spencermortensen.com/articles/bezier-circle/
void Bezier1D::makeQuarterCircles() {
    float a = 1.00005519f * 2.5f;
    float b = 0.55342686f * 2.5f;
    float c = 0.99873585 * 2.5f;
    // first half:
    glm::vec3 p0, p1, p2, p3;
    p2 = control_points[2];
    p3 = control_points[3];
    moveControlPoint(2, get_delta(p2, p3 + glm::vec3(0-b, c - a ,0)));
    p1 = control_points[1];
    p2 = control_points[2];
    moveControlPoint(1, get_delta(p1, p2 + glm::vec3(b - c, b - c ,0)));
    p0 = control_points[0];
    p1 = control_points[1];
    moveControlPoint(0, get_delta(p0, p1 + glm::vec3(c - a, 0 - b ,0)));


    // second half:
    p0 = control_points[control_points.size() - 4];
    p1 = control_points[control_points.size() - 3];
    moveControlPoint(control_points.size() - 3, get_delta(p1, p0 + glm::vec3(b - 0, c - a ,0)));
    p1 = control_points[control_points.size() - 3];
    p2 = control_points[control_points.size() - 2];
    moveControlPoint(control_points.size() - 2, get_delta(p2, p1 + glm::vec3(c - b, b - c ,0)));
    p2 = control_points[control_points.size() - 2];
    p3 = control_points[control_points.size() - 1];
    moveControlPoint(control_points.size() - 1, get_delta(p3, p2 + glm::vec3(a - c, 0 - b ,0)));
//    moveControlPoint(0, glm::vec3(1.00005519, -1.00005519f, 0));
}
