#include "pch.h"
#include <GL/glew.h>    /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/projection.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include "world.hpp"

using namespace std;
using namespace glm;

const float PI = 3.1415926535;

float macheps(void)
{
	float e = 1.0f;

	while (1.0f + e / 2.0f > 1.0f)
		e /= 2.0f;
	return e;
}

const float eps = macheps();

float getTime() {
	return 1.f*clock() / CLOCKS_PER_SEC;
}

void calculateLine(const vector <vec3> &&points, vector <float> &points_out) {
	int n = points.size();
	double c = 0.5;
	unsigned int segs = 300;
	double xa, xb, xc, xd;
	double ya, yb, yc, yd; 
	double za, zb, zc, zd; 
	points_out.push_back(points[0].x);
	points_out.push_back(points[0].y);
	points_out.push_back(points[0].z);

	xa = (1. - c) / 2 * points[0].x
		- 1. / 2 * points[1].x
		+ c / 2 * points[2].x;
	xb = 0.;
	xc = (c - 3.) / 2 * points[0].x
		+ 3. / 2 * points[1].x
		+ -c / 2 * points[2].x;
	xd = points[0].x;
	ya = (1. - c) / 2 * points[0].y
		- 1. / 2 * points[1].y
		+ c / 2 * points[2].y;
	yb = 0.;
	yc = (c - 3.) / 2 * points[0].y
		+ 3. / 2 * points[1].y
		+ -c / 2 * points[2].y;
	yd = points[0].y;
	za = (1. - c) / 2 * points[0].z
		- 1. / 2 * points[1].z
		+ c / 2 * points[2].z;
	zb = 0.;
	zc = (c - 3.) / 2 * points[0].z
		+ 3. / 2 * points[1].z
		+ -c / 2 * points[2].z;
	zd = points[0].z;
	for (unsigned int k = 1; k <= segs; ++k)
	{
		double t = double(k) / segs; 
		double x = xa * t*t*t + xb * t*t + xc * t + xd;
		double y = ya * t*t*t + yb * t*t + yc * t + yd;
		double z = za * t*t*t + zb * t*t + zc * t + zd;
		points_out.push_back(x);
		points_out.push_back(y);
		points_out.push_back(z);
	}

	for (int j = 0; j < n - 3; ++j)
	{
		xa = -c * points[j].x
			+ (2. - c)*points[j + 1].x
			+ (c - 2.)*points[j + 2].x
			+ c * points[j + 3].x;
		xb = 2.*c*points[j].x
			+ (c - 3.)*points[j + 1].x
			+ (3. - 2.*c)*points[j + 2].x
			- c * points[j + 3].x;
		xc = -c * points[j].x
			+ c * points[j + 2].x;
		xd = points[j + 1].x;
		ya = -c * points[j].y
			+ (2. - c)*points[j + 1].y
			+ (c - 2.)*points[j + 2].y
			+ c * points[j + 3].y;
		yb = 2.*c*points[j].y
			+ (c - 3.)*points[j + 1].y
			+ (3. - 2.*c)*points[j + 2].y
			- c * points[j + 3].y;
		yc = -c * points[j].y
			+ c * points[j + 2].y;
		yd = points[j + 1].y;
		za = -c * points[j].z
			+ (2. - c)*points[j + 1].z
			+ (c - 2.)*points[j + 2].z
			+ c * points[j + 3].z;
		zb = 2.*c*points[j].z
			+ (c - 3.)*points[j + 1].z
			+ (3. - 2.*c)*points[j + 2].z
			- c * points[j + 3].z;
		zc = -c * points[j].z
			+ c * points[j + 2].z;
		zd = points[j + 1].z;
		for (unsigned int k = 1; k <= segs; ++k)
		{
			double t = double(k) / segs; 
			double x = xa * t*t*t + xb * t*t + xc * t + xd;
			double y = ya * t*t*t + yb * t*t + yc * t + yd;
			double z = za * t*t*t + zb * t*t + zc * t + zd;
			points_out.push_back(x);
			points_out.push_back(y);
			points_out.push_back(z);
		}
	}

	xa = -c / 2 * points[n - 3].x
		+ 1. / 2 * points[n - 2].x
		+ (c - 1.) / 2 * points[n - 1].x;
	xb = 3.*c / 2 * points[n - 3].x
		- 3. / 2 * points[n - 2].x
		+ (3. - 3.*c) / 2 * points[n - 1].x;
	xc = -c * points[n - 3].x
		+ c * points[n - 1].x;
	xd = points[n - 2].x;
	ya = -c / 2 * points[n - 3].y
		+ 1. / 2 * points[n - 2].y
		+ (c - 1.) / 2 * points[n - 1].y;
	yb = 3.*c / 2 * points[n - 3].y
		- 3. / 2 * points[n - 2].y
		+ (3. - 3.*c) / 2 * points[n - 1].y;
	yc = -c * points[n - 3].y
		+ c * points[n - 1].y;
	yd = points[n - 2].y;
	za = -c / 2 * points[n - 3].z
		+ 1. / 2 * points[n - 2].z
		+ (c - 1.) / 2 * points[n - 1].z;
	zb = 3.*c / 2 * points[n - 3].z
		- 3. / 2 * points[n - 2].z
		+ (3. - 3.*c) / 2 * points[n - 1].z;
	zc = -c * points[n - 3].z
		+ c * points[n - 1].z;
	zd = points[n - 2].z;
	for (unsigned int k = 1; k <= segs; ++k)
	{
		double t = double(k) / segs;
		double x = xa * t*t*t + xb * t*t + xc * t + xd;
		double y = ya * t*t*t + yb * t*t + yc * t + yd;
		double z = za * t*t*t + zb * t*t + zc * t + zd;
		points_out.push_back(x);
		points_out.push_back(y);
		points_out.push_back(z);
	}
}

void generateBorders(vector <vec3> &points, const float &a, const float &b, const float &df) {
	float l;
	float C = (b / a);
	points.push_back({ 0.f, 0.f, 0.f });
	for (float r = 0.01; r <= a; r += 0.01)
		for (float f = 0; f < 2 * PI; f += df) {
			l = C * r / sqrt(pow_2(sin(f)) + pow_2(C*cos(f)));
			points.push_back({ l * cos(f), l * sin(f), 0.f });
		}
}

void generateElps(vector <vec3> &points, vector <vec3> &points_bord, const float &&a, const float &&b, const float &df = 2*PI/300) {
	float l;
	float C = (b / a);
	for (float f = 0; f < 2 * PI; f += df) {
		l = C * a / sqrt(pow_2(sin(f)) + pow_2(C*cos(f)));
		points.push_back({l * cos(f), l * sin(f), 0.f});
	}
	generateBorders(points_bord, a, b, df);
}

void generateElpsIndex(const vector <float> &points, const size_t &elps, vector <unsigned int> &index) {
	for (size_t k = 1; k < elps; k++) {
		index.push_back(0);
		index.push_back(k);
		index.push_back(k + 1);
	}
	index.push_back(0);
	index.push_back(elps);
	index.push_back(1);
	for (size_t q = 0; 6*((q + 1)*elps + 1) < points.size(); q++) {
		for (size_t k = 1; k < elps; k++) {
			index.push_back(k + q * elps);
			index.push_back(k + elps + q * elps);
			index.push_back(k + elps + 1 + q * elps);
			index.push_back(k + elps + 1 + q * elps);
			index.push_back(k + 1 + q * elps);
			index.push_back(k + q * elps);
		}
		index.push_back(elps + q * elps);
		index.push_back(elps + elps + q * elps);
		index.push_back(elps + 1 + q * elps);
		index.push_back(elps + 1 + q * elps);
		index.push_back(1 + q * elps);
		index.push_back(elps + q * elps);
	}
	size_t size = index.size();
	for (size_t i = 0; i < size; i++) {
		index.push_back(index[i] + points.size() / 2 / 3);
	}
}

void calculateElps(
	const vector <float> &center,
	vector <float> &points, const vector <vec3> &elps, vector <float> &out_center, 
	vector <float> &points_bord, const vector <vec3> &elps_bord, vector <float> &out_center_bord)
	{
	vec3 m, n1, n2, x;
	vec4 p;
	float ang_x, ang_n, ang_z;
	n1 = vec3(center[3], center[4], center[5]) - vec3(center[0], center[1], center[2]);
	ang_z = acos(dot(normalize(n1), { 0, 0, 1.f }));
	if (ang_z > eps)
		m = vec3(rotate(mat4(1.f), ang_z, cross(normalize(n1), { 0, 0, -1.f }))*vec4(elps[0], 1.f));
	else
		m = elps[0];
	for (size_t j = 0; j < elps.size(); j++) {
		if (ang_z > eps)
			p = rotate(mat4(1.f), ang_z, cross(normalize(n1), { 0, 0, -1.f }))*vec4(elps[j], 1.f);
		p = translate(mat4(1.f), vec3(center[0], center[1], center[2]))*p;
		points.push_back(p.x);
		points.push_back(p.y);
		points.push_back(p.z);
		out_center.push_back(p.x - center[0]);
		out_center.push_back(p.y - center[1]);
		out_center.push_back(p.z - center[2]);
	}
	for (size_t j = 0; j < elps_bord.size(); j++) {
		if (ang_z > eps)
			p = rotate(mat4(1.f), ang_z, cross(normalize(n1), { 0, 0, -1.f }))*vec4(elps_bord[j], 1.f);
		p = translate(mat4(1.f), vec3(center[0], center[1], center[2]))*p;
		points_bord.push_back(p.x);
		points_bord.push_back(p.y);
		points_bord.push_back(p.z);
		out_center_bord.push_back(-n1.x);
		out_center_bord.push_back(-n1.y);
		out_center_bord.push_back(-n1.z);
	}
	for (size_t i = 0; i + 8 < center.size(); i += 3) {
		n2 = vec3(center[i + 6], center[i + 7], center[i + 8]) - vec3(center[i + 3], center[i + 4], center[i + 5]);
		ang_n = acos(dot(normalize(n1), normalize(n2)));
		if (abs(ang_n) > eps)
			m = vec3(rotate(mat4(1.f), ang_n, cross(normalize(n1), normalize(n2)))*vec4(m, 1.f));
		ang_z = acos(dot(normalize(n2), { 0, 0, 1.f }));
		if (ang_z > eps)
			x = vec3(rotate(mat4(1.f), ang_z, cross(normalize(n2), { 0, 0, -1.f }))*vec4(elps[0], 1.f));
		else
			x = elps[0];
		ang_x = acos(dot(normalize(m), normalize(x)));
		for (size_t j = 0; j < elps.size(); j++) {
			if (ang_z > eps)
				p = rotate(mat4(1.f), ang_z, cross(normalize(n2), { 0, 0, -1.f }))*vec4(elps[j], 1.f);
			if (abs(ang_x) > eps)
				p = rotate(mat4(1.f), ang_x, cross(normalize(x), normalize(m)))*p;
			p = translate(mat4(1.f), vec3(center[i + 3], center[i + 4], center[i + 5]))*p;
			points.push_back(p.x);
			points.push_back(p.y);
			points.push_back(p.z);
			out_center.push_back(p.x - center[i + 3]);
			out_center.push_back(p.y - center[i + 4]);
			out_center.push_back(p.z - center[i + 5]);
		}
		n1 = n2;
	}
	for (size_t j = 0; j < elps.size(); j++) {
		if (ang_z > eps)
			p = rotate(mat4(1.f), ang_z, cross(normalize(n2), { 0, 0, -1.f }))*vec4(elps[j], 1.f);
		if (abs(ang_x) > eps)
			p = rotate(mat4(1.f), ang_x, cross(normalize(x), normalize(m)))*p;
		p = translate(mat4(1.f), vec3(center[center.size() - 3], center[center.size() - 2], center[center.size() - 1]))*p;
		points.push_back(p.x);
		points.push_back(p.y);
		points.push_back(p.z);
		out_center.push_back(p.x - center[center.size() - 3]);
		out_center.push_back(p.y - center[center.size() - 2]);
		out_center.push_back(p.z - center[center.size() - 1]);
	}
	for (size_t j = 0; j < elps_bord.size(); j++) {
		if (ang_z > eps)
			p = rotate(mat4(1.f), ang_z, cross(normalize(n2), { 0, 0, -1.f }))*vec4(elps_bord[j], 1.f);
		if (abs(ang_x) > eps)
			p = rotate(mat4(1.f), ang_x, cross(normalize(x), normalize(m)))*p;
		p = translate(mat4(1.f), vec3(center[center.size() - 3], center[center.size() - 2], center[center.size() - 1]))*p;
		points_bord.push_back(p.x);
		points_bord.push_back(p.y);
		points_bord.push_back(p.z);
		out_center_bord.push_back(n2.x);
		out_center_bord.push_back(n2.y);
		out_center_bord.push_back(n2.z);
	}
}

void generateIndexes(const vector <float> &points, const size_t &elps, vector <unsigned int> &index) {
	for (size_t i = elps; 3*i < points.size(); i += elps) {
		for (size_t j = 0; j + 1 < elps; j++) {
			index.push_back(i - elps + j);
			index.push_back(i + j);
			index.push_back(i + 1 + j);
			index.push_back(i + 1 + j);
			index.push_back(i + 1 - elps + j);
			index.push_back(i - elps + j);
		}
		index.push_back(i - 1);
		index.push_back(i + elps - 1);
		index.push_back(i);
		index.push_back(i);
		index.push_back(i - elps);
		index.push_back(i - 1);
	}
}

void generateColor(const size_t &points_size, const size_t &elps, vector <float> &elps_color) {
	for (size_t i = 0; i < points_size; i++) {
		if (4*i % elps == 0) {
			elps_color.push_back(0.f);
			elps_color.push_back(0.f);
			elps_color.push_back(0.f);
			continue;
		}
		elps_color.push_back(0.3);
		elps_color.push_back(0.6);
		elps_color.push_back(0.9);
	}
	size_t step = points_size / 6 / elps;
	for (size_t i = 0; i < 3*points_size; i += 3*step*elps) {
		for (size_t j = 0; j < 3 * elps; j += 3) {
			elps_color[i + j] = 0.f;
			elps_color[i + j + 1] = 0.f;
			elps_color[i + j + 2] = 0.f;
		}
	}
}

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao, vao_l, vao_b;
    GLuint vbo_p, vbo_l, vbo_n, ebo, vbo_c, ebo_l, vbo_b, ebo_b, vbo_nb;
	vector <float> points_direction;
	vector <float> points_elps;
	vector <float> points_elps_borders;
	vector <vec3> elps_border;
	vector <float> elps_color;
	vector <vec3> elps;
	vector <float> points_center;
	vector <float> points_center_border;
	generateElps(elps, elps_border, 0.3, 0.1);
	vector <unsigned int> index_line;
	vector <unsigned int> index_elps;
	vector <unsigned int> index_elps_border;
	calculateLine({ {1.0, 0.0, 0.0}, {2.0, 2.0, 2.0},{ 2.f, 2.f, 0.0 }, {3.f, 3.f, -1.0 } }, points_direction);
	index_line.push_back(0);
	for (size_t i = 1; 3*(i + 1) < points_direction.size(); i++) {
		index_line.push_back(i);
		index_line.push_back(i);
	}
	index_line.push_back(points_direction.size()/3 - 1);

	calculateElps(points_direction, points_elps, elps, points_center, points_elps_borders, elps_border, points_center_border);
	generateIndexes(points_elps, elps.size(), index_elps);
	generateColor(points_elps.size() / 3, elps.size(), elps_color);
	generateElpsIndex(points_elps_borders, elps.size(), index_elps_border);

    const char* vertex_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 vp;"
        "layout (location = 1) in vec3 n;"
		"layout (location = 2) in vec3 c;"
        "uniform mat4 rotate;"
        "uniform mat4 perspective;"
        "uniform mat4 translate;"
        "out vec4 normal;"
        "out vec4 point;"
		"out vec3 cl;"
        "void main () {"
		"  cl = c;"
        "  normal = rotate * vec4(n, 1.f);"
        "  point = rotate * vec4(vp.x, vp.y, vp.z, 1.0);"
        "  gl_Position = perspective * translate * point;"
        "}";
    const char* fragment_shader = 
        "#version 410\n"
        "out vec4 frag_colour;"
        "uniform vec3 light_pos;"
        "uniform vec3 cam;"
        "in vec4 normal;"
        "in vec4 point;"
		"in vec3 cl;"
        "void main () {"
        //"  float k1 = pow(max(dot(normalize(normal.xyz), normalize(light_pos.xyz - point.xyz)), 0.f), 5.f);"
        "  float k2 = pow(max(dot(-normalize(point.xyz - cam), reflect(normalize(point.xyz - light_pos), normalize(normal.xyz))), 0.f), 50.f)/2;"
        "  frag_colour = vec4(k2 + cl.x, k2 + cl.y, k2 + cl.z, 1.f);"
        "}";
	const char* vertex_shader_bord =
		"#version 410\n"
		"layout (location = 0) in vec3 vp;"
		"layout (location = 1) in vec3 n;"
		"uniform mat4 rotate;"
		"uniform mat4 perspective;"
		"uniform mat4 translate;"
		"out vec4 normal;"
		"out vec4 point;"
		"void main () {"
		"  normal = rotate * vec4(n, 1.f);"
		"  point = rotate * vec4(vp.x, vp.y, vp.z, 1.0);"
		"  gl_Position = perspective * translate * point;"
		"}";
	const char* fragment_shader_bord =
		"#version 410\n"
		"out vec4 frag_colour;"
		"uniform vec3 light_pos;"
		"uniform vec3 cam;"
		"in vec4 normal;"
		"in vec4 point;"
		"void main () {"
		"  float k1 = pow(max(dot(normalize(normal.xyz), normalize(light_pos.xyz - point.xyz)), 0.f), 5.f);"
		"  float k2 = pow(max(dot(-normalize(point.xyz - cam), reflect(normalize(point.xyz - light_pos), normalize(normal.xyz))), 0.f), 50.f)/2;"
		"  frag_colour = vec4(k2 + 0.3f, k2 + 0.6f, k2 + 0.9f, 1.f);"
		"}";
    const char* line_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 vp;"
        "uniform mat4 perspective;"
        "void main () {"
        "  gl_Position = perspective * vec4(vp, 1.f);"
        "}";
    const char* fragment_line_shader =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(1.f, 1.f, 1.f, 0.5);"
        "}";
    GLuint vert_shader, frag_shader, ln_shader, frag_ln_shader, vert_shader_bord, frag_shader_bord;
	GLuint shader_programme, shader_line, shader_bord;
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
    glGenBuffers(1, &vbo_c);
    glGenBuffers(1, &ebo_l);
	glGenBuffers(1, &vbo_b);
	glGenBuffers(1, &vbo_nb);
	glGenBuffers(1, &ebo_b);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vao_l);
	glGenVertexArrays(1, &vao_b);

    glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_p);
	glBufferData(GL_ARRAY_BUFFER, points_elps.size() * sizeof(float), &points_elps[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
	glBufferData(GL_ARRAY_BUFFER, points_center.size() * sizeof(float), &points_center[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_c);
	glBufferData(GL_ARRAY_BUFFER, elps_color.size() * sizeof(float), &elps_color[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_elps.size() * sizeof(unsigned int), &index_elps[0], GL_STATIC_DRAW);

    glBindVertexArray(vao_l);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_l);
	glBufferData(GL_ARRAY_BUFFER, points_direction.size() * sizeof(float), &points_direction[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_l);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_line.size() * sizeof(unsigned int), &index_line[0], GL_STATIC_DRAW);

	glBindVertexArray(vao_b);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_b);
	glBufferData(GL_ARRAY_BUFFER, points_elps_borders.size() * sizeof(float), &points_elps_borders[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_nb);
	glBufferData(GL_ARRAY_BUFFER, points_center_border.size() * sizeof(float), &points_center_border[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_b);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_elps_border.size() * sizeof(unsigned int), &index_elps_border[0], GL_STATIC_DRAW);

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

    ln_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ln_shader, 1, &line_shader, NULL);
    glCompileShader(ln_shader);

    frag_ln_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_ln_shader, 1, &fragment_line_shader, NULL);
    glCompileShader(frag_ln_shader);

    shader_line = glCreateProgram();
    glAttachShader(shader_line, ln_shader);
    glAttachShader(shader_line, frag_ln_shader);
    glLinkProgram(shader_line);

	vert_shader_bord = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader_bord, 1, &vertex_shader_bord, NULL);
	glCompileShader(vert_shader_bord);

	frag_shader_bord = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader_bord, 1, &fragment_shader_bord, NULL);
	glCompileShader(frag_shader_bord);

	shader_bord = glCreateProgram();
	glAttachShader(shader_bord, vert_shader_bord);
	glAttachShader(shader_bord, frag_shader_bord);
	glLinkProgram(shader_bord);

    vec3 light_pos(5.f, 0.f, 0.f);
    mat4 persp = perspective(radians(45.f), 1.f, 0.1f, 100.f);

    float f_rotate_x = 0.f;
    float f_rotate_z = 0.f;
    float cam_f = 0.f;
    float cam_r = 3.f;

    const float df = 0.02f;
    const float dr = 0.05f;

    unsigned int persp_loc;
    unsigned int rotate_loc = glGetUniformLocation(shader_programme, "rotate");
    unsigned int transl_loc = glGetUniformLocation(shader_programme, "translate");
    unsigned int light_loc = glGetUniformLocation(shader_programme, "light_pos");
    unsigned int cam_loc = glGetUniformLocation(shader_programme, "cam");
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
        transl = lookAt(camera_pos, vec3(0.f, 0.f, 0.f), normalize(cross(normalize(camera_pos), normalize(cross(vec3(0.f, 1.f, 0.f), normalize(camera_pos))))));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5, 0.5, 0.5, 0.5);

        glUseProgram(shader_programme);
		glBindVertexArray(vao);
		persp_loc = glGetUniformLocation(shader_programme, "perspective");
        glUniform3f(light_loc, light_pos.x, light_pos.y, light_pos.z);
        glUniform3f(cam_loc, camera_pos.x, camera_pos.y, camera_pos.z);
        glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, value_ptr(rotate_matrix_x*rotate_matrix_z));
        glUniformMatrix4fv(persp_loc, 1, GL_FALSE, value_ptr(persp));
        glUniformMatrix4fv(transl_loc, 1, GL_FALSE, value_ptr(transl));
        glDrawElements(GL_TRIANGLES, index_elps.size(), GL_UNSIGNED_INT, 0);

		glUseProgram(shader_bord);
		glBindVertexArray(vao_b);
		persp_loc = glGetUniformLocation(shader_programme, "perspective");
		glUniform3f(light_loc, light_pos.x, light_pos.y, light_pos.z);
		glUniform3f(cam_loc, camera_pos.x, camera_pos.y, camera_pos.z);
		glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, value_ptr(rotate_matrix_x*rotate_matrix_z));
		glUniformMatrix4fv(persp_loc, 1, GL_FALSE, value_ptr(persp));
		glUniformMatrix4fv(transl_loc, 1, GL_FALSE, value_ptr(transl));
		glDrawElements(GL_TRIANGLES, index_elps_border.size(), GL_UNSIGNED_INT, 0);

		/*
		//glDisable(GL_DEPTH_TEST);
		glUseProgram(shader_line);
		glBindVertexArray(vao_l);

		persp_loc = glGetUniformLocation(shader_line, "perspective");

		glUniformMatrix4fv(persp_loc, 1, GL_FALSE, value_ptr(persp*transl*rotate_matrix_x*rotate_matrix_z));

		glDrawElements(GL_LINE_STRIP, index_line.size(), GL_UNSIGNED_INT, 0);
		glEnable(GL_DEPTH_TEST);
		*/
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