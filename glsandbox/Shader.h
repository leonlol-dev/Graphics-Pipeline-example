#include <GL/glew.h>
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp> 
#include "Texture.h"
#include "VertexArray.h"

struct Shader
{
public:
	Shader(const char* VertexShader, const char* fragmentShader);
	~Shader();
	void draw(VertexArray* vertexArray);
	void setUniform(std::string uniform, glm::mat4 value);
	void setUniform(std::string uniform, glm::vec4 value);
	void setUniform(std::string uniform, float value);
	void setUniform(std::string uniform, Texture* texture);
	GLuint getId();
	GLint getFragmentId();
	GLint getVertexId();

private:
	GLuint id;
	GLuint fragmentShaderId;
	GLuint vertexShaderId;
	GLuint programId;
	GLint success;



};