
#ifndef DICE_H
#define DICE_H
#include <vector>
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct Triangle {
	int i,j,k;
};

class Dice {
private:
	int faces;
	std::vector<glm::vec3> points;
	std::vector<Triangle> triangles;
public:
	Dice();

	Dice(int f);

	int get_faces();

	const std::vector<glm::vec3>& get_points() const;

	const std::vector<Triangle>& get_triangles() const;
};

#endif