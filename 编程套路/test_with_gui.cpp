#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

// MuJoCo data structures
mjModel* m = NULL;                  // MuJoCo model
mjData* d = NULL;                   // MuJoCo data
mjvCamera cam;                      // abstract camera
mjvOption opt;                      // visualization options
mjvScene scn;                       // abstract scene
mjrContext con;                     // custom GPU context

GLFWwindow * InitWindow()
{
    if (!glfwInit())
        mju_error("无法初始化 GLFW");

    GLFWwindow* window = glfwCreateWindow(1200, 900, "Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // initialize visualization data structures
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    // create scene and context
    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_150);

    return window;
}

void DrawOneFrame(GLFWwindow* window)
{
    // get framebuffer viewport
    mjrRect viewport = {0, 0, 0, 0};
    glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

    // update scene and render
    mjv_updateScene(m, d, &opt, NULL, &cam, mjCAT_ALL, &scn);
    mjr_render(viewport, &scn, &con);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

// main function
int main(int argc, const char** argv) {
    char error[1000];
    m = mj_loadXML("hello.xml", NULL, error, 1000);
    d = mj_makeData(m);
    GLFWwindow* window = InitWindow();

    // run main loop, target real-time simulation and 60 fps rendering
    while (!glfwWindowShouldClose(window)) {
        // 推进 1/60 秒
        mjtNum simstart = d->time;
        while (d->time - simstart < 1.0/60.0) {
            mj_step(m, d);
        }
        DrawOneFrame(window);
    }

    //free visualization storage
    mjv_freeScene(&scn);
    mjr_freeContext(&con);

    // free MuJoCo model and data
    mj_deleteData(d);
    mj_deleteModel(m);

    return 1;
}
