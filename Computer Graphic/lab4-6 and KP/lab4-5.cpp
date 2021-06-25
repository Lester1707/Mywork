#include "pch.h"
#include <GL/glew.h>    /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include "world.hpp"

using namespace std;
using namespace glm;

float getTime() {
	return 1.f*clock() / CLOCKS_PER_SEC;
}

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao, vao_l;
    GLuint vbo_p, vbo_l, vbo_n, ebo, vbo_pl, ebo_l;
    Cylinder fig(0.2f, 0.3f, 0.3f, 0.4f);
    GLfloat points[] = {
        -0.01, -0.01, 0.01,
        -0.01f, 0.01f, 0.01,
        0.01f, 0.01f, 0.01,
        0.01f, -0.01f, 0.01,
        -0.01f, -0.01f, -0.01,
        -0.01f, 0.01f, -0.01,
        0.01f, 0.01f, -0.01,
        0.01f, -0.01f, -0.01,
    };
    GLuint indexes[] = {
        0, 1, 2,
        2, 3, 0,
        3, 2, 6,
        6, 7, 3,
        4, 5, 6,
        6, 7, 4,
        0, 1, 5,
        5, 4, 0,
        0, 4, 7,
        7, 3, 0,
        1, 5, 6,
        6, 2, 1
    };
    const char* vertex_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 vp;"
        "layout (location = 1) in vec3 n;"
        "uniform mat4 rotate;"
        "uniform mat4 perspective;"
        "uniform mat4 translate;"
		"uniform float time;"
        "out vec4 normal;"
        "out vec4 point;"
        "void main () {"
        "  normal = rotate * vec4(n, 1.f);"
        "  point = rotate * vec4(vp.x, vp.y*cos(time + vp.y), vp.z, 1.0);"
        "  gl_Position = perspective * translate * point;"
        "}";
    const char* fragment_shader = 
        "#version 410\n"
        "out vec4 frag_colour;"
        "uniform vec3 light_pos;"
        "uniform vec3 cam;"
        "in vec4 normal;"
        "in vec4 point;"
        "void main () {"
        "  float k1 = pow(max(dot(normalize(normal.xyz), normalize(light_pos.xyz - point.xyz)), 0.f), 15.f);"
        "  float k2 = pow(max(dot(-normalize(point.xyz - cam), reflect(normalize(point.xyz - light_pos), normalize(normal.xyz))), 0.f), 5.f)/2;"
        "  frag_colour = vec4(k1 + k2 + 0.1f, k1 + k2 + 0.1f, k1 + k2 + 0.1f, 1.f);"
        "}";
    const char* light_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 vp;"
        "uniform mat4 perspective;"
        "void main () {"
        "  gl_Position = perspective * vec4(vp, 1.f);"
        "}";
    const char* fragment_light_shader =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(1.f, 1.f, 1.f, 0.5);"
        "}";
    GLuint vert_shader, frag_shader, lt_shader, frag_lt_shader;
    GLuint shader_programme, shader_light;
    if (!glfwInit()) {
        cout << "ERROR!!" << endl;
        return 1;
    }
    window = glfwCreateWindow(800, 800, "Hello Triangle", NULL, NULL);
    if (!window) {
        cout << "ERROR!!" << endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (!glewInit()) {
        //cout << "ERROR!!" << endl;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    glGenBuffers(1, &vbo_p);
    glGenBuffers(1, &vbo_l);
    glGenBuffers(1, &vbo_n);
    glGenBuffers(1, &vbo_pl);
    glGenBuffers(1, &ebo_l);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vao_l);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_p);
    glBufferData(GL_ARRAY_BUFFER, fig.flats.size()*sizeof(float), &fig.flats[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glBufferData(GL_ARRAY_BUFFER, fig.normals.size()*sizeof(float), &fig.normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fig.indexes.size() * sizeof(unsigned int), &fig.indexes[0], GL_STATIC_DRAW);

    glBindVertexArray(vao_l);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_l);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);

    lt_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(lt_shader, 1, &light_shader, NULL);
    glCompileShader(lt_shader);

    frag_lt_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_lt_shader, 1, &fragment_light_shader, NULL);
    glCompileShader(frag_lt_shader);

    shader_light = glCreateProgram();
    glAttachShader(shader_light, lt_shader);
    glAttachShader(shader_light, frag_lt_shader);
    glLinkProgram(shader_light);

    vec3 light_pos(0.f, 0.f, 5.f);
    mat4 persp = perspective(radians(45.f), 1.f, 0.1f, 100.f);
    mat4 light_translate = translate(mat4(1.f), light_pos);

    float f_rotate_x = 0.f;
    float f_rotate_z = 0.f;
    float cam_f = 0.f;
    float cam_r = 3.f;

    const float df = 0.02f;
    const float dr = 0.01f;

    unsigned int persp_loc;
    unsigned int rotate_loc = glGetUniformLocation(shader_programme, "rotate");
    unsigned int transl_loc = glGetUniformLocation(shader_programme, "translate");
    unsigned int light_loc = glGetUniformLocation(shader_programme, "light_pos");
    unsigned int cam_loc = glGetUniformLocation(shader_programme, "cam");
	unsigned int time_loc = glGetUniformLocation(shader_programme, "time");
    mat4 transl;
    mat4 rotate_matrix_x(1.f);
    mat4 rotate_matrix_z(1.f);
    vec3 camera_pos(0, 0, cam_r);
	bool animate = false;
	float animate_time = getTime();
	float start_animate = animate_time;
	float frame_rate = 1.f/60.f;
	chrono::high_resolution_clock::time_point frame_time_start;
	chrono::high_resolution_clock::time_point frame_time_end;
	chrono::duration<float> frame_time_need;
    while (!glfwWindowShouldClose(window)) {
		frame_time_start = chrono::high_resolution_clock::now();
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			animate = !animate;
			start_animate = getTime();
			animate_time = start_animate;
		}
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            f_rotate_x += df;
            rotate_matrix_x = rotate(mat4(1.f), f_rotate_x, vec3(1.f, 0.f, 0.f));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            f_rotate_x -= df;
            rotate_matrix_x = rotate(mat4(1.f), f_rotate_x, vec3(1.f, 0.f, 0.f));
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            f_rotate_z -= df;
            rotate_matrix_z = rotate(mat4(1.f), f_rotate_z, vec3(0.f, 0.f, 1.f));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            f_rotate_z += df;
            rotate_matrix_z = rotate(mat4(1.f), f_rotate_z, vec3(0.f, 0.f, 1.f));
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam_r -= dr;
            camera_pos = cam_r * vec3(sin(cam_f), 0.f, cos(cam_f));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam_r += dr;
            camera_pos = cam_r * vec3(sin(cam_f), 0.f, cos(cam_f));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam_f -= df;
            camera_pos = cam_r * vec3(sin(cam_f), 0.f, cos(cam_f));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam_f += df;
            camera_pos = cam_r*vec3(sin(cam_f), 0.f, cos(cam_f));
        }
        if (abs(f_rotate_x) >= 2 * M_PI)
            f_rotate_x = 0.f;
        if (abs(f_rotate_z) >= 2 * M_PI)
            f_rotate_z = 0.f;
        if (abs(cam_f) >= 2 * M_PI)
            cam_f = 0.f;
        transl = lookAt(camera_pos, vec3(0.f, 0.f, 0.f), normalize(cross(normalize(camera_pos), normalize(cross(vec3(0.f, 1.f, 0.f), normalize(camera_pos))))));
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shader_programme);
		glBindVertexArray(vao);
        persp_loc = glGetUniformLocation(shader_programme, "perspective");
		if (animate) {
			animate_time = getTime();
			fig.changeNormals(animate_time - start_animate);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
			glBufferSubData(GL_ARRAY_BUFFER, 0, fig.normals.size() * sizeof(float), &fig.normals[0]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}
		glUniform1f(time_loc, animate_time - start_animate);
        glUniform3f(light_loc, light_pos.x, light_pos.y, light_pos.z);
        glUniform3f(cam_loc, camera_pos.x, camera_pos.y, camera_pos.z);
        glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, value_ptr(rotate_matrix_x*rotate_matrix_z));
        glUniformMatrix4fv(persp_loc, 1, GL_FALSE, value_ptr(persp));
        glUniformMatrix4fv(transl_loc, 1, GL_FALSE, value_ptr(transl));
        
        glDrawElements(GL_TRIANGLES, fig.indexes.size(), GL_UNSIGNED_INT, 0);
        
        glUseProgram(shader_light);
        glBindVertexArray(vao_l);
        
        persp_loc = glGetUniformLocation(shader_light, "perspective");
        
        glUniformMatrix4fv(persp_loc, 1, GL_FALSE, value_ptr(persp*transl*light_translate));
        
        glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
		do
			frame_time_need = chrono::high_resolution_clock::now() - frame_time_start;
		while (frame_time_need.count() < frame_rate);
		cout << "\r        ";
		cout << '\r' << 1.f / frame_time_need.count();
    } 
	cout << endl;
    glfwTerminate();
    return 0;
}