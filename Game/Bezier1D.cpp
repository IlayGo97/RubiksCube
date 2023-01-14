#include "Bezier1D.h"

Bezier1D::Bezier1D(int segNum,int res,int mode, int viewport)
{
    segmentsNum = segNum;
    resT = res;
    this->SetMode(mode);
}

IndexedModel Bezier1D::GetLine() {
    IndexedModel model;
    int num_of_dots_on_line = (resT - 1) / segmentsNum;
    for (int i = 0; i < resT; i++) {
        model.indices.push_back(i);
    }

    glm::vec4 p_0 = GetControlPoint(0, 0);
    model.positions.push_back(glm::vec3(p_0.x, p_0.y, p_0.z));

    for (int i = 0; i < segmentsNum; i++) {
        for (int j = 0; j < num_of_dots_on_line; j++) {
            float t = (1.f / (float)num_of_dots_on_line) * (j + 1);
            glm::vec4 p_t = GetPointOnCurve(i, t);

            // Normal to the Curve (2D)
            glm::vec3 dt = GetVelosity(i, t);
            float sqrt_t = sqrt(pow(dt.x, 2) + pow(dt.y, 2));
            glm::vec3 normal = glm::vec3(-dt.y / sqrt_t, dt.x / sqrt_t, 0);

            model.positions.push_back(glm::vec3(p_t.x, p_t.y, p_t.z));
            model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
            model.normals.push_back(normal);
        }
    }

    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t)
{
    glm::vec4 b_0 = segments[segment][0]; //p0
    glm::vec4 b_1 = segments[segment][1]; //p1
    glm::vec4 b_2 = segments[segment][2]; //p2
    glm::vec4 b_3 = segments[segment][3]; //p3

    float a_0 = pow(1 - t, 3);
    float a_1 = 3 * pow(1 - t, 2) * t;
    float a_2 = 3 * (1 - t) * pow(t, 2);
    float a_3 = pow(t, 3);

    glm::vec4 b_t = a_0 * b_0 + a_1 * b_1 + a_2 * b_2 + a_3 * b_3;

    return b_t;
}

glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
    glm::vec4 b0 = segments[segment][0]; //p0
    glm::vec4 b1 = segments[segment][1]; //p1
    glm::vec4 b2 = segments[segment][2]; //p2
    glm::vec4 b3 = segments[segment][3]; //p3

    float a0 = -3 * pow(1 - t, 2);
    float a1 = 3 - 12 * t + 9 * pow(t, 2);
    float a2 = 6 * t - 9 * pow(t, 2);
    float a3 = 3 * pow(t, 2);

    glm::vec4 db_t = a0 * b0 + a1 * b1 + a2 * b2 + a3 * b3;

    return glm::vec3(db_t.x, db_t.y, db_t.z);
}

//void Bezier1D::SplitSegment(int segment, int t)
//{
//}

void Bezier1D::AddFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3) {
    segments.clear();
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    segments[0] = glm::mat4(p0, p1, p2, p3);
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

int Bezier1D::getSegmentsNum() const {
    return segmentsNum;
}

void Bezier1D::setSegmentsNum(int segmentsNum) {
    this->segmentsNum = segmentsNum;
}

int Bezier1D::getResT() const {
    return resT;
}

void Bezier1D::setResT(int resT) {
    Bezier1D::resT = resT;
}

const glm::mat4 &Bezier1D::getM() const {
    return M;
}

void Bezier1D::setM(const glm::mat4 &m) {
    M = m;
}
