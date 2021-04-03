
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp> 
#include <exception>
#include <iostream>
#include <memory>

#include "../VertexBuffer.h"
#include "../VertexArray.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	SDL_Window* window = SDL_CreateWindow("OpenGL Template",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

	const GLfloat colors[] = {
  1.0f, 0.0f, 0.0f, 1.0f,
  0.0f, 1.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f, 1.0f,
	};

	const GLfloat positions[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
	};

	float angle = 450.0f;
	GLint success = 0;

	// VertexBuffer* positionsVbo = new VertexBuffer();

	//Texture Loading
	
	int w = 0;
	int h = 0;
	int channels = 0;

	unsigned char* data = stbi_load("Whiskers_diffuse.png", &w, &h, &channels, 4);
	if (!data)
	{
		throw std::exception();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	//Create Position Buffer 
	std::shared_ptr<VertexBuffer> positionsVbo = std::make_shared<VertexBuffer>();
	positionsVbo->add(glm::vec3(0.0f, 0.5f, 0.0f));
	positionsVbo->add(glm::vec3(-0.5f, -0.5f, 0.0f));
	positionsVbo->add(glm::vec3(0.5f, -0.5f, 0.0f));

	//Create Colour Buffer 
	std::shared_ptr<VertexBuffer> colorVbo = std::make_shared<VertexBuffer>();
	colorVbo->add(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); 
	colorVbo->add(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colorVbo->add(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


	//Create Vertex Array
	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->setBuffer(0, positionsVbo);
	vao->setBuffer(1, colorVbo);

	std::shared_ptr<VertexArray> cat = std::make_shared<VertexArray>("models/curuthers/curuthers.obj");

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const GLchar* vertexShaderSrc =
		"														" \
		"uniform mat4 u_Projection;								" \
		"uniform mat4 u_Model;									" \
		"uniform mat4 u_View;									" \
		"													    " \
		"attribute vec3 a_Position;								" \
		"attribute vec3 a_Normal;								" \
		"attribute vec3 a_TexCoord;								" \
		"													    " \
		"varying vec3 v_FragPos;								" \
		"varying vec3 v_Normal;									" \
		"varying vec2 v_TexCoord;								" \
		"														" \
		"void main()											" \
		"{																		 " \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));					 " \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0); " \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));							 " \
		" v_TexCoord = a_TexCoord;												 " \
		"}																		 " \
		"														";


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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const GLchar* fragmentShaderSrc =
		"uniform vec4 u_Color;								" \
		"varying vec3 v_FragPos;							" \
		"varying vec3 v_Normal;								" \
		"uniform sampler2D u_Texture;						" \
		"varying vec2 v_TexCoord;							" \
		"													" \
		"void main()										" \
		"{													" \
		" vec3 lightPos = vec3(20, 10, 10);					" \
		" vec3 norm = normalize(v_Normal);					" \
		" vec3 lightDir = normalize(lightPos - v_FragPos);	" \
		"													" \
		" float diff = max(dot(norm, lightDir), 0.0);		" \
		" vec3 diffuse = vec3(1, 1, 1) * diff;				" \
		" vec4 tex = texture2D (u_Texture, v_TexCoord);		" \
		" gl_FragColor = tex * vec4(diffuse, 1);								" \
		"}													" \
		"                          ";
	//gl_FragColor =  vec4(diffuse, 1);

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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Creating and binding a texture

	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Upload image data to bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Free the loaded data cus it has a copy on the gpu
	free(data);

	// Generate mipmap so texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);










	
	// Create new shader program and attach our shader objects
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	


	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link.
	glBindAttribLocation(programId, 0, "a_Position");
	glBindAttribLocation(programId, 1, "u_Color");
	glBindAttribLocation(programId, 1, "a_TexCoord");

	// Perform the link and check for failure
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

	if (uniformId == -1)
	{
		throw std::exception();
	}




	if (modelLoc == -1)
	{
		throw std::exception();
	}

	if (projectionLoc == -1)
	{
		throw std::exception();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Store location of the color uniform and check if successfully found
	//GLint colorUniformId = glGetUniformLocation(programId, "u_Color");

	/*if (colorUniformId == -1)
	{
		throw std::exception();
	}*/



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	if (!success)
	{
		throw std::exception();
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Bind the shader to change the uniform, set the uniform and reset state
	glUseProgram(programId);
	//glUniform4f(colorUniformId, 0, 1, 0, 1);
	glUseProgram(0);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	bool quit = false;

	while (!quit)
	{
		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.f);

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		// view matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0, 0, 10));
		view = glm::rotate(view, glm::radians(0 * 0.03f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::rotate(view, glm::radians(angle * 0.03f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::inverse(view);
	

		// Increase the float angle so next frame the triangle rotates further
		angle += 0.8f;

		// Draw shape as before
		////////////////////////////////
		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(programId);
		//glBindVertexArray(vao->getId());
		glBindVertexArray(cat->getId());

		//texture stuff
		glBindTexture(GL_TEXTURE_2D, textureId);
		glEnable(GL_DEPTH_TEST);

		// Upload the model matrix
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Upload the projection matrix
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
				glm::value_ptr(projection));

		// upload the view matrix

			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(view)));

			// uniform id
			//glUniform1i(uniformId, 1);

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, cat->getVertCount());

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		glDisable(GL_DEPTH_TEST);
		SDL_GL_SwapWindow(window);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}