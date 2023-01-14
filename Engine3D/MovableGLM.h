#pragma once
#include "../res/includes/glm/glm.hpp"

class MovableGLM 
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	void MyTranslate(glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(glm::vec3 scale);
	void ZeroTrans();

    const glm::mat4 &getTrans() const;

    void setTrans(const glm::mat4 &trans);

    const glm::mat4 &getRot() const;

    void setRot(const glm::mat4 &rot);

    const glm::mat4 &getScl() const;

    void setScl(const glm::mat4 &scl);

private:
	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scl;
};