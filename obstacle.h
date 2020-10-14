#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
class obstacle
{

public:
	/*
	*Constructors
	*/
	obstacle();
	obstacle(Vector newCenter);
	obstacle(Vector newCenter, int xSize, int ySize, int zSize);
	
	~obstacle();

	/*
	*Setters
	*/
	void setCenter(Vector newCenter);
	void setX(int newXSize);
	void setY(int newYSize);
	void setZ(int newZSize);
	Vector setOffset(Vector newOffset);
	Vector addOffset(Vector adder);
	
	/*
	*Getters
	*/
	int getXSize();
	int getYSize();
	int getZSize();
	Vector getCenter();

	
	/*
	*location of each corner
	*/
	Vector downLeftFront;
	Vector downRightFront;
	Vector upLeftFront;
	Vector upRightFront;
	Vector downLeftBack;
	Vector downRightBack;
	Vector upLeftBack;
	Vector upRightBack;
	
	bool last_collided = false;

	/// <summary>
	/// Checks whether the other object (e.g. the ball) collides with this obstacle
	/// </summary>
	/// <param name="other">The location if the other object</param>
	/// <returns>They collide: true; they don't: false</returns>
	bool collides(Vector other);

	/// <summary>
	/// Calculates which face of this obstacle has been hit
	/// </summary>
	/// <param name="other">The location of the object that hit this obstacle (e.g. the ball)</param>
	/// <returns>0: front, 1: right, 2: back, 3: left, 4: top, 5: bottom</returns>
	int nearestFace(Vector other);
	
	

private:
	Vector center;
	int xSize;
	int ySize;
	int zSize;
	//The offset is used when the obstacle is moving. The actual center ist center+center_offset then.
	Vector center_offset;
	//Middle Points of the 6 faces of the cuboid
	Vector faces[6]; //0: front, 1: right, 2: back, 3: left, 4: top, 5: bottom
	
	/// <summary>
	/// Recalculates the location of the obstacle's corners. To be called when the size or the position changes.
	/// </summary>
	void recalcCorners();

	/// <summary>
	/// Recalculates the location of the obstacle's faces. To be called when the size or the position changes.
	/// </summary>
	void recalcFaces();
};


//Left-Right: x
//front-back: y
//Up-Down: z