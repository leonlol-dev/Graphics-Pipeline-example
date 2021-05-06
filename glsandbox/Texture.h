#include <GL/glew.h>
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp> 

struct Texture
{
public: 
	Texture(const char* data);
	Texture();
	~Texture();
	glm::vec2 getSize();
	GLuint getId();

private:
	GLuint id;
	glm::vec2 size;
	int w = 0;
	int h = 0;
	int channels = 0;

};