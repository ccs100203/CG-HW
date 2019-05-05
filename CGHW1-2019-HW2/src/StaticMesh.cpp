#include "StaticMesh.h"
#include "tiny_obj_loader.h"
#include <iostream>
#include <iomanip>
#include <glm/vec3.hpp>
#include <typeinfo>
#include <glm/geometric.hpp>

StaticMesh::StaticMesh()
    : m_uv(false), m_normal(false)
{

}

StaticMesh StaticMesh::LoadMesh(const std::string &filename)
{

    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}

    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    // Upload postion array
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
        shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (shapes[0].mesh.texcoords.size() > 0) {
        // Upload texCoord array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
            shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_uv = true;
    }

    if (shapes[0].mesh.normals.size() > 0) {
        // Upload normal array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
            shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_normal = true;
    }

    // Setup index buffer for glDrawElements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
        shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

    ret.numIndices = shapes[0].mesh.indices.size();

    glBindVertexArray(0);
    return ret;
}

StaticMesh StaticMesh::LoadMeshFlat(const std::string &filename)
{

    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}

    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    // Upload postion array
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
        shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (shapes[0].mesh.texcoords.size() > 0) {
        // Upload texCoord array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
            shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_uv = true;
    }

    if (shapes[0].mesh.normals.size() > 0) {
        // std::cout<<"index  " << shapes[0].mesh.indices.size() <<std::endl;
        // std::cout<<"normal  " << shapes[0].mesh.normals.size() <<std::endl;
        // std::cout<<"position  " << shapes[0].mesh.positions.size() <<std::endl;

        for(int i=0; i<shapes[0].mesh.indices.size(); i+=3){
            // std::cout <<shapes[0].mesh.indices[i]<<"  "<<shapes[0].mesh.indices[i+1]<<"  "<<shapes[0].mesh.indices[i+2] << std::endl;
            glm::vec3 temp1 = {shapes[0].mesh.positions[shapes[0].mesh.indices[i] * 3], shapes[0].mesh.positions[shapes[0].mesh.indices[i] * 3 +1], shapes[0].mesh.positions[shapes[0].mesh.indices[i] * 3 +2]};
            glm::vec3 temp2 = {shapes[0].mesh.positions[shapes[0].mesh.indices[i+1] * 3], shapes[0].mesh.positions[shapes[0].mesh.indices[i+1] * 3 +1], shapes[0].mesh.positions[shapes[0].mesh.indices[i+1] * 3 +2]};
            glm::vec3 temp3 = {shapes[0].mesh.positions[shapes[0].mesh.indices[i+2] * 3], shapes[0].mesh.positions[shapes[0].mesh.indices[i+2] * 3 +1], shapes[0].mesh.positions[shapes[0].mesh.indices[i+2] * 3 +2]};
            glm::vec3 ans = glm::normalize(glm::cross(temp2-temp1, temp3-temp1));
            // std::cout << ans[0]<<"  "<< ans[1]<<"  "<< ans[2] << "    "<<i << std::endl;
            shapes[0].mesh.normals[shapes[0].mesh.indices[i] * 3] = ans[0];
            shapes[0].mesh.normals[shapes[0].mesh.indices[i] * 3 +1] = ans[1];
            shapes[0].mesh.normals[shapes[0].mesh.indices[i] * 3 +2] = ans[2];
        }

        // Upload normal array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
            shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_normal = true;
    }

    // Setup index buffer for glDrawElements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
        shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

    ret.numIndices = shapes[0].mesh.indices.size();

    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

bool StaticMesh::hasNormal() const
{
	return m_normal;
}

bool StaticMesh::hasUV() const 
{
	return m_uv;
}
