#ifndef BUFFERINIT_H
#define BUFFERINIT_H
#include <vector>
#include <glm/glm.hpp>
void prepareBottom(std::vector<float> bottom, unsigned& VAO, unsigned& VBO);
void prepareDeck(std::vector<float> deck, unsigned& VAO, unsigned& VBO);
void prepareLightedShape(std::vector<float> floor, unsigned& VAO, unsigned& VBO);
void prepareCuboid(std::vector<glm::vec3> vertices_c1, std::vector<unsigned int> indices_c1, unsigned& VAO, unsigned& VBO, unsigned& EBO);


#endif