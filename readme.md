Obstacle plugin
===============
A plugin for BakkesMod

## What does this plugin do?
Spawns a resizable, movable obstacle in free play which blocks the ball and bounces it back.
You can use it as a dummy goalkeeper to make scoring harder or you can build a high wall to train high shots/flicks or whatever comes to your mind.

## Manual installation
Just in case the installer doesn't work.
1. Install BakkesMod 
2. place the obstaclePlugin.dll in the BakkesMod/plugins folder. (You cann quickly open the BakkesMod folder by opening BakkesMod -> click on _file_ -> click on _Open BakkesMod folder_ )
3. place the obstaclePlugin.set in the BakkesMod/plugins/settings folder
4. Start RocketLeague
5. Press F6 to open the console
6. Type "plugin load obstacleplugin" (without "") and press enter 
7. Type "cl_settings_refreshplugins" (without "") and press enter

## How to use
When in freeplay drive to the location where you want the obstacle to be spawned. Press F2 to open the BakkesMod settings. Click on _Plugins_. Choose _obstacle plugin_ from the list on the left. Click on _place obstacle here_ to spawn the obstacle. You can change the obstacle's size with the three sliders (x size, y size, z size) on the top of the window. 

If you want to move the obstacle somewhere else you can either drive your car to a different location and press _place obstacle here_ again or use the buttons _move x+_, _move x-_, _move_y+_ etc. to move it whereever you need it.

The obstacle can also move automatically. Just active the checkbox _Moving_. After that you cann access the sliders (moving distance in x, y and z dimension, and moving speed in x, y and z dimension) below. 

To deactive the obstacle press _deactivate_. You can reactivate it by either pressing _active_ to spawn it to its last location or by pressing _place obstacle here_ (see above).

## Known bugs
* Sometimes the ball is sucked into the obstacle, especially when the ball is rolling rather slow.
* The obstacle isn't always fully visible, especially when you are quite near

I am sorry and I am working on it!


## Feedback 
You are welcome to leave any (productive) feedback, questions, wishes, sorrows on my [discord](https://discord.gg/hdbTzn) or on [youtube](https://www.youtube.com/channel/UC1EfDyD3_BzykGvJnjTIKEw).

### A big thanks
to Rocket Science for his detailed [video](https://www.youtube.com/watch?v=t-5SGaunD_s) on how to create a plugin!