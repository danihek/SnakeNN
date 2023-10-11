# SnakeNN - Neural Network Training for Snake Game

# 1. Project

Training neural network to solve the best way to play Snake
using Genetic Algorithm. Program creates X (population) NN's, testing their performance
in game and calculates score of each one using fitness function.
NN with the best fitness value is mixed with second
best NN and mutated X times to fill out population vector.
The best progressing NN's are saved to file named with their fitness and generation.

![alt text](https://raw.githubusercontent.com/danihek07/SnakeNN/main/data/snakes.png)

Neural network is designed like this:

16 - Input Layer ( Every update Snake (his head) is sending rays in 8 directions (like in compass: N, NE, E, SE, S.. etc.)

8 for walls/borders and 8 for food if ray is coliding with object like Food-Apple or border then it takes that distance to neuron with 1/distance.
If ray sent for Food is not pointing for it Neuron Value is set to 0

  {  - Hidden Layer
	8 - first with 8 neurons
     	6 - second with 6 neurons
  }

4 - Output Layer (4 output neurons(move): left, right, up, down)


# 2. Keybinds

pattern = [key] - function

[SPACE] - fast/slow simulation

[0] and [9] - increasing and decreasing speed of simulation

[Q] - no drawing
	
[R] - draw best snake/draw all snakes

[U] - show input and output of NN's (best ID) - only if [R] is activated 

[T] - drawing raycasts on/off

[Y] - drawing neural network on/off

//If game mode is set to player you can move snake with arrows

