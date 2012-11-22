#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "scene.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

class SceneBasic : public Scene
{
private:
    int width, height;
    GLuint m_vboHandles[2];
    GLuint m_vaoHandle;
	
	GLuint m_vertShader;
	GLuint m_fragShader;
	GLuint m_programHandle;

    void linkMe(GLint vertShader, GLint fragShader);

public:
    SceneBasic();
	~SceneBasic();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
	void use();
};

#endif // SCENEBASIC_H
