#include "StaticMesh.h"
#include "tiny_obj_loader.h"

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
    //產生一個VAO
    glGenVertexArrays(1, &ret.vao);
    //綁定VAO
    glBindVertexArray(ret.vao);
    //產生3個VBO, 1個IBO
    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    // Upload postion array
    //綁定VBO
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    //void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
    //target: GL_ARRAY_BUFFER()頂點數據 or GL_ELEMENT_ARRAY_BUFFER()索引數據
    //size:所需的容量
    //*data: 用於初始化
    //usage: 放已經定義好的參數，決定如何讀寫
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
        shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
    //建立CPU與GPU的連結
    //允許頂點shader讀取GPU的數據
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

    //清除綁定
    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    //釋放掉資源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw()
{
    //綁定要繪製的VAO
    glBindVertexArray(vao);
    //渲染出來
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
