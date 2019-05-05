
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#define GLM_FORCE_RADIAN
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <memory>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "WenCheng.h"
#include <math.h>


static void error_callback(int error, const char* description)
{
    std::cerr<<fmt::format("Error: {0}\n", description);
}

int main(void)
{
    // create a window
    GLFWwindow* window;
    // set the error callback
    glfwSetErrorCallback(error_callback);
    // initialize GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);
    // Good bye Mac OS X
    //initialize openGL and give it some parameter
    //set openGL 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //avoid old openGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Open a window and set its size
    window = glfwCreateWindow(1200, 900, "Sun & Moon", NULL, NULL);
    if (!window)
    {
        //destroys all remaining windows
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //Makes the context of the specified window current for the calling thread
    glfwMakeContextCurrent(window);
    if(!gladLoadGL()) {
        exit(EXIT_FAILURE);
    }
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup style
    ImGui::StyleColorsDark();
    //initialize clear color
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    auto textSun = Texture2D::LoadFromFile("../resource/Sun.png");
    auto meshSun = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto progSun = Program::LoadFromFile("../resource/vs.vert", "../resource/fs.frag");

    auto textEarth = Texture2D::LoadFromFile("../resource/Earth.png");
    auto meshEarth = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto progEarth = Program::LoadFromFile("../resource/vs.vert", "../resource/fs.frag");

    auto textMoon = Texture2D::LoadFromFile("../resource/Moon.png");
    auto meshMoon = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto progMoon = Program::LoadFromFile("../resource/vs.vert", "../resource/fs.frag");


    //Do not remove {}, why???
    {
    // text and mesh, shader => garbage collector
    //會在結束時清掉資源
    auto g1 = Protect(textSun);
    auto g2 = Protect(meshSun);
    auto g3 = Protect(progSun);

    auto g1_2 = Protect(textEarth);
    auto g2_2 = Protect(meshEarth);
    auto g3_2 = Protect(progEarth);

    auto g1_3 = Protect(textMoon);
    auto g2_3 = Protect(meshMoon);
    auto g3_3 = Protect(progMoon);

    if(!meshSun.hasNormal()||!meshSun.hasUV()) {
        std::cerr<<"Mesh does not have normal or uv\n";
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float inDegreeSun = 0.0f;
    float outDegreeSun = 0.0f;
    float inDegreeEarth = 0.0f;
    float outDegreeEarth = 0.0f;
    float inDegreeMoon = 0.0f;
    float outDegreeMoon = 0.0f;

    //initialize RGBA
    glm::vec3 object_colorSun{1.0f};
    glm::vec3 object_colorEarth{1.0f};
    glm::vec3 object_colorMoon{1.0f};

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //設定OpenGL渲染視窗的大小，0,0為左下角，對OpenGL來說座標只有-1~1，所以需要經過轉換
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //設定背景顏色
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //深度測試，隱藏被遮住的圖形
        glEnable(GL_DEPTH_TEST);

        /*lookAt(eye, center, up) eye: camaraPosition,
        center: camaraTarget, up: upVector */
        //glm::mat4 create a 4*4 matrix, perspective 透視投影， perspective(float fovy, float aspect, float zNear, float zFar)
        progSun["vp"] = glm::perspective(45/180.0f*3.1415926f, 1200.0f/900.0f, 0.1f, 10000.0f)*
            glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        //glm::rotate(mat4, radian, vec3)
        progSun["model"] = glm::rotate(glm::mat4(1.0f), inDegreeSun*3.1415926f/180.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));;
        //glm::vec3
        progSun["object_color"] = object_colorSun;

        progSun["text"] = 0;
        textSun.bindToChannel(0);
        progSun.use();
        meshSun.draw();

        progEarth["vp"] = glm::perspective(45/180.0f*3.1415926f, 640.0f/480.0f, 0.1f, 10000.0f)*
            glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        // progEarth["model"] = glm::rotate(glm::mat4(1.0f), inDegreeEarth*3.1415926f/180.0f, glm::vec3(0.5, 1, 0));
        progEarth["object_color"] = object_colorEarth;
        // progEarth["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(6*cos(outDegreeEarth*3.1415926f/180.0f), 6*sin(outDegreeEarth*3.1415926f/180.0f), 0));
        progEarth["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(3*cos(outDegreeEarth*3.1415926f/180.0f), 3*sin(outDegreeEarth*3.1415926f/180.0f), 0)) * glm::rotate(glm::mat4(1.0f), inDegreeEarth*3.1415926f/180.0f, glm::vec3(0.5, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f));
        
        progEarth["text"] = 0;
        textEarth.bindToChannel(0);
        progEarth.use();
        meshEarth.draw();

        progMoon["vp"] = glm::perspective(45/180.0f*3.1415926f, 640.0f/480.0f, 0.1f, 10000.0f)*
            glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        // progMoon["model"] = glm::rotate(glm::mat4(1.0f), inDegreeMoon*3.1415926f/180.0f, glm::vec3(0.5, 1, 0));
        progMoon["model"] = (glm::translate(glm::mat4(1.0f), glm::vec3(3*cos(outDegreeEarth*3.1415926f/180.0f), 3*sin(outDegreeEarth*3.1415926f/180.0f), 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(1*cos(outDegreeMoon*3.1415926f/180.0f), 1*sin(outDegreeMoon*3.1415926f/180.0f), 0)) )* glm::rotate(glm::mat4(1.0f), inDegreeMoon*3.1415926f/180.0f, glm::vec3(0.5, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        progMoon["object_color"] = object_colorMoon;

        progMoon["text"] = 0;
        //progMoon["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(10*cos(outDegreeMoon*3.1415926f/180.0f), 10*sin(outDegreeMoon*3.1415926f/180.0f), 0));

        textMoon.bindToChannel(0);
        progMoon.use();
        meshMoon.draw();

        inDegreeSun+= 0.2f;
        inDegreeEarth+= 0.2f;
        inDegreeMoon+= 0.2f;
        outDegreeEarth+= 0.2f;
        outDegreeMoon+= 2.4f;
        if(inDegreeEarth > 360.0){
            inDegreeSun = 0.0;
            inDegreeEarth = 0.0;
            inDegreeMoon = 0.0;
        }if(outDegreeMoon > 360.0){
            outDegreeMoon = 0.0;
        }if(outDegreeEarth > 360.0){
            outDegreeEarth = 0.0;
        }
        
        // std::cout << progMoon["vp"][0][0] << std::endl;
//
        glDisable(GL_DEPTH_TEST);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool new_shader = false;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static int counter = 0;
            // Create a window called "Hello, world!" and append into it.
            ImGui::Begin("Hello, world!");                          
            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("degreeSun", &inDegreeSun, 0.0f, 360.0f);            
            ImGui::SliderFloat("degreeEarth", &inDegreeEarth, 0.0f, 360.0f); 
            // Edit 3 floats representing a color
            ImGui::ColorEdit3("clear color", (float*)&clear_color); 
            // Edit 3 floats representing a color
            ImGui::ColorEdit3("object color", glm::value_ptr(object_colorSun)); 
            ImGui::ColorEdit3("Earrr", glm::value_ptr(object_colorEarth));
            
            // Buttons return true when clicked (most widgets return true when edited/activated)
            if (ImGui::Button("Button"))                           
                counter++;
            //上下兩個物件放在同一行
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Image(reinterpret_cast<ImTextureID>(textSun.id()), ImVec2{128, 128});
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if(ImGui::Button("Reload Shader")) {
                auto new_prog = Program::LoadFromFile("../resource/vs.vert", "../resource/fs.frag");
                // because we would like to switch value of prog
                // we need to release object on our own.
                progSun.release();
                progSun = new_prog;

            }
            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //swap buffers, 儲存每個pixel的顏色並用來繪製
        glfwSwapBuffers(window);
        //set FPS on 60
        glfwSwapInterval(1);

    }


    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
