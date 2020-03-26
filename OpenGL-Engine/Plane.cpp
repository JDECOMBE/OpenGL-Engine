#define GLM_FORCE_CTOR_INIT

#include "Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <guiddef.h>

rendering::Plane::Plane(glm::mat4 const& normalize_basis, glm::vec2 const& size, std::string const& texturePath)
	: IGraphNode(normalize_basis), m_size(size), m_shader("./Shader/Vertex/plane.vert", "./Shader/Fragment/plane.frag"), m_texture(texturePath, 0)
{
	glGenVertexArrays(1, &this->m_vao);
	glGenBuffers(1, &this->m_vbo);
	glBindVertexArray(this->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 30, this->m_planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void rendering::Plane::Render(rendering::Camera const& camera, int framebuffer)
{
	for (auto& elt : this->m_children)
		elt->Render(camera, framebuffer);

	auto model = glm::scale(glm::mat4(1.f), { this->m_size[0], 1, this->m_size[1] });
	model = this->InWorld() * this->m_basis * model;
	this->m_shader.use();
	this->m_shader.setMat4("model", model);
	this->m_shader.setMat4("view", camera.GetViewMatrix());
	this->m_shader.setMat4("projection", camera.GetProjectionMatrix(this->m_size));
	this->m_texture.Use();
	this->m_shader.setInt("texture1", this->m_texture.GetTextureUnit());

	glBindVertexArray(this->m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


}
