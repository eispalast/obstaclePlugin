#include "obstaclePlugin.h"
#include "utils\parser.h"


#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"

BAKKESMOD_PLUGIN(obstaclePlugin, "Obstacle plugin", "0.1", PLUGINTYPE_FREEPLAY)


void obstaclePlugin::onLoad()
{
	this->active = false;
	cvarManager->registerCvar("obstacle_x_size", "1000", "The size in x dimension", false, false, 10.0f, true, 2000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_x_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_y_size", "1000", "The size in y dimension", false, false, 10.0f, true, 2000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_y_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_z_size", "1000", "The size in z dimension", false, false, 10.0f, true, 2000.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_z_size, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_moving", "0", "Determines if the obstacle moves", true, true, 0.0f, true, 1.0f, true);
	cvarManager->registerCvar("obstacle_x_speed", "0", "Determines the speed in x dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_x_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_y_speed", "0", "Determines the speed in y dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_y_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_z_speed", "0", "Determines the speed in z dimension", true, true, 0.0f, true, 10.0f, true).addOnValueChanged(std::bind(&obstaclePlugin::change_z_speed, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->registerCvar("obstacle_x_moving_distance", "0", "Determines how far the obstacle moves in x dimension", true, true, 0.0f, true, 1000.0f, true);
	cvarManager->registerCvar("obstacle_y_moving_distance", "0", "Determines how far the obstacle moves in y dimension", true, true, 0.0f, true, 1000.0f, true);
	cvarManager->registerCvar("obstacle_z_moving_distance", "0", "Determines how far the obstacle moves in z dimension", true, true, 0.0f, true, 1000.0f, true);
	
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
		Obstacle.setCenter(carPosition + Vector(0, 0, 500));
	}
	else {
		this->Obstacle = obstacle(carPosition + Vector(0, 0, 500), 1000, 1000, 1000);
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


	

	/*
	canvas.DrawLine(canvas.Project(Obstacle.downLeftFront), canvas.Project(Obstacle.downRightFront), 2);
	canvas.DrawLine(canvas.Project(Obstacle.downLeftFront), canvas.Project(Obstacle.downLeftBack));
	canvas.DrawLine(canvas.Project(Obstacle.downLeftFront), canvas.Project(Obstacle.upLeftFront));
	canvas.DrawLine(canvas.Project(Obstacle.upRightFront), canvas.Project(Obstacle.upLeftFront));
	canvas.DrawLine(canvas.Project(Obstacle.upRightFront), canvas.Project(Obstacle.downRightFront));
	canvas.DrawLine(canvas.Project(Obstacle.upRightFront), canvas.Project(Obstacle.upRightBack));
	canvas.DrawLine(canvas.Project(Obstacle.upRightBack), canvas.Project(Obstacle.upLeftBack));
	canvas.DrawLine(canvas.Project(Obstacle.upRightBack), canvas.Project(Obstacle.downRightBack));
	canvas.DrawLine(canvas.Project(Obstacle.downRightBack), canvas.Project(Obstacle.downLeftBack));
	canvas.DrawLine(canvas.Project(Obstacle.downRightBack), canvas.Project(Obstacle.downRightFront));
	canvas.DrawLine(canvas.Project(Obstacle.upLeftFront), canvas.Project(Obstacle.upLeftBack));
	canvas.DrawLine(canvas.Project(Obstacle.upLeftBack), canvas.Project(Obstacle.downLeftBack));
	*/
}

void obstaclePlugin::checkCollision(std::string eventName)
{
	if (!active || !gameWrapper->IsInFreeplay()) {
		active = false;
		gameWrapper->UnregisterDrawables();
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	}
	if (cvarManager->getCvar("obstacle_moving").getBoolValue()) {
		Vector result = Obstacle.addOffset(offsetAdder);
		if (result.X > cvarManager->getCvar("obstacle_x_moving_distance").getIntValue() ||
			result.X < -cvarManager->getCvar("obstacle_x_moving_distance").getIntValue()) {
			offsetAdder.X *= -1;
		}
		if (result.Y > cvarManager->getCvar("obstacle_y_moving_distance").getIntValue() ||
			result.Y < -cvarManager->getCvar("obstacle_y_moving_distance").getIntValue()) {
			offsetAdder.Y *= -1;
		}
		if (result.Z > cvarManager->getCvar("obstacle_z_moving_distance").getIntValue() ||
			result.Z < -cvarManager->getCvar("obstacle_z_moving_distance").getIntValue()) {
			offsetAdder.Z *= -1;
		}
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
		ball.SetLocation(prevBallPosition);
		if (nearest_face == 0 || nearest_face == 2) {
			ball.SetVelocity(Vector(prevBallVel.X, -prevBallVel.Y, prevBallVel.Z));
		}
		else if (nearest_face == 1 || nearest_face == 3) {
			ball.SetVelocity(Vector(-prevBallVel.X, prevBallVel.Y, prevBallVel.Z));
		}
		else if (nearest_face == 4) {
			ball.SetVelocity(Vector(prevBallVel.X, prevBallVel.Y, -prevBallVel.Z+650/120));
		}
		else {
			ball.SetVelocity(Vector(prevBallVel.X, prevBallVel.Y, -prevBallVel.Z));
		}


	}
	prevBallPosition = ball_location;
	prevBallVel = ball_vel;

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
			/*canvas.DrawLine(prevLine.minus({ 1,1 }), line.minus({ 1,1 })); //make lines thicker
			cw.DrawLine(prevLine.minus({ -1,-1 }), line.minus({ -1,-1 }));
			if (renderFrames) {
				cw.SetColor(0, 0, 0, 255);
				cw.SetPosition(line);
				cw.DrawString(to_string(it->first));
			}*/
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
}

void obstaclePlugin::change_y_speed(std::string oldvalue, CVarWrapper cvar)
{
	offsetAdder.Y = cvar.getIntValue();
}

void obstaclePlugin::change_z_speed(std::string oldvalue, CVarWrapper cvar)
{
	offsetAdder.Z = cvar.getIntValue();
}


