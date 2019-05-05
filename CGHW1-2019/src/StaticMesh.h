#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <glad/glad.h>
#include <string>
class StaticMesh {
public:
    StaticMesh(const StaticMesh &rhs)=default;
    //釋放掉VAO, VBO, IBO
    void release();
    //從檔案載入mesh
    static StaticMesh LoadMesh(const std::string &filename);
    //綁定VAO並渲染出來
    void draw();

	bool hasNormal() const;
	bool hasUV() const;
    //判斷VAO是否相等
    bool operator!=(const StaticMesh &rhs) const
    { return vao != rhs.vao; }
private:
    StaticMesh();
    //VAO vertex array object
    GLuint vao;
    //VBO  vertex buffer object
    GLuint vbo[3];
    //IBO index buffer object
    GLuint ibo;
    //number of indices
    GLuint numIndices;

    //m_uv  mesh的uv向量
    //m_normal  mesh的法向量
	bool m_uv, m_normal;
};

#endif