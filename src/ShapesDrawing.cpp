#define _USE_MATH_DEFINES
#include "ShapesDrawing.h"
#include "Defines.h"
#include <cmath>



std::vector<float> draw_cylinder(float x, float y, float z, float r1, float r2, float r3, int nr_points)
{
	std::vector<glm::vec3> v1, v2, v3;
	std::vector<float> vertices;
	glm::vec3 up_center = glm::vec3(x, y, z);
	glm::vec3 down_center = glm::vec3(x, 0, z);
	for (float i = 0; i < nr_points; ++i)
	{
		float u = i / (float)nr_points,
			nx = x + r1 * cos(u * 2.0f*M_PI),
			nz = z + r1 * sin(u * 2.0f*M_PI);
		v1.push_back(glm::vec3(nx, 0, nz));
	}
	for (float i = 0; i < nr_points; ++i)
	{
		float u = i / (float)nr_points,
			nx = x + r2 * cos(u * 2.0f*M_PI),
			nz = z + r2 * sin(u * 2.0f*M_PI);
		v2.push_back(glm::vec3(nx, y, nz));
	}
	for (float i = 0; i < nr_points; ++i)
	{
		float u = i / (float)nr_points,
			nx = x + r3 * cos(u * 2.0f*M_PI),
			nz = z + r3 * sin(u * 2.0f*M_PI);
		v3.push_back(glm::vec3(nx, y, nz));
	}
	for (int i = 0; i < v1.size(); ++i)
	{
		vertices.push_back(v1[i].x);	//coordinates
		vertices.push_back(v1[i].y);
		vertices.push_back(v1[i].z);
		glm::vec3 tmp = v1[i] - down_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(0.0f);
		vertices.push_back(v1[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v1[(i + 1) % nr_points].y);
		vertices.push_back(v1[(i + 1) % nr_points].z);
		tmp = v1[(i + 1) % nr_points] - down_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		if (i == v1.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(0.0f);
		vertices.push_back(v2[i].x);	//coordinates
		vertices.push_back(v2[i].y);
		vertices.push_back(v2[i].z);
		tmp = v2[i] - up_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(0.95f);
		//2nd part
		vertices.push_back(v1[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v1[(i + 1) % nr_points].y);
		vertices.push_back(v1[(i + 1) % nr_points].z);
		tmp = v1[(i + 1) % nr_points] - down_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		if (i == v1.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(0.0f);
		vertices.push_back(v2[i].x);	//coordinates
		vertices.push_back(v2[i].y);
		vertices.push_back(v2[i].z);
		tmp = v2[i] - up_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(0.95f);
		vertices.push_back(v2[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v2[(i + 1) % nr_points].y);
		vertices.push_back(v2[(i + 1) % nr_points].z);
		tmp = v2[(i + 1) % nr_points] - up_center;	//normal
		normalize(tmp);
		vertices.push_back(tmp.x);
		vertices.push_back(tmp.y);
		vertices.push_back(tmp.z);
		if (i == v1.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(0.95f);
	}
	for (int i = 0; i < v2.size(); ++i)
	{
		vertices.push_back(v2[i].x);	//coordinates
		vertices.push_back(v2[i].y);
		vertices.push_back(v2[i].z);
		vertices.push_back(0.0f);	//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(0.95f);
		vertices.push_back(v2[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v2[(i + 1) % nr_points].y);
		vertices.push_back(v2[(i + 1) % nr_points].z);
		vertices.push_back(0.0f);		//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);
		if (i == v2.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(0.95f);
		vertices.push_back(v3[i].x);	//coordinates
		vertices.push_back(v3[i].y);
		vertices.push_back(v3[i].z);
		vertices.push_back(0.0f);	//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(1.0f);
		//2nd part
		vertices.push_back(v2[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v2[(i + 1) % nr_points].y);
		vertices.push_back(v2[(i + 1) % nr_points].z);
		vertices.push_back(0.0f);	//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);;
		if (i == v2.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(0.95f);
		vertices.push_back(v3[i].x);	//coordinates
		vertices.push_back(v3[i].y);
		vertices.push_back(v3[i].z);
		vertices.push_back(0.0f);	//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);;
		vertices.push_back(i / (float)nr_points);	//texture
		vertices.push_back(1.0f);
		vertices.push_back(v3[(i + 1) % nr_points].x);	//coordinates
		vertices.push_back(v3[(i + 1) % nr_points].y);
		vertices.push_back(v3[(i + 1) % nr_points].z);
		vertices.push_back(0.0f);	//normal
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);;
		if (i == v3.size() - 1)
			vertices.push_back(1.0f);	//texture
		else
			vertices.push_back(((i + 1) % nr_points) / (float)nr_points);	//texture
		vertices.push_back(1.0f);
	}
	return vertices;
}
std::vector<glm::vec3> draw_cuboid_v(float x, float y, float z, float length, float width, float height)
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(x, y, z));
	vertices.push_back(glm::vec3(x + length, y, z));
	vertices.push_back(glm::vec3(x + length, y, z + width));
	vertices.push_back(glm::vec3(x, y, z + width));
	vertices.push_back(glm::vec3(x, y + height, z));
	vertices.push_back(glm::vec3(x + length, y + height, z));
	vertices.push_back(glm::vec3(x + length, y + height, z + width));
	vertices.push_back(glm::vec3(x, y + height, z + width));
	return vertices;
}
std::vector<unsigned int> draw_cuboid_i()
{
	std::vector<unsigned int> indices;
	for (int i = 0; i < 4; ++i)
	{
		indices.push_back(i);
		indices.push_back((i + 1) % 4);
		indices.push_back(i + 4);
		indices.push_back((i + 1) % 4);
		indices.push_back((i + 5) % 4 + 4);
		indices.push_back(i + 4);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(6);
	return indices;
}
//~````````````````````````````````````````````````Main part
std::vector<float> draw_bottom(glm::vec3 left, glm::vec3 right, glm::vec3 left_up, glm::vec3 bukszpryt, int nr_points)
{
	std::vector<glm::vec3> vertices;
	std::vector<float> result;
	left[0] += LENGTH / 12;
	vertices.push_back(left);
	vertices.push_back(right);
	//some maths here - computing the parabola equation 
	//
	float z = left[2];
	float y = left[1];
	float z2 = left_up[2];
	float y2 = left_up[1];
	float a = (y2 - y) / (z2*z2 - 2 * z2*z + z * z);
	float b = -2 * z*a;
	float c = y + z * z*a;
	float step = (z2 - z) / (nr_points * 2);
	float step_in_x = LENGTH / (12 * nr_points * 2);
	float new_x = left[0];
	float new_rx = right[0];
	for (int i = 0; i < nr_points * 2; i++)
	{
		new_x -= step_in_x;
		z += step;
		y = a * z*z + b * z + c;
		glm::vec3 new_vert_left = { new_x, y, z };
		vertices.push_back(new_vert_left);
		glm::vec3 new_vert_right = { new_rx, y, z };
		vertices.push_back(new_vert_right);
	}
	glm::vec3 right_up = { new_rx, left_up[1], left_up[2] };
	vertices.push_back(left_up);
	vertices.push_back(right_up);
	//now we have to generate mirrored part
	new_x = left[0];
	new_rx = right[0];
	z = left[2];
	y = left[1];
	vertices.push_back(left);
	vertices.push_back(right);
	for (int i = 0; i < nr_points * 2; i++)
	{
		new_x -= step_in_x;
		z -= step;
		y = a * z*z + b * z + c;
		glm::vec3 new_vert_left = { new_x, y, z };
		vertices.push_back(new_vert_left);
		glm::vec3 new_vert_right = { new_rx, y, z };
		vertices.push_back(new_vert_right);
	}
	//corrections
	glm::vec3 right_up2 = { new_rx, left_up[1], left_up[2] - 2 * (left_up[2] - left[2]) };
	glm::vec3 left_up2 = { left_up[0], left_up[1], left_up[2] - 2 * (left_up[2] - left[2]) };
	vertices.push_back(left_up2);
	vertices.push_back(right_up2);
	glm::vec3 left_middle = { left_up[0], DECK_LEVEL-DECK_HEIGHT, left[2] };
	glm::vec3 right_middle = { right[0], left_up[1], left[2] };
	vertices.push_back(left_middle);
	vertices.push_back(right_middle);
	//now let's define the bow's shape
	//we'll need 'right' and 'bukszpryt' to define the parabola
	z = left[2];
	y2 = bukszpryt[1];
	float x2 = bukszpryt[0];
	float x1 = right[0];
	float y1 = right[1];
	a = (y2 - y1) / (x2*x2 - 2 * x2*x1 + x1 * x1);
	b = -2 * x1*a;
	c = y1 + x1 * x1*a;
	step_in_x = (x2 - x1) / (nr_points * 2);
	new_x = right[0];
	for (int i = 0; i < nr_points * 2; i++)
	{
		new_x += step_in_x;
		y = a * new_x*new_x + b * new_x + c;
		glm::vec3 new_vert_left = { new_x, y, z };
		vertices.push_back(new_vert_left);
	}
	vertices.push_back(bukszpryt);
	//	vertices.push_back(bukszpryt); 
	//All vertices ready, time to put them together into result with texture coords
	//bukszpryt is at			vertices.size()-1
	//up middle od the stern is vertices.size() - nr_points*2 - 3
	//right bottom is at        vertices.size() - nr_points*4 - 5 and 1 
	float text_x_step = 1.0f / (nr_points * 4 + 3);
	float text_y_step = 1.0f / (nr_points * 4 + 3);
	float text_x = 0.5f;
	float text_y = 0.0f;
	int j = 0;
	for (int i = 0; i < nr_points * 8 + 6; i++)
	{
		if (i == nr_points * 4 + 2 || i == nr_points * 4 + 3)
		{
			text_x = 0.5f;
			text_x_step = abs(text_x_step)*-1.0f;
			text_y = 0.0f;
			j = 0;
			continue;
		}
		if (i & 1)
		{
			addIndices(&result, &vertices, i, i + 1, i + 2, 1.0f, text_y, 0.0f, text_y + text_y_step, 1.0f, text_y + text_y_step);
			text_y += text_y_step;
			//bukszpryt
			addIndices(&result, &vertices, i, i + 2, vertices.size() - 1 - nr_points * 2 + j, 1.0f, text_y - text_y_step, 1.0f, text_y, 0.5f, 0.5f);
			if (j == nr_points * 2)
				continue;
			addIndices(&result, &vertices, i + 2, vertices.size() - 1 - nr_points * 2 + j, vertices.size() - 1 - nr_points * 2 + j + 1, 0.98f, text_y - text_y_step, 0.5f, 0.5f, 0.6f, 0.6f);
			j++;
		}
		else
		{
			addIndices(&result, &vertices, i, i + 1, i + 2, 0.0f, text_y, 1.0f, text_y, 0.0f, text_y + text_y_step);
			text_y += text_y_step;
			//rufa
			addIndices(&result, &vertices, i, vertices.size() - 3 - nr_points * 2, i + 2, text_x, 0.0f, 0.0f, 1.0f, text_x + text_x_step, 0.0f);
			text_x += text_x_step;
		}
	}
	return result;
}
//~~~~~~~~~~~~~~~~~~~~deck
std::vector<float> draw_deck(glm::vec3 bukszpryt)
{
	float proportion = abs(LENGTH) / 100;
	std::vector<float> result;
	std::vector<glm::vec3> vertices;
	glm::vec3 v0 = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, MIDDLE_Z + 0.5*WIDTH };				//0 - left, bottom, front
	vertices.push_back(v0);
	glm::vec3 v1 = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, MIDDLE_Z - 0.5*WIDTH };				//1 - left bottom, back
	vertices.push_back(v1);
	glm::vec3 v2 = { LEFT_X, DECK_LEVEL, MIDDLE_Z + 0.5*WIDTH };								//2 - left, top, front
	vertices.push_back(v2);
	glm::vec3 v3 = { LEFT_X, DECK_LEVEL, MIDDLE_Z - 0.5*WIDTH };								//3 - left, top, back
	vertices.push_back(v3);
	glm::vec3 v4 = { LEFT_X + LENGTH, DECK_LEVEL - DECK_HEIGHT, MIDDLE_Z - 0.5*WIDTH };		//4 - right, bottom, back
	vertices.push_back(v4);
	glm::vec3 v5 = { LEFT_X + LENGTH, DECK_LEVEL - DECK_HEIGHT, MIDDLE_Z + 0.5*WIDTH };		//5 - right, bottom, front
	vertices.push_back(v5);
	glm::vec3 v6 = { LEFT_X + LENGTH, DECK_LEVEL, MIDDLE_Z + 0.5*WIDTH };						//6 - right, top, front
	vertices.push_back(v6);
	glm::vec3 v7 = { LEFT_X + LENGTH, DECK_LEVEL, MIDDLE_Z - 0.5*WIDTH };					//7 - right, top, back
	vertices.push_back(v7);
	float x1 = LEFT_X + LENGTH / 20;
	float z1 = MIDDLE_Z + proportion;
	float z2 = MIDDLE_Z - proportion;
	float y1 = DECK_LEVEL - DECK_HEIGHT;
	glm::vec3 steer1 = { x1, y1, z1 };			//8
	glm::vec3 steer2 = { x1, y1, z2 };			//9
	glm::vec3 steer3 = { x1 - abs(steer1[2] - steer2[2]), y1, z1 };		//10
	glm::vec3 steer4 = { x1 - abs(steer1[2] - steer2[2]), y1, z2 };
	glm::vec3 steer5 = { x1 - abs(steer1[2] - steer2[2]),  y1 - 0.3*(abs(DECK_LEVEL - y1)), z1 };	//12
	glm::vec3 steer6 = { x1 - abs(steer1[2] - steer2[2]),  y1 - 0.3*(abs(DECK_LEVEL - y1)), z2 };
	glm::vec3 steer7 = { x1,  BOTTOM_LEVEL + 0.1*DECK_HEIGHT, z1 };		//14
	glm::vec3 steer8 = { x1,  BOTTOM_LEVEL + 0.1*DECK_HEIGHT, z2 };
	glm::vec3 steer9 = { x1 - abs(steer1[2] - steer2[2]),   BOTTOM_LEVEL + 0.1*DECK_HEIGHT, z1 };//16
	glm::vec3 steer10 = { x1 - abs(steer1[2] - steer2[2]),   BOTTOM_LEVEL + 0.1*DECK_HEIGHT, z2 };
	glm::vec3 steer11 = { x1 - LENGTH / 9,  y1 - 0.5*(abs(DECK_LEVEL - y1)), MIDDLE_Z };	//18
	glm::vec3 steer12 = { x1 - LENGTH / 9, BOTTOM_LEVEL + 0.1*DECK_HEIGHT, MIDDLE_Z };
	vertices.push_back(steer1);		//8		right up front
	vertices.push_back(steer2);		//9		right up back
	vertices.push_back(steer3);		//10	left up front
	vertices.push_back(steer4);		//11	left up back
	vertices.push_back(steer5);		//12	left middle front
	vertices.push_back(steer6);		//13	left middle back
	vertices.push_back(steer7);		//14	right down front
	vertices.push_back(steer8);		//15	right down back
	vertices.push_back(steer9);		//16	left down front
	vertices.push_back(steer10);	//17	left down back
	vertices.push_back(steer11);	//18	up
	vertices.push_back(steer12);	//19	down
	glm::vec3 v8 = { LEFT_X + DECK_EDGE, DECK, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };	//20 - left, bottom, front edge
	vertices.push_back(v8);
	glm::vec3 v9 = { LEFT_X + DECK_EDGE, DECK, MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };	//21 - left bottom, back edge
	vertices.push_back(v9);
	glm::vec3 v10 = { LEFT_X + DECK_EDGE, DECK_LEVEL, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };								//22 - left, top, front edge
	vertices.push_back(v10);
	glm::vec3 v11 = { LEFT_X + DECK_EDGE, DECK_LEVEL, MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };							//23 - left, top, back edge
	vertices.push_back(v11);
	glm::vec3 v12 = { LEFT_X + LENGTH, DECK , MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };		//24 - right, bottom, back edge
	vertices.push_back(v12);
	glm::vec3 v13 = { LEFT_X + LENGTH, DECK, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };		//25 - right, bottom, front edge
	vertices.push_back(v13);
	glm::vec3 v14 = { LEFT_X + LENGTH, DECK_LEVEL, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };								//26 - right, top, front edge
	vertices.push_back(v14);
	glm::vec3 v15 = { LEFT_X + LENGTH, DECK_LEVEL, MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };								//27 - right, top, back edge
	vertices.push_back(v15);
	vertices.push_back(bukszpryt); //28
	glm::vec3 v16 = { bukszpryt[0] - 0.1*LENGTH, DECK, bukszpryt[2] };
	vertices.push_back(v16);	//29
	glm::vec3 v17 = { bukszpryt[0] - 4 * DECK_EDGE, bukszpryt[1], bukszpryt[2] };
	vertices.push_back(v17);	//30
								//vertices ready, time to put them together
	addIndices(&result, &vertices, 0, 1, 2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);	//left
	addIndices(&result, &vertices, 1, 2, 3, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	addIndices(&result, &vertices, 0, 2, 5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//front
	addIndices(&result, &vertices, 2, 5, 6, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 0, 1, 4, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);	//bottom
	addIndices(&result, &vertices, 0, 4, 5, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	addIndices(&result, &vertices, 1, 3, 4, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);//back
	addIndices(&result, &vertices, 3, 4, 7, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//bukszpryt
	addIndices(&result, &vertices, 28, 5, 6, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//sides
	addIndices(&result, &vertices, 28, 4, 7, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 7, 27, 28, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);	//top back edge
	addIndices(&result, &vertices, 27, 28, 30, 1.0f, 0.0, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 6, 26, 28, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f); //top front edge
	addIndices(&result, &vertices, 26, 28, 30, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 24, 27, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//back side edge
	addIndices(&result, &vertices, 27, 29, 30, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 25, 26, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//front side edge
	addIndices(&result, &vertices, 26, 29, 30, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 24, 25, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);	//inside
	addIndices(&result, &vertices, 4, 5, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 4, 24, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 5, 25, 29, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	//steer
	addIndices(&result, &vertices, 8, 9, 10, 0.4f, 0.5f, 0.5f, 0.5f, 0.5f, 0.6f);	//steer up
	addIndices(&result, &vertices, 9, 10, 11, 0.5f, 0.5f, 0.5f, 0.6f, 0.4f, 0.6f);
	addIndices(&result, &vertices, 8, 9, 14, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//steer right side
	addIndices(&result, &vertices, 9, 14, 15, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 9, 11, 15, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//steer back
	addIndices(&result, &vertices, 11, 15, 17, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 10, 11, 16, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//steer left side
	addIndices(&result, &vertices, 11, 16, 17, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 8, 10, 16, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//steer front
	addIndices(&result, &vertices, 8, 14, 16, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 14, 15, 16, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);	//steer down
	addIndices(&result, &vertices, 15, 16, 17, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 19, 16, 17, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 18, 12, 13, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 18, 19, 12, 0.0f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 18, 19, 13, 0.0f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 19, 16, 12, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 19, 17, 13, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	//edges
	addIndices(&result, &vertices, 3, 7, 23, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//top back
	addIndices(&result, &vertices, 7, 23, 27, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 3, 23, 2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//top left
	addIndices(&result, &vertices, 2, 23, 22, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 2, 22, 6, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//top front
	addIndices(&result, &vertices, 6, 22, 26, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 23, 21, 27, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//back edge side
	addIndices(&result, &vertices, 21, 27, 24, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 23, 21, 22, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//left edge side
	addIndices(&result, &vertices, 21, 22, 20, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 20, 22, 25, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//front edge side
	addIndices(&result, &vertices, 22, 25, 26, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndices(&result, &vertices, 20, 21, 24, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);	//bottom
	addIndices(&result, &vertices, 20, 24, 25, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	return result;
}
std::vector<float> draw_floor(glm::vec3 bukszpryt)
{
	std::vector<glm::vec3> vertices;
	std::vector<float> result;
	glm::vec3 v0 = { LEFT_X + DECK_EDGE, DECK + FLOOR_HEIGHT, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };	//0 - left, front
	vertices.push_back(v0);
	glm::vec3 v1 = { LEFT_X + DECK_EDGE, DECK + FLOOR_HEIGHT, MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };	//1 - left, back 
	vertices.push_back(v1);
	glm::vec3 v2 = { LEFT_X + LENGTH, DECK + FLOOR_HEIGHT, MIDDLE_Z + 0.5*WIDTH - DECK_EDGE };	//2 - right, front
	vertices.push_back(v2);
	glm::vec3 v3 = { LEFT_X + LENGTH, DECK + FLOOR_HEIGHT, MIDDLE_Z - 0.5*WIDTH + DECK_EDGE };	//3 - right, back 
	vertices.push_back(v3);
	vertices.push_back(bukszpryt);
	vertices.push_back(glm::vec3(bukszpryt[0] - 0.1f*LENGTH, DECK + FLOOR_HEIGHT, bukszpryt[2]));
	addIndicesWithPerpLight(&result, &vertices, 0, 1, 2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	addIndicesWithPerpLight(&result, &vertices, 1, 2, 3, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	addIndicesWithPerpLight(&result, &vertices, 5, 2, 3, 0.8f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f);
	return result;
}
void addIndicesWithPerpLight(std::vector<float>* result, std::vector<glm::vec3>* source, int a, int b, int c, float atx, float aty, float btx, float bty, float ctx, float cty)
{
	result->push_back((*source)[a][0]);
	result->push_back((*source)[a][1]);
	result->push_back((*source)[a][2]);
	result->push_back(0.0f);
	result->push_back(1.0f);
	result->push_back(0.0f);
	result->push_back(atx);
	result->push_back(aty);
	result->push_back((*source)[b][0]);
	result->push_back((*source)[b][1]);
	result->push_back((*source)[b][2]);
	result->push_back(0.0f);
	result->push_back(1.0f);
	result->push_back(0.0f);
	result->push_back(btx);
	result->push_back(bty);
	result->push_back((*source)[c][0]);
	result->push_back((*source)[c][1]);
	result->push_back((*source)[c][2]);
	result->push_back(0.0f);
	result->push_back(1.0f);
	result->push_back(0.0f);
	result->push_back(ctx);
	result->push_back(cty);
}
void addIndices(std::vector<float>* result, std::vector<glm::vec3>* source, int a, int b, int c, float atx, float aty, float btx, float bty, float ctx, float cty)
{
	result->push_back((*source)[a][0]);
	result->push_back((*source)[a][1]);
	result->push_back((*source)[a][2]);
	result->push_back(atx);
	result->push_back(aty);
	result->push_back((*source)[b][0]);
	result->push_back((*source)[b][1]);
	result->push_back((*source)[b][2]);
	result->push_back(btx);
	result->push_back(bty);
	result->push_back((*source)[c][0]);
	result->push_back((*source)[c][1]);
	result->push_back((*source)[c][2]);
	result->push_back(ctx);
	result->push_back(cty);
}
//rotator
std::vector<float> draw_rotator(float x, float y, float z, float r1, float r2, float width, int nr_points)
{
	std::vector<glm::vec3> v1, v2, v3, v4;
	std::vector<float> vertices;
	glm::vec3 front(x, y, z);
	glm::vec3 back(x, y, z + width);
	float x1 = r2 - r1, x2 = (-1)*width;
	float x3 = 2 * x1 + x2;
	x1 /= x3, x2 /= x3;
	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r1 * cos(u * 2.0f*M_PI),
			ny = y + r1 * sin(u * 2.0f*M_PI);
		v1.push_back(glm::vec3(nx, ny, z));
	}
	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r2 * cos(u * 2.0f*M_PI),
			ny = y + r2 * sin(u * 2.0f*M_PI);
		v2.push_back(glm::vec3(nx, ny, z));
	}
	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r1 * cos(u * 2.0f*M_PI),
			ny = y + r1 * sin(u * 2.0f*M_PI);
		v3.push_back(glm::vec3(nx, ny, z + width));
	}
	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r2 * cos(u * 2.0f*M_PI),
			ny = y + r2 * sin(u * 2.0f*M_PI);
		v4.push_back(glm::vec3(nx, ny, z + width));
	}
	for (int i = 0; i < v1.size() - 1; ++i)
	{
		//coordinates
		vertices.push_back(v1[i].x);	vertices.push_back(v1[i].y);	vertices.push_back(v1[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v1[(i + 1)].x);	vertices.push_back(v1[(i + 1)].y);	vertices.push_back(v1[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v2[i].x);	vertices.push_back(v2[i].y);	vertices.push_back(v2[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1);
		//2nd part
		//coordinates
		vertices.push_back(v1[(i + 1)].x);	vertices.push_back(v1[(i + 1)].y);	vertices.push_back(v1[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v2[i].x);	vertices.push_back(v2[i].y);	vertices.push_back(v2[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1);
		//coordinates
		vertices.push_back(v2[(i + 1)].x);	vertices.push_back(v2[(i + 1)].y);	vertices.push_back(v2[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(x1);
	}
	for (int i = 0; i < v3.size() - 1; ++i)
	{
		//coordinates
		vertices.push_back(v3[i].x);	vertices.push_back(v3[i].y);	vertices.push_back(v3[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(1.0f);
		//coordinates
		vertices.push_back(v3[(i + 1)].x);	vertices.push_back(v3[(i + 1)].y);	vertices.push_back(v3[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(1.0f);
		//coordinates
		vertices.push_back(v4[i].x);	vertices.push_back(v4[i].y);	vertices.push_back(v4[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1 + x2);
		//2nd part
		//coordinates
		vertices.push_back(v3[(i + 1)].x);	vertices.push_back(v3[(i + 1)].y);	vertices.push_back(v3[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(1.0f);
		//coordinates
		vertices.push_back(v4[i].x);	vertices.push_back(v4[i].y);	vertices.push_back(v4[i].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1 + x2);
		//coordinates
		vertices.push_back(v4[(i + 1)].x);	vertices.push_back(v4[(i + 1)].y);	vertices.push_back(v4[(i + 1)].z);
		//normals
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(-1.0f);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(x1 + x2);
	}
	for (int i = 0; i < v1.size() - 1; ++i)
	{
		//coordinates
		vertices.push_back(v1[i].x);	vertices.push_back(v1[i].y);	vertices.push_back(v1[i].z);
		//normals
		glm::vec3 tmp = front - v1[i];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v1[(i + 1)].x);	vertices.push_back(v1[(i + 1)].y);	vertices.push_back(v1[(i + 1)].z);
		//normals
		tmp = front - v1[i + 1];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v3[i].x);	vertices.push_back(v3[i].y);	vertices.push_back(v3[i].z);
		//normals
		tmp = back - v3[i];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(1.0f);
		//2nd part
		//coordinates
		vertices.push_back(v1[(i + 1)].x);	vertices.push_back(v1[(i + 1)].y);	vertices.push_back(v1[(i + 1)].z);
		//normals
		tmp = front - v1[i + 1];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(0.0f);
		//coordinates
		vertices.push_back(v3[i].x);	vertices.push_back(v3[i].y);	vertices.push_back(v3[i].z);
		//normals
		tmp = back - v3[i];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(1.0f);
		//coordinates
		vertices.push_back(v3[(i + 1)].x);	vertices.push_back(v3[(i + 1)].y);	vertices.push_back(v3[(i + 1)].z);
		//normals
		tmp = back - v3[i + 1];
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(1.0f);
	}
	for (int i = 0; i < v2.size() - 1; ++i)
	{
		//coordinates
		vertices.push_back(v2[i].x);	vertices.push_back(v2[i].y);	vertices.push_back(v2[i].z);
		//normals
		glm::vec3 tmp = v2[i] - front;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1);
		//coordinates
		vertices.push_back(v2[(i + 1)].x);	vertices.push_back(v2[(i + 1)].y);	vertices.push_back(v2[(i + 1)].z);
		//normals
		tmp = v2[i + 1] - front;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(x1);
		//coordinates
		vertices.push_back(v4[i].x);	vertices.push_back(v4[i].y);	vertices.push_back(v4[i].z);
		//normals
		tmp = v4[i] - back;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1 + x2);
		//2nd part
		//coordinates
		vertices.push_back(v2[(i + 1)].x);	vertices.push_back(v2[(i + 1)].y);	vertices.push_back(v2[(i + 1)].z);
		//normals
		tmp = v2[i + 1] - front;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(x1);
		//coordinates
		vertices.push_back(v4[i].x);	vertices.push_back(v4[i].y);	vertices.push_back(v4[i].z);
		//normals
		tmp = v4[i] - back;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(i / (float)nr_points);	vertices.push_back(x1 + x2);
		//coordinates
		vertices.push_back(v4[(i + 1)].x);	vertices.push_back(v4[(i + 1)].y);	vertices.push_back(v4[(i + 1)].z);
		//normals
		tmp = v4[i + 1] - back;
		normalize(tmp);
		vertices.push_back(tmp.x);	vertices.push_back(tmp.y);	vertices.push_back(tmp.z);
		//texture
		vertices.push_back(((i + 1)) / (float)nr_points);	vertices.push_back(x1 + x2);
	}
	{
		vertices.push_back(v1[0].x);	vertices.push_back(v1[0].y);	vertices.push_back(v1[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(0.0f);		vertices.push_back(0.0f);
		vertices.push_back(v2[0].x);	vertices.push_back(v2[0].y);	vertices.push_back(v2[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(1.0f);		vertices.push_back(0.0f);
		vertices.push_back(v4[0].x);	vertices.push_back(v4[0].y);	vertices.push_back(v4[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(1.0f);		vertices.push_back(1.0f);
		vertices.push_back(v1[0].x);	vertices.push_back(v1[0].y);	vertices.push_back(v1[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(0.0f);		vertices.push_back(0.0f);
		vertices.push_back(v3[0].x);	vertices.push_back(v3[0].y);	vertices.push_back(v3[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(0.0f);		vertices.push_back(1.0f);
		vertices.push_back(v4[0].x);	vertices.push_back(v4[0].y);	vertices.push_back(v4[0].z);
		vertices.push_back(0.0f);		vertices.push_back(-1.0f);		vertices.push_back(0.0f);
		vertices.push_back(1.0f);		vertices.push_back(1.0f);
		int size = v1.size();
		vertices.push_back(v1[size - 1].x);	vertices.push_back(v1[size - 1].y);	vertices.push_back(v1[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(0.0f);			vertices.push_back(0.0f);
		vertices.push_back(v2[size - 1].x);	vertices.push_back(v2[size - 1].y);	vertices.push_back(v2[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(1.0f);			vertices.push_back(0.0f);
		vertices.push_back(v4[size - 1].x);	vertices.push_back(v4[size - 1].y);	vertices.push_back(v4[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(1.0f);			vertices.push_back(1.0f);
		vertices.push_back(v1[size - 1].x);	vertices.push_back(v1[size - 1].y);	vertices.push_back(v1[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(0.0f);			vertices.push_back(0.0f);
		vertices.push_back(v3[size - 1].x);	vertices.push_back(v3[size - 1].y);	vertices.push_back(v3[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(0.0f);			vertices.push_back(1.0f);
		vertices.push_back(v4[size - 1].x);	vertices.push_back(v4[size - 1].y);	vertices.push_back(v4[size - 1].z);
		vertices.push_back(0.0f);			vertices.push_back(-1.0f);			vertices.push_back(0.0f);
		vertices.push_back(1.0f);			vertices.push_back(1.0f);
	}
	return vertices;
}
std::vector<float> draw_paddles(float x, float y, float z, float r1, float width, float length)
{
	std::vector<float> vertices;
	float height = r1 - 0.05f;
	//z -= 0.001f;
	//przednia
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//0
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//1
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//2
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//1
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//3
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//2
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//tylna
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//4
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//5
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//6
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//5
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//7
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//6
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//prawo
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//1
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//5
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//3
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//5
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//7
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//3
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//lewo
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//0
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//4
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//2
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//4
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//6
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//2
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//gora
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//2
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//3
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//6
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z);			//3
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//7
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y + height);	vertices.push_back(z + length);	//6
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//dol
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//0
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//1
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//4
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z);			//1
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//5
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - width / 2);	vertices.push_back(y - height);	vertices.push_back(z + length);	//4
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	// drugi
	//przod
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//8
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//9
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//10
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//9
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//11
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//10
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//tyl
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//12
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//13
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//14
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//13
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//15
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//14
	vertices.push_back(0.0f); vertices.push_back(0.0f);	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//prawo
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//10
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//11
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//14
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//11
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//15
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//14
	vertices.push_back(1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//lewo
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//8
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//9
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//12
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//9
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//13
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//12
	vertices.push_back(-1.0f); vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	//gora
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//8
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//10
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//12
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z);			//10
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//14
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y + width / 2);	vertices.push_back(z + length);	//12
	vertices.push_back(0.0f); vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	//dol
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//9
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//11
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//13
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z);			//11
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(1.0f);	vertices.push_back(1.0f);
	vertices.push_back(x + height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//15
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(1.0f);
	vertices.push_back(x - height);	vertices.push_back(y - width / 2);	vertices.push_back(z + length);	//13
	vertices.push_back(0.0f); vertices.push_back(-1.0f);	vertices.push_back(0.0f);
	vertices.push_back(0.0f);	vertices.push_back(0.0f);
	return vertices;
}
std::vector<float> draw_circle(float x, float y, float z, float r, int nr_points)
{
	std::vector<glm::vec3> v1;
	std::vector<float> vertices;
	for (float i = 0; i < nr_points; ++i)
	{
		float u = i / (float)nr_points,
			nx = x + r * cos(u * 2.0f*M_PI),
			ny = y + r * sin(u * 2.0f*M_PI);
		v1.push_back(glm::vec3(nx, ny, z));
	}
	float rc = 0.5f;
	for (int i = 0; i < v1.size(); ++i)
	{
		vertices.push_back(x);	vertices.push_back(y);	vertices.push_back(z);
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		vertices.push_back(rc);	vertices.push_back(rc);
		float u = i / (float)nr_points;
		vertices.push_back(v1[i].x);	vertices.push_back(v1[i].y);	vertices.push_back(v1[i].z);
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		vertices.push_back(rc + rc * cos(u * 2.0f*M_PI));	vertices.push_back(rc + rc * sin(u * 2.0f*M_PI));
		u = (i + 1) / (float)nr_points;
		vertices.push_back(v1[(i + 1) % nr_points].x);	vertices.push_back(v1[(i + 1) % nr_points].y);	vertices.push_back(v1[(i + 1) % nr_points].z);
		vertices.push_back(0.0f);	vertices.push_back(0.0f);	vertices.push_back(1.0f);
		vertices.push_back(rc + rc * cos(u * 2.0f*M_PI));	vertices.push_back(rc + rc * sin(u * 2.0f*M_PI));
	}
	return vertices;
}
