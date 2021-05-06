#include <exception>
#include "Shader.h"

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
	//Debugging purposes
	GLint success = 0;

	//Fragment Shader
	const GLchar* fragmentShaderSrc = fragmentShader;

	// Create a new fragment shader, attach source code, compile it and
	// check for errors.
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorLog.at(0));
		std::cout << &errorLog.at(0) << std::endl;
		throw std::exception();
	}

	//Vertex Shader
	const GLchar* vertexShaderSrc = vertexShader;

	// Create a new vertex shader, attach source code, compile it and
	// check for errors.
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog.at(0));
		std::cout << &errorLog.at(0) << std::endl;
		throw std::exception();
	}

	//Create program and bind shader
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link.
	glBindAttribLocation(programId, 0, "a_Position");
	glBindAttribLocation(programId, 1, "u_Color");
	glBindAttribLocation(programId, 1, "a_TexCoord");

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	// Find uniform locations
	GLint modelLoc = glGetUniformLocation(programId, "u_Model");
	GLint viewLoc = glGetUniformLocation(programId, "u_View");
	GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");
	GLint uniformId = glGetUniformLocation(programId, "u_Texture");

	glUseProgram(programId);



}

Shader::~Shader()
{
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	if (!success)
	{
		throw std::exception();
	}
}

void Shader::draw(VertexArray* vertexArray)
{
	//Display cat
	glDrawArrays(GL_TRIANGLES, 0, vertexArray->getVertCount());
}

void Shader::setUniform(std::string uniform, glm::vec4 value) 
{
	// upload the model matrix

	glUseProgram(id);
	glUniform4f(glGetUniformLocation(id, uniform.c_str()), value.x, value.y, value.z, value.w);

	glUseProgram(0);
}

void Shader::setUniform(std::string uniform, glm::mat4 value)
{
	// upload the view matrix

	glUseProgram(id);
	glUniformMatrix4fv(glGetUniformLocation(id, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(glm::inverse(value)));

	glUseProgram(0);
}

void Shader::setUniform(std::string uniform, float value)
{
	// Upload the projection matrix
	glUseProgram(id);
	glUniform1f(glGetUniformLocation(id, uniform.c_str()), value);

	glUseProgram(0);
}

void Shader::setUniform(std::string uniform, Texture* texture)
{
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, uniform.c_str()), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getId());
	glUseProgram(0);
}

GLuint Shader::getId()
{
	return id;
}

GLint Shader::getFragmentId()
{
	return fragmentShaderId;
}

GLint Shader::getVertexId()
{
	return vertexShaderId;
}
