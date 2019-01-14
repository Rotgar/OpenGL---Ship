#ifndef SHAPESDRAWING_H
#define SHAPESDRAWING_H
#include <vector>
#include <glm/glm.hpp>

std::vector<glm::vec3> draw_cuboid_v(float x, float y, float z, float length, float width, float height);
std::vector<unsigned int> draw_cuboid_i();
std::vector<float> draw_bottom(glm::vec3 left, glm::vec3 right, glm::vec3 left_up, glm::vec3 bukszpryt, int nr_points);
std::vector<float> draw_deck(glm::vec3 bukszpryt);
std::vector<float> draw_floor(glm::vec3 bukszpryt);
void addIndicesWithPerpLight(std::vector<float>* result, std::vector<glm::vec3>* source, int a, int b, int c, float atx, float aty, float btx, float bty, float ctx, float cty);
void addIndices(std::vector<float>* result, std::vector<glm::vec3>* source, int a, int b, int c, float atx, float aty, float btx, float bty, float ctx, float cty);
std::vector<float> draw_cylinder(float x, float y, float z, float r1, float r2, float r3, int nr_points);
std::vector<float> draw_rotator(float x, float y, float z, float r1, float r2, float width, int nr_points);
std::vector<float> draw_paddles(float x, float y, float z, float r1, float width, float length);
std::vector<float> draw_circle(float x, float y, float z, float r, int nr_points);







#endif
