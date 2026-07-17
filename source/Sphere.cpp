#define STB_IMAGE_IMPLEMENTATION

#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <../textures/stb_image.h>

#include <filesystem>



Sphere::~Sphere()
{
}



Sphere::Sphere(float rad, GLuint sl, GLuint st) :
radius(rad), slices(sl), stacks(st)
{


	

	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];  
	// Tex coords
	float * tex = new float[2 * nVerts];    //we don't use it now
	// Index
	unsigned int * el = new unsigned int[elements];  //index

	// Generate the vertex data
	generateVerts(v, n, tex, el);
	

	//create vao, vbo and ibo here... (We didn't use std::vector here...)
	// create vao, vbo and ibo here
	//interive 방식을 위해 data 추가 
	float* data = new float[8 * nVerts];

	for (int i = 0; i < nVerts; i++)
	{
		data[8 * i + 0] = v[3 * i + 0];
		data[8 * i + 1] = v[3 * i + 1];
		data[8 * i + 2] = v[3 * i + 2];

		data[8 * i + 3] = n[3 * i + 0];
		data[8 * i + 4] = n[3 * i + 1];
		data[8 * i + 5] = n[3 * i + 2];

		data[8 * i + 6] = tex[2 * i + 0];
		data[8 * i + 7] = tex[2 * i + 1];
	}

	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(2, handle);

	// VBO 설정
	glNamedBufferData(
		handle[0],
		sizeof(float) * 8 * nVerts,
		data,
		GL_STATIC_DRAW
	);

	glVertexArrayVertexBuffer(
		VAO,
		0,
		handle[0],
		0,
		8 * sizeof(float)
	);

	// Position attribute
	glVertexArrayAttribFormat(
		VAO,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0
	);

	glVertexArrayAttribBinding(VAO, 0, 0);
	glEnableVertexArrayAttrib(VAO, 0);

	// Normal attribute
	glVertexArrayAttribFormat(
		VAO,
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float)
	);

	glVertexArrayAttribBinding(VAO, 1, 0);
	glEnableVertexArrayAttrib(VAO, 1);

	// Texcoord attribute
	glVertexArrayAttribFormat(
		VAO,
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float)
	);

	glVertexArrayAttribBinding(VAO, 2, 0);
	glEnableVertexArrayAttrib(VAO, 2);

	// EBO
	glNamedBufferData(
		handle[1],
		sizeof(unsigned int) * elements,
		el,
		GL_STATIC_DRAW
	);

	glVertexArrayElementBuffer(VAO, handle[1]);

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;
	delete[] data;


	std::cout << "현재 작업 디렉토리: " << std::filesystem::current_path() << std::endl;
	//texture obj 생성 
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	int width, height, channel;
	unsigned char* image = stbi_load("textures/earth.jpg", & width, & height, & channel, 0);
	glTextureStorage2D(texture, 1, GL_RGB8, width, height);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateTextureMipmap(texture);
	stbi_image_free(image);

}

void Sphere::draw(){

	glBindTextureUnit(0, texture); //텍스처 unit 0번과 텍스처 객체 연결 

	glBindVertexArray(VAO);

	int size;
	glGetBufferParameteriv(
		GL_ELEMENT_ARRAY_BUFFER,
		GL_BUFFER_SIZE,
		&size
	);
	int elementCount = size / sizeof(unsigned int);

	glDrawElements(
		GL_TRIANGLES,
		elementCount,
		GL_UNSIGNED_INT,
		0
	);
	glBindVertexArray(0);
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

int Sphere::getVertexArrayHandle() 
{
	return this->VAO;
}

