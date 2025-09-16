
#ifndef DICE_H
#define DICE_H
#include <vector>


struct Point3D{
	float x, y, z;
};

class Dice {
private:
	int faces;
	std::vector<Point3D> points;
public:
	Dice();

	Dice(int f);

	int getFaces();

	const std::vector<Point3D>& getPoints() const;
};

#endif