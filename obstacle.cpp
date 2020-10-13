#include "obstacle.h"

obstacle::obstacle()
{

	this->center = Vector(0, 0, 0);
	this->xSize = 0;
	this->ySize = 0;
	this->zSize = 0;

	this->recalcCorners();
	recalcFaces();

}

obstacle::obstacle(Vector newCenter)
{
	this->center = newCenter;
	this->xSize = 0;
	this->ySize = 0;
	this->zSize = 0;
	this->recalcCorners();
	recalcFaces();

}

obstacle::obstacle(Vector newCenter, int xSize, int ySize, int zSize)
{
	this->center = newCenter;
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
	this->recalcCorners();
	recalcFaces();
}

void obstacle::setCenter(Vector newCenter)
{
	this->center = newCenter;
	this->recalcCorners();
	recalcFaces();
}

void obstacle::setX(int newXSize)
{
	this->xSize = newXSize;
	this->recalcCorners();
	recalcFaces();
}


void obstacle::setY(int newYSize)
{
	this->ySize = newYSize;
	this->recalcCorners();
	recalcFaces();
}

void obstacle::setZ(int newZSize)
{
	this->zSize = newZSize;
	this->recalcCorners();
	recalcFaces();
}

Vector obstacle::setOffset(Vector newOffset)
{
	this->center_offset = newOffset;
	recalcCorners();
	recalcFaces();
	return this->center_offset;
}

Vector obstacle::addOffset(Vector adder)
{
	return setOffset(center_offset + adder);
	
}

int obstacle::getXSize()
{
	return xSize;
}

obstacle::~obstacle()
{
}

void obstacle::recalcCorners()
{
	Vector actualCenter = center + center_offset;
	this->downLeftFront = Vector(actualCenter.X - xSize / 2, actualCenter.Y - ySize / 2, actualCenter.Z - zSize / 2);
	this->downRightFront = Vector(actualCenter.X + xSize / 2, actualCenter.Y - ySize / 2, actualCenter.Z - zSize / 2);
	this->upLeftFront = Vector(actualCenter.X - xSize / 2, actualCenter.Y - ySize / 2, actualCenter.Z + zSize / 2);
	this->upRightFront = Vector(actualCenter.X + xSize / 2, actualCenter.Y - ySize / 2, actualCenter.Z + zSize / 2);
	this->downLeftBack = Vector(actualCenter.X - xSize / 2, actualCenter.Y + ySize / 2, actualCenter.Z - zSize / 2);
	this->downRightBack = Vector(actualCenter.X + xSize / 2, actualCenter.Y + ySize / 2, actualCenter.Z - zSize / 2);
	this->upLeftBack = Vector(actualCenter.X - xSize / 2, actualCenter.Y + ySize / 2, actualCenter.Z + zSize / 2);
	this->upRightBack = Vector(actualCenter.X + xSize / 2, actualCenter.Y + ySize / 2, actualCenter.Z + zSize / 2);
}

void obstacle::recalcFaces()
{
	Vector actualCenter = center + center_offset;
	faces[0] = actualCenter - Vector(0, ySize / 2, 0);
	faces[1] = actualCenter + Vector(xSize / 2, 0, 0);
	faces[2] = actualCenter + Vector(0, ySize / 2, 0);
	faces[3] = actualCenter - Vector(xSize / 2, 0, 0);
	faces[4] = actualCenter + Vector(0, 0, ySize);
	faces[5] = actualCenter - Vector(0, 0, ySize);

}

bool obstacle::collides(Vector other) {
	
	Vector actualCenter = center + center_offset;
	if (other.X >= actualCenter.X - xSize / 2 && other.X <= actualCenter.X + xSize / 2
		&& other.Y >= actualCenter.Y - ySize / 2 && other.Y <= actualCenter.Y + ySize / 2
		&& other.Z >= actualCenter.Z - zSize / 2 && other.Z <= actualCenter.Z + zSize / 2 && !last_collided)
	{
		last_collided = true;
		return true;
	}
	else {
		last_collided = false;
		return false;
	}
}

int obstacle::nearestFace(Vector other)
{
	int nearest_id = 0;
	Vector diff_vec = faces[0] - other;
	double nearest_dist = (double)abs(diff_vec.X) * (double)abs(diff_vec.Y) * (double)abs(diff_vec.Z);;
	for (int i = 1; i < 6; i++) {
		//double this_dist = abs(faces[i].X - other.X) * abs(faces[i].Y - other.Y) * abs(faces[i].Z - other.Z);
		Vector diff_vec = faces[i] - other;
		double this_dist = (double)abs(diff_vec.X) * (double)abs(diff_vec.Y) * (double)abs(diff_vec.Z);
		if (this_dist < nearest_dist) {
			nearest_dist = this_dist;
			nearest_id = i;
		}
	}
	return nearest_id;
}

