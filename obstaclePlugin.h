#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "obstacle.h"


class obstaclePlugin : public BakkesMod::Plugin::BakkesModPlugin
{

public:
	virtual void onLoad();
	void spawnObstacle(bool& retflag);
	virtual void onUnload();
	void checkCollision(std::string eventName);
private:
	obstacle Obstacle;
	boolean active = false;
	void render(CanvasWrapper canvas);
	Vector prevBallPosition;
	Vector prevBallVel;
	bool obsExists = false;
	void renderCorrectly(CanvasWrapper canvas, Vector firstPoint, Vector secondPoint, Vector camLocation, Rotator camRotation, Vector2 canvasSize);
	void change_x_size(std::string oldvalue, CVarWrapper cvar);
	void change_y_size(std::string oldvalue, CVarWrapper cvar);
	void change_z_size(std::string oldvalue, CVarWrapper cvar);
	void change_x_speed(std::string oldvalue, CVarWrapper cvar);
	void change_y_speed(std::string oldvalue, CVarWrapper cvar);
	void change_z_speed(std::string oldvalue, CVarWrapper cvar);
	Vector offsetAdder=Vector(0,0,0);
	Vector maxMovement=Vector(0,0,0);
	
	

};