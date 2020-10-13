#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
class obstacle
{
	struct corner {
		Vector location;
		struct corner* neighbours;
		int id;
	};

public:
	obstacle();
	obstacle(Vector newCenter);
	obstacle(Vector newCenter, int xSize, int ySize, int zSize);
	void setCenter(Vector newCenter);
	void setX(int newXSize);
	void setY(int newYSize);
	void setZ(int newZSize);
	Vector setOffset(Vector newOffset);
	Vector addOffset(Vector adder);
	int getXSize();
	~obstacle();

	//Left-Right: x
	//Up-Down: z
	//front-back: y
	Vector downLeftFront;
	Vector downRightFront;
	Vector upLeftFront;
	Vector upRightFront;
	Vector downLeftBack;
	Vector downRightBack;
	Vector upLeftBack;
	Vector upRightBack;
	bool collides(Vector other);
	int nearestFace(Vector other);
	Vector last_other_location;
	bool last_collided = false;


private:
	Vector center;
	int xSize;
	int ySize;
	int zSize;
	Vector center_offset;
	//Middle Points of the 6 faces of the quader(?)
	Vector faces[6]; //0: front, 1: right, 2: back, 3: left, 4: top, 5: bottom
	void recalcCorners();
	void recalcFaces();


};



