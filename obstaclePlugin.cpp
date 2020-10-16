#include "obstaclePlugin.h"
#include "utils\parser.h"


#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"

BAKKESMOD_PLUGIN(obstaclePlugin, "Obstacle plugin", "1.0", PLUGINTYPE_FREEPLAY)


void obstaclePlugin::onLoad()
{
	this->active = false;
	cvarManager->registerCvar("obstacle_x_size", "1000", "The size in x dimension", false, false, 10.0f, true, 8000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_x_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_y_size", "1000", "The size in y dimension", false, false, 10.0f, true, 8000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_y_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_z_size", "1000", "The size in z dimension", false, false, 10.0f, true, 2000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_z_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_moving", "0", "Determines if the obstacle moves", true, true, 0.0f, true, 1.0f, true);
	cvarManager->registerCvar("obstacle_x_speed", "0", "Determines the speed in x dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_x_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_y_speed", "0", "Determines the speed in y dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_y_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_z_speed", "0", "Determines the speed in z dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_z_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_x_moving_distance", "0", "Determines how far the obstacle moves in x dimension", true, true, 0.0f, true, 1000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_x_distance, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_y_moving_distance", "0", "Determines how far the obstacle moves in y dimension", true, true, 0.0f, true, 1000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_y_distance, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_z_moving_distance", "0", "Determines how far the obstacle moves in z dimension", true, true, 0.0f, true, 1000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_z_distance, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_move_center", "", "Usage obstacle_move_center [dimension] [amount]",true,false,0.0f,false,0.0f,false).addOnValueChanged(std::bind(&obstaclePlugin::move_obstacle, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_bouncyness", "1.0", "How hard will the ball get pushed back: 1: very hard, 0: not at all", true, true, 0.0f, true, 1.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::k_changed, this, std::placeholders::_1, std::placeholders::_2));

	cvarManager->registerNotifier("obstacle_place", [&gw = this->gameWrapper, this](std::vector<std::string> commands) {
		bool retflag;
		spawnObstacle(retflag);
		if (retflag) return;
		this->active = true;
		gameWrapper->RegisterDrawable(std::bind(&obstaclePlugin::render, this, std::placeholders::_1));
		gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&obstaclePlugin::checkCollision, this, std::placeholders::_1));
	}, "Spawns the obstacle at to your car's position and actives it", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);

	cvarManager->registerNotifier("obstacle_deactivate", [this](std::vector<std::string> commands) {
		this->active = false;
		gameWrapper->UnregisterDrawables();
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
		}, "Deactivates ", PERMISSION_ALL);

	cvarManager->registerNotifier("obstacle_activate", [this](std::vector<std::string> commands) {
		if (active) return;
		if (!obsExists) {
			bool retflag;
			spawnObstacle(retflag);
			if (retflag) return;
		}
		this->active = true;
		gameWrapper->RegisterDrawable(std::bind(&obstaclePlugin::render, this, std::placeholders::_1));
		gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&obstaclePlugin::checkCollision, this, std::placeholders::_1));
		}, "activates ", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);
}

void obstaclePlugin::spawnObstacle(bool& retflag)
{
	retflag = true;
	ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();

	CarWrapper car = tutorial.GetGameCar();
	if (car.IsNull()) {
		return;
	}
	Vector carPosition = car.GetLocation();
	if (obsExists) {
		Obstacle.setCenter(carPosition + Vector(0, 0, cvarManager->getCvar("obstacle_z_size").getIntValue()/2));
	}
	else {
		this->Obstacle = obstacle(carPosition + Vector(0, 0, cvarManager->getCvar("obstacle_z_size").getIntValue() / 2), cvarManager->getCvar("obstacle_x_size").getIntValue(), cvarManager->getCvar("obstacle_y_size").getIntValue(), cvarManager->getCvar("obstacle_z_size").getIntValue());
	}
	obsExists = true;
	retflag = false;
}

void obstaclePlugin::onUnload()
{

}


void obstaclePlugin::render(CanvasWrapper canvas)
{
	if (!active || !gameWrapper->IsInFreeplay()) {
		active = false;
		gameWrapper->UnregisterDrawables();
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");

		return;
	}
	CameraWrapper cam = gameWrapper->GetCamera();
	auto location = cam.GetLocation();
	auto rotation = cam.GetRotation();
	Vector2 canvasSize = canvas.GetSize();
	canvas.SetColor(255, 255, 255, 255);
	
	renderCorrectly(canvas, Obstacle.downLeftFront, Obstacle.downRightFront, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.downLeftFront, Obstacle.upLeftFront, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.downLeftFront, Obstacle.downLeftBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upRightFront, Obstacle.upLeftFront, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upRightFront, Obstacle.downRightFront, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upRightFront, Obstacle.upRightBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upRightBack, Obstacle.upLeftBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upRightBack, Obstacle.downRightBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.downRightBack, Obstacle.downLeftBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.downRightBack, Obstacle.downRightFront, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upLeftFront, Obstacle.upLeftBack, location, rotation, canvasSize);
	renderCorrectly(canvas, Obstacle.upLeftBack, Obstacle.downLeftBack, location, rotation, canvasSize);
	

}

void obstaclePlugin::checkCollision(std::string eventName)
{
	if (!active || !gameWrapper->IsInFreeplay()) {
		active = false;
		gameWrapper->UnregisterDrawables();
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	}

	//apply the movement and check whether it reached its maximum movement. If so, flip the moving direction
	if (cvarManager->getCvar("obstacle_moving").getBoolValue()) {
		applyMovement();
	}
	//get all information about the ball
	ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
	BallWrapper ball = tutorial.GetGameBalls().Get(0);
	Vector ball_location = ball.GetLocation();
	Vector ball_vel = ball.GetVelocity();
	
	//check if it collides with the obstacle
	if (Obstacle.collides(ball_location)) {
		
		int nearest_face = Obstacle.nearestFace(ball_location);
		if (nearest_face == -1) { //error (no nearest face found)
			return;
		}
		
		
		if (nearest_face == 0 || nearest_face == 2) {
			int yVel = 0;
			if (cvarManager->getCvar("obstacle_moving").getBoolValue() && maxMovement.Y>0 ) {
				yVel = offsetAdder.Y*120;
			}
			ball.SetVelocity(Vector(ball_vel.X, (yVel-(ball_vel.Y-yVel)*k), ball_vel.Z));
			

		}
		else if (nearest_face == 1 || nearest_face == 3) {
			int xVel = 0;
			if (cvarManager->getCvar("obstacle_moving").getBoolValue() && maxMovement.X > 0) {
				xVel = offsetAdder.X * 120;
			}
			ball.SetVelocity(Vector((xVel - (ball_vel.X - xVel) * k), ball_vel.Y,  ball_vel.Z));
			
		}
		else {//top/bottom
			int zVel = 0;
			if (cvarManager->getCvar("obstacle_moving").getBoolValue() && maxMovement.Z > 0) {
				zVel = offsetAdder.Z * 120;
			}
			ball.SetVelocity(Vector(ball_vel.X, ball_vel.Y, (zVel - (ball_vel.Z - zVel) * k)));
			
		}
		
		
	}
	prevBallPosition = ball_location;
	prevBallVel = ball_vel;

}

void obstaclePlugin::applyMovement()
{
	//actually applying movement by changing the offset
	Vector result = Obstacle.addOffset(offsetAdder);

	//change movement direction when the maximum movement distance is reached
	if (result.X > maxMovement.X || result.X < -maxMovement.X) {
		offsetAdder.X *= -1;
	}
	if (result.Y > maxMovement.Y || result.Y < -maxMovement.Y) {
		offsetAdder.Y *= -1;
	}
	if (result.Z > maxMovement.Z || result.Z < -maxMovement.Z) {
		offsetAdder.Z *= -1;
	}
}

inline float Dot(Rotator rot, Vector line)
{
	Vector fov = RotatorToVector(rot);
	return Vector::dot(fov, line);
}

void obstaclePlugin::renderCorrectly(CanvasWrapper canvas, Vector firstPoint, Vector secondPoint, Vector camLocation, Rotator camRotation, Vector2 canvasSize) {
	Vector2 firstPoint2 = canvas.Project(firstPoint);
	Vector2 secondPoint2 = canvas.Project(secondPoint);
	firstPoint.X = std::max(0.0f, firstPoint.X);
	firstPoint.X = std::min((float)canvasSize.X, firstPoint.X);
	firstPoint.Y = std::max(0.0f, firstPoint.Y);
	firstPoint.Y = std::min((float)canvasSize.Y, firstPoint.Y);	
	secondPoint.X = std::max(0.0f, secondPoint.X);
	secondPoint.X = std::min((float)canvasSize.X, secondPoint.X);
	secondPoint.Y = std::max(0.0f, secondPoint.Y);
	secondPoint.Y = std::min((float)canvasSize.Y, secondPoint.Y);
	bool inFrustum = false;
	Vector cam_to_line = (secondPoint - camLocation);
	cam_to_line.normalize();
	float cam_dot_line = Dot(camRotation, cam_to_line);
	if (cam_dot_line > 0) inFrustum = true;
	//if (inFrustum) {
		//if (!(((secondPoint2.X < .1 || secondPoint2.X >= canvasSize.X - .5) && (secondPoint2.Y < .1 || secondPoint2.Y >= canvasSize.Y - .5)) || ((firstPoint2.X < .1 || firstPoint2.X >= canvasSize.X - .5) && (firstPoint2.Y < .1 || firstPoint2.Y >= canvasSize.Y - .5))))
		if (!(((secondPoint2.X < 0 || secondPoint2.X > canvasSize.X ) && (secondPoint2.Y < 0 || secondPoint2.Y >= canvasSize.Y )) || ((firstPoint2.X < 0 || firstPoint2.X >= canvasSize.X ) && (firstPoint2.Y < 0 || firstPoint2.Y >= canvasSize.Y ))))
		{
			canvas.DrawLine(firstPoint2, secondPoint2);
			
		}
	//}
}

void obstaclePlugin::change_x_size(std::string oldvalue, CVarWrapper cvar)
{
	Obstacle.setX(cvar.getIntValue());
}

void obstaclePlugin::change_y_size(std::string oldvalue, CVarWrapper cvar)
{
	Obstacle.setY(cvar.getIntValue());
}

void obstaclePlugin::change_z_size(std::string oldvalue, CVarWrapper cvar)
{
	Obstacle.setZ(cvar.getIntValue());
}

void obstaclePlugin::change_x_speed(std::string oldvalue, CVarWrapper cvar)
{
	offsetAdder.X = cvar.getIntValue();
	if (maxMovement.X > 0) {
		wallVelocity.X = offsetAdder.X * 120;
	}
	else {
		wallVelocity.X = 0;
	}
}

void obstaclePlugin::change_y_speed(std::string oldvalue, CVarWrapper cvar)
{
	offsetAdder.Y = cvar.getIntValue();
	if (maxMovement.Y > 0) {
		wallVelocity.Y = offsetAdder.Y * 120;
	}
	else {
		wallVelocity.Y = 0;
	}
}

void obstaclePlugin::change_z_speed(std::string oldvalue, CVarWrapper cvar)
{
	offsetAdder.Z = cvar.getIntValue();
	if (maxMovement.Z > 0) {
		wallVelocity.Z = offsetAdder.Z * 120;
	}
	else {
		wallVelocity.Z = 0;
	}
}


void obstaclePlugin::change_x_distance(std::string oldvalue, CVarWrapper cvar)
{
	maxMovement.X = cvar.getIntValue();
	if (maxMovement.X > 0) {
		wallVelocity.X = offsetAdder.X * 120;
	}
	else {
		wallVelocity.X = 0;
	}
}

void obstaclePlugin::change_y_distance(std::string oldvalue, CVarWrapper cvar)
{
	maxMovement.Y = cvar.getIntValue();
	if (maxMovement.Y > 0) {
		wallVelocity.Y = offsetAdder.Y * 120;
	}
	else {
		wallVelocity.Y = 0;
	}
}

void obstaclePlugin::change_z_distance(std::string oldvalue, CVarWrapper cvar)
{
	maxMovement.Z = cvar.getIntValue();
	if (maxMovement.Z > 0) {
		wallVelocity.Z = offsetAdder.Z * 120;
	}
	else {
		wallVelocity.Z = 0;
	}
}

void strip(std::string& input)
{
	for (int i = 0; i < input.size(); i++) {
		if (input.at(i) != ' ') {
			input = input.substr(i, input.size() - i);
			break;
		}
	}
	for (int i = input.size() - 1; i >= 0; i--) {
		if (input.at(i) != ' ') {
			input = input.substr(0, i + 1);
			break;
		}
	}
}

void obstaclePlugin::move_obstacle(std::string oldvalue, CVarWrapper cvar)
{
	std::string input = cvar.getStringValue();
	strip(input);
	char dim = input.at(0);
	std::string amount = input.substr(1, input.size() - 1);
	strip(amount);
	int am = std::stoi(amount);
	if (dim == 'x') {
		Obstacle.setCenter(Obstacle.getCenter() + Vector(am, 0, 0));
	}
	else if (dim == 'y') {
		Obstacle.setCenter(Obstacle.getCenter() + Vector(0, am, 0));
	}
	else if (dim == 'z') {
		Obstacle.setCenter(Obstacle.getCenter() + Vector(0, 0,am));
	}
}

void obstaclePlugin::k_changed(std::string oldvalue, CVarWrapper cvar)
{
	k = cvar.getFloatValue();
}

