#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "obstacle.h"


class obstaclePlugin : public BakkesMod::Plugin::BakkesModPlugin
{

public:
	virtual void onLoad();
	virtual void onUnload();

	/// <summary>
	/// Places the Obstacle at the player's position with the currently set dimensions (x,y,z size)
	/// </summary>
	void spawnObstacle(bool& retflag);

	/// <summary>
	/// Is called every tick. Applyies movement to the obstacle, checks whether it is hit by the the ball and if so, changes the ball's velocity
	/// </summary>
	void checkCollision(std::string eventName);
	
	/// <summary>
	/// Is called by the checkCollision function (because it is called every tick). Yeah I know, this function could also be hooked. Lazy.
	/// </summary>
	void applyMovement();


private:
	obstacle Obstacle;
	boolean active = false;
	void render(CanvasWrapper canvas);
	Vector prevBallPosition;
	Vector prevBallVel;
	bool obsExists = false;

	//Yeah right.. "correctly"...
	void renderCorrectly(CanvasWrapper canvas, Vector firstPoint, Vector secondPoint, Vector camLocation, Rotator camRotation, Vector2 canvasSize);
	void change_x_size(std::string oldvalue, CVarWrapper cvar);
	void change_y_size(std::string oldvalue, CVarWrapper cvar);
	void change_z_size(std::string oldvalue, CVarWrapper cvar);
	void change_x_speed(std::string oldvalue, CVarWrapper cvar);
	void change_y_speed(std::string oldvalue, CVarWrapper cvar);
	void change_z_speed(std::string oldvalue, CVarWrapper cvar);
	void change_x_distance(std::string oldvalue, CVarWrapper cvar);
	void change_y_distance(std::string oldvalue, CVarWrapper cvar);
	void change_z_distance(std::string oldvalue, CVarWrapper cvar);
	
	/// <summary>
	/// change the true center of the obstacle. 
	/// </summary>
	/// <param name="oldvalue"></param>
	/// <param name="cvar"></param>
	void move_obstacle(std::string oldvalue, CVarWrapper cvar);

	//the obstacle's speed
	Vector offsetAdder=Vector(0,0,0);
	Vector maxMovement=Vector(0,0,0);
	Vector wallVelocity = Vector(0, 0, 0);
	float k = 1.0;
	void k_changed(std::string oldvalue, CVarWrapper cvar);
	
	

};

void strip(std::string& input);
