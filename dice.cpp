using namespace std;
#include "dice.h"
#include <cmath>
#include <stdexcept>

Dice::Dice(){
	faces = 6;
	points = {
		{-0.5f,-0.5f,-0.5f},
		{0.5f,-0.5f,-0.5f},
		{-0.5f,0.5f,-0.5f},
		{-0.5f,-0.5f,0.5f},
		{0.5f,0.5f,-0.5f},
		{0.5f,-0.5f,0.5f},
		{-0.5f,0.5f,0.5f},
		{0.5f,0.5f,0.5f}
	};
}

Dice::Dice(int f){
	faces = f;
    /*
    In this constructor we determine how many faces the dice will have based on f
    we then use this to define the points
    and we also define the triangles (the indices of points that make up each face)*/
    switch (f) {
        case 4:  // d4
            points = {
                { 0.0f, 1.0f, 0.0f },
                {  sqrt(3.0f)/2.0f, -0.5f, 0.0f },
                {  -sqrt(3.0f)/2.0f, -0.5f, 0.0f },
                {  0.0f, 0.0f, 1.41f }
            };
            triangles = {
                {0, 1, 2},
                {1, 2, 3},
                {0, 2, 3},
                {0, 1, 3}
            };
            break;
        case 6:  // d6
        	faces = 6;
			points = {
				{-0.5f,-0.5f,-0.5f},
				{0.5f,-0.5f,-0.5f},
				{-0.5f,0.5f,-0.5f},
				{-0.5f,-0.5f,0.5f},
				{0.5f,0.5f,-0.5f},
				{0.5f,-0.5f,0.5f},
				{-0.5f,0.5f,0.5f},
				{0.5f,0.5f,0.5f}
			};
            triangles = {
                // Front face (+Z)
                {3, 5, 7},
                {3, 7, 6},
                // Back face (-Z)
                {0, 2, 4},
                {0, 4, 1},
                // Left face (-X)
                {0, 3, 6},
                {0, 6, 2},
                // Right face (+X)
                {1, 4, 7},
                {1, 7, 5},
                // Top face (+Y)
                {2, 6, 7},
                {2, 7, 4},
                // Bottom face (-Y)
                {0, 1, 5},
                {0, 5, 3}
            };
			break;
		case 8:  // d8
            points = {
                { 0.211325f, -0.788675f,  0.000000f }, // v0
                {-0.211325f,  0.788675f,  1.154701f }, // v1
                {-0.788675f,  0.211325f,  0.000000f }, // v2
                { 0.788675f, -0.211325f,  1.154701f }, // v3
                { 0.577350f,  0.577350f,  0.000000f }, // v4
                {-0.577350f, -0.577350f,  1.154701f }  // v5
            };
            
            
            
            triangles = {
                {0, 2, 4}, {2, 1, 4}, {1, 3, 4}, {3, 0, 4}, // top half
                {0, 5, 2}, {2, 5, 1}, {1, 5, 3}, {3, 5, 0}  // bottom half
            };
            
            break;

        case 10:  // d10
            points = {
                { -0.95f, -0.649f, 1.155f },
                {  0.0f, -1.155f, 1.153f },
                {  0.95f, -0.649f, 1.155f },
                {  1.54f, -0.465f, 0.271f },
                {  1.54f, 0.465f, -0.271f },
                {  0.95f, 0.649f, -1.155f },
                {  0.0f, 1.149f, -1.146f },
                { -0.95f, 0.649f, -1.155f },
                { -1.54f, 0.465f, -0.271f },
                { -1.54f, -0.465f, 0.271f },
                {  0.0f, 1.489f, 1.164f },
                {  0.0f, -1.489f, -1.164f }
            };
            break;

        case 12:  // d12
            points = {
                { -0.5f, -0.77f, 0.0f },
                {  0.5f, -0.77f, 0.0f },
                {  0.81f, 0.18f, 0.0f },
                {  0.0f, 0.77f, 0.0f },
                { -0.81f, 0.18f, 0.0f },
                { -0.81f, -1.2f, 0.85f },
                {  0.0f, -1.46f, 1.38f },
                {  0.81f, -1.19f, 0.85f },
                {  1.31f, -0.51f, 1.38f },
                {  1.31f, 0.34f, 0.85f },
                {  0.81f, 1.03f, 1.38f },
                {  0.0f, 1.29f, 0.85f },
                { -0.81f, 1.03f, 1.38f },
                { -1.31f, 0.34f, 0.85f },
                { -1.31f, -0.51f, 1.38f },
                {  0.0f, -0.93f, 2.23f },
                {  0.81f, -0.34f, 2.23f },
                {  0.5f, 0.61f, 2.23f },
                { -0.5f, 0.61f, 2.23f },
                { -0.81f, -0.35f, 2.23f }
            };
            break;

        case 20:  // d20
            points = {
                { 0.5f, 0.43f, 0.0f },
                { -0.5f, 0.43f, 0.0f },
                { 0.0f, -0.43f, 0.0f },
                { 0.0f, 1.08f, 0.58f },
                { -0.81f, -0.32f, 0.58f },
                { 0.81f, -0.32f, 0.58f },
                { 0.81f, 0.61f, 0.93f },
                { -0.81f, 0.61f, 0.93f },
                { 0.0f, -0.79f, 0.93f },
                { 0.0f, 0.72f, 1.51f },
                { -0.5f, -0.14f, 1.51f },
                { 0.5f, -0.14f, 1.51f }
            };
            break;

        default:
            throw invalid_argument("Unsupported dice type");
    }
}

const vector<glm::vec3>& Dice::get_points() const {
    return points;
}

const vector<Triangle>& Dice::get_triangles() const {
    return triangles;
}   
