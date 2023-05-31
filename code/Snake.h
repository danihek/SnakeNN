#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <random>
#include "Apple.h"
#include "Thinking.h"


int rrandomnumber(int mini, int maxi)
{
	return rand() % (maxi - mini + 1) + mini;
}

float generateRandomFloat(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

void swap(double& a, double& b) {
	double temp = a;
	a = b;
	b = temp;
}

int partition(std::vector<double>& arr, int low, int high) {
	float pivot = arr[high];
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (arr[j] <= pivot) {
			i++;
			swap(arr[i], arr[j]);
		}
	}

	swap(arr[i + 1], arr[high]);
	return i + 1;
}

void quickSort(std::vector<double>& arr, int low, int high) {
	if (low < high) {
		int pivotIndex = partition(arr, low, high);
		quickSort(arr, low, pivotIndex - 1);
		quickSort(arr, pivotIndex + 1, high);
	}
}

class Snake
{
public:
	Snake(int _x, int _y, int _rows, int _cols, Net _nn, bool _isAi)
	{
		segment.push_back(sf::Vector2i(_x, _y));
		lastPositions.push_back(segment.at(0));

		int r = rrandomnumber(0, 255);
		int g = rrandomnumber(0, 255);
		int b = rrandomnumber(0, 255);
		color = sf::Color(r, g, b);

		vel.x = 0;
		vel.y = 0;

		rows = _rows;
		cols = _cols;

		isAi = _isAi;

		nn.push_back(_nn);

		SpawnApple();

	}

	~Snake()
	{

	}

	void SpawnApple()
	{
		if (apples.size()>=1)
			apples.clear();
		bool succed = false;
		
		int tX = 0;
		int tY = 0;
		while (!succed)
		{
			tX = rrandomnumber(0, rows-1);
			tY = rrandomnumber(0, cols-1);
			
			for (int i = 0; i < segmentSize(); i++)
			{
				if (!(tX == segment.at(i).x && tY == segment.at(i).y))
				{
					succed = true;
				}
				else
				{
					succed = false;
				}
			}
		}
		
		apples.push_back(Apple(tX, tY));
	}

	void MutateCreature()
	{
		if (mutateMutations)
		{
			//mutationAmount = // / 100;
		}
		mutationChance = generateRandomFloat(0.01, 1)/100;;//generateRandomFloat(0.001f, 1.0f)// / 100;

		nn.at(0).mutate(mutationChance);// , mutationAmount);

		mutateMutations = false;
	}

	void SetNN(Net _nn)
	{
		nn.clear();
		nn.push_back(_nn);
	}

	Net getNN()
	{
		//MutateCreature();
		return nn.at(0);
	}

	Snake GetChild()
	{
		//MutateCreature();

		return *this;
	}


	void AddSegment()
	{
		sf::Vector2i newSegment;
		newSegment.x = segment.at(segment.size() - 1).x - (0 - vel.x);
		newSegment.y = segment.at(segment.size() - 1).y - (0 - vel.y);

		segment.push_back(newSegment);
		lastPositions.push_back(segment.at(segment.size() - 1));
		lastPositionsForExecute.push_back(segment.at(0));
	}

	bool canChangeVelX(int dir)
	{
		if (vel.x != 0 )
		{
			//if (dir==vel.x)
			//	return false;
			//else
			//	isAlive = false;
			return false;
		}
		else
			return true;
	}

	bool canChangeVelY(int dir)
	{
		if (vel.y != 0)
		{
			//if (dir == vel.y)
				//return false;
			///else
			//	isAlive = false;
			return false;
		}
		else
			return true;
	}

	void mVeritcal(int direction)
	{
		vel.x = direction;
		vel.y = 0;
	}

	void mHorizontal(int direction)
	{
		vel.x = 0;
		vel.y = direction;
	}

	bool isAlive_()
	{
		return isAlive;
	}

	void setIsAlive(bool alv)
	{
		isAlive = alv;
	}
	
	double distance(double x1, double y1, double x2, double y2)
	{
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
	}

	
	void inputSet(std::vector<double> *inputs)
	{
		bool n = true, e = n, s = n, w = n, nw = n, ne = n, se = n, sw = n;
		bool n1 = true, e1 = n, s1 = n, w1 = n, nw1 = n, ne1 = n, se1 = n, sw1 = n;
		bool n2 = true, e2 = n, s2 = n, w2 = n, nw2 = n, ne2 = n, se2 = n, sw2 = n;
		//inputs.push_back(1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x, segment.at(0).y)); -- template
		inputs->at(0) = (1 / distance(0, segment.at(0).y, segment.at(0).x, segment.at(0).y));
		inputs->at(1) = (1 / distance(segment.at(0).x, 0, segment.at(0).x, segment.at(0).y));
		inputs->at(2) = (1 / distance(segment.at(0).x, segment.at(0).y, rows, segment.at(0).y));
		inputs->at(3) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x, cols));
		
		for (int r = rows*2; r > 0; r--)
		{
			// for walls
			if (segment.at(0).x-r <= 0 || segment.at(0).y-r <= 0 && nw)
			{
				inputs->at(4) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x-r, segment.at(0).y-r));
				nw = false;
			}
			if (segment.at(0).x + r >= rows || segment.at(0).y - r <= 0 && ne)
			{
				inputs->at(5) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y - r));
				ne = false;
			}
			if (segment.at(0).x + r >= rows || segment.at(0).y + r >= cols && se)
			{
				inputs->at(6) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y + r));
				se = false;
			}
			if (segment.at(0).x - r <= 0 || segment.at(0).y + r >= cols && sw)
			{
				inputs->at(7) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x - r, segment.at(0).y + r));
				sw = false;
			}
			// walls up here

			//apples rays down here
			if (segment.at(0).x - r == apples.at(0).getPos().x && w2)
			{
				inputs->at(8) = (1 / distance(segment.at(0).x, 0, segment.at(0).x - r, 0));
				w2 = false;
			}
			if (segment.at(0).x + r == apples.at(0).getPos().x && e2)
			{
				inputs->at(9) = (1 / distance(segment.at(0).x, 0, segment.at(0).x + r, 0));
				e2 = false;
			}
			if (segment.at(0).y - r == apples.at(0).getPos().y && n2)
			{
				inputs->at(10) = (1 / distance(0, segment.at(0).y, segment.at(0).y - r, 0));
				n2 = false;
			}
			if (segment.at(0).y + r == apples.at(0).getPos().y && s2)
			{
				inputs->at(11) = (1 / distance(0, segment.at(0).y, segment.at(0).y + r, 0));
				s2 = false;
			}

			if (segment.at(0).x - r == apples.at(0).getPos().x && segment.at(0).y - r == apples.at(0).getPos().y && nw2)
			{
				inputs->at(12) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x - r, segment.at(0).y - r));
				nw2 = false;
			}
			if (segment.at(0).x + r == apples.at(0).getPos().x && segment.at(0).y - r == apples.at(0).getPos().y && ne2)
			{
				inputs->at(13) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y - r));
				ne2 = false;
			}
			if (segment.at(0).x + r == apples.at(0).getPos().x && segment.at(0).y + r == apples.at(0).getPos().y && se2)
			{
				inputs->at(14) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y + r));
				se2 = false;
			}
			if (segment.at(0).x - r == apples.at(0).getPos().x && segment.at(0).y + r == apples.at(0).getPos().y && sw2)
			{
				inputs->at(15) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x - r, segment.at(0).y + r));
				sw2 = false;
			}
			//apples rays up here

			//segment rays down here
			
		/*	for (int s = 0; s < segmentSize(); s++)
			{
				if (segment.at(0).x - r == segment.at(s).x && w1)
				{
					inputs->at(16) = (1 / distance(segment.at(0).x, 0, segment.at(0).x - r, 0));
					w1 = false;
				}
				if (segment.at(0).x + r == segment.at(s).x && e1)
				{
					inputs->at(17) = (1 / distance(segment.at(0).x, 0, segment.at(0).x + r, 0));
					e1 = false;
				}
				if (segment.at(0).y - r == segment.at(s).y && n1)
				{
					inputs->at(18) = (1 / distance(0, segment.at(0).y, segment.at(0).y - r, 0));
					w1 = false;
				}
				if (segment.at(0).y + r == segment.at(s).y && s1)
				{
					inputs->at(19) = (1 / distance(0, segment.at(0).y, segment.at(0).y + r, 0));
					s1 = false;
				}

				if (segment.at(0).x - r == segment.at(s).x && segment.at(0).y - r == segment.at(s).y && nw1)
				{
					inputs->at(20) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x - r, segment.at(0).y - r));
					nw1 = false;
				}
				if (segment.at(0).x + r == segment.at(s).x && segment.at(0).y - r == segment.at(s).y && ne1)
				{
					inputs->at(21) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y - r));
					ne1 = false;
				}
				if (segment.at(0).x + r == segment.at(s).x && segment.at(0).y + r == segment.at(s).y && se1)
				{
					inputs->at(22) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x + r, segment.at(0).y + r));
					se1 = false;
				}
				if (segment.at(0).x - r == segment.at(s).x && segment.at(0).y + r == segment.at(s).y && sw1)
				{
					inputs->at(23) = (1 / distance(segment.at(0).x, segment.at(0).y, segment.at(0).x - r, segment.at(0).y + r));
					sw1 = false;
				}
			}*/

		}

		//inputs->at(6) = (1 / distance(segment.at(0).x, segment.at(0).y, apples.at(0).getPos().x, apples.at(0).getPos().y));

	}

	void think(int appleX, int appleY)
	{
		std::vector<double> inp;

		for (int i = 0; i < 16; i ++)
		{
			inp.push_back(0.0f);
		}
		
		inputSet(&inp);

		bool n = true, e = n, s = n, w = n, nw = n, ne = n, se = n, sw = n;
		bool n1 = true, e1 = n, s1 = n, w1 = n, nw1 = n, ne1 = n, se1 = n, sw1 = n;
		bool n2 = true, e2 = n, s2 = n, w2 = n, nw2 = n, ne2 = n, se2 = n, sw2 = n;

		if (showInputVarS)
		{
			for (int i = 0; i < inp.size(); i++)
			{
				std::cout << inp.at(i) << ", ";
			}
			std::cout << std::endl;
		}

		nn.at(0).feedForward(inp);
		std::vector<double> outputs;

		nn.at(0).getResult(outputs);

		if (showInputVarS)
		{
			std::cout << " ==== ";
			for (int i = 0; i < outputs.size(); i++)
			{
				std::cout << outputs.at(i) << ", ";
			}
			std::cout << std::endl;
		}
		float maxy = 0;
		int properIndex = 0;
		int properIndex2 = 0;
		/*for (int i = 0; i < nn.at(0).outSize(); i++)
		{
			if (outputs.at(i) == outputsSorted.at(i))
			{
				properIndex = i;
			}
		}*/
		for (int i = 0; i < outputs.size(); i++)
		{
			if (maxy >= outputs.at(i))
			{
				maxy = outputs.at(i);
				properIndex = i;
			}
		}

		//std::cout << properIndex << " " << properIndex2 << std::endl;

		if (properIndex == 0)
		{
			//if (canChangeVelX(-1))
				mVeritcal(-1);
		}
		else if (properIndex == 1)
		{
			//if (canChangeVelX(1))
				mVeritcal(1);
		}
		else if (properIndex == 2)
		{
			//if (canChangeVelY(-1))
				mHorizontal(-1);
		}
		else if (properIndex == 3)
		{
			//if (canChangeVelY(1))
				mHorizontal(1);
		}
	}


	std::vector<sf::Vector2f> linePos()
	{
		std::vector<sf::Vector2f> linepos;
		bool n = true, e = n, s = n, w = n, nw = n, ne = n, se = n, sw = n;

		for (int r = 0; r < rows * 2; r++ )
		{
			if ((segment.at(0).x - r == 0 || segment.at(0).y - r == 0)&&nw)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x - r) * (std::abs(segment.at(0).x) - segment.at(0).x - r)) 
					+ float((std::abs(segment.at(0).y) - segment.at(0).y - r) * (std::abs(segment.at(0).y) - segment.at(0).y - r)))));
				//std::cout << "NW: " << distance / (rows*std::sqrt(2)) << std::endl;
				
				linepos.push_back(sf::Vector2f(segment.at(0).x, segment.at(0).y));
				linepos.push_back(sf::Vector2f(segment.at(0).x - r, segment.at(0).y - r));

				nw = false;
			}

			if ((segment.at(0).x + r == rows || segment.at(0).y + r == cols)&&se)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x + r) * (std::abs(segment.at(0).x) - segment.at(0).x + r))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y + r) * (std::abs(segment.at(0).y) - segment.at(0).y + r)))));
				//std::cout << "SE: " << distance / (rows * std::sqrt(2)) << std::endl;

				linepos.push_back(sf::Vector2f(segment.at(0).x, segment.at(0).y));
				linepos.push_back(sf::Vector2f(segment.at(0).x + r, segment.at(0).y + r));

				se = false;
			}

			if ((segment.at(0).x + 1 + r == 0 || segment.at(0).y - r == 0) && ne)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x + +1+r) * (std::abs(segment.at(0).x) - segment.at(0).x + 1+r))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y - r) * (std::abs(segment.at(0).y) - segment.at(0).y - r)))));
				//std::cout << "NE: " << distance / (rows * std::sqrt(2)) << std::endl;

				linepos.push_back(sf::Vector2f(segment.at(0).x + 1, segment.at(0).y));
				linepos.push_back(sf::Vector2f(segment.at(0).x + r, segment.at(0).y - r));

				ne = false;
			}

			if ((segment.at(0).x - r == 0 || segment.at(0).y+1 + r == 0) && sw)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x - r) * (std::abs(segment.at(0).x) - segment.at(0).x - r))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y+1 + r) * (std::abs(segment.at(0).y) - segment.at(0).y+1 + r)))));
				//std::cout << "SW: " << distance / (rows * std::sqrt(2)) << std::endl;

				linepos.push_back(sf::Vector2f(segment.at(0).x, segment.at(0).y + 1));
				linepos.push_back(sf::Vector2f(segment.at(0).x - r, segment.at(0).y + r));

				sw = false;
			}


			if ((segment.at(0).x - r == 0) && w)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x - r) * (std::abs(segment.at(0).x) - segment.at(0).x - r))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y) * (std::abs(segment.at(0).y) - segment.at(0).y)))));
				
				//std::cout << "W: " << distance / rows << std::endl;
				linepos.push_back(sf::Vector2f(segment.at(0).x, segment.at(0).y + 0.5));
				linepos.push_back(sf::Vector2f(segment.at(0).x - r, segment.at(0).y + 0.5));
				w = false;
			}

			if ((segment.at(0).x + r == rows) && e)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x + r) * (std::abs(segment.at(0).x) - segment.at(0).x + r))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y) * (std::abs(segment.at(0).y) - segment.at(0).y)))));

				//std::cout << "E: " << distance / rows << std::endl;
				linepos.push_back(sf::Vector2f(segment.at(0).x, segment.at(0).y + 0.5));
				linepos.push_back(sf::Vector2f(segment.at(0).x + r, segment.at(0).y + 0.5));
				e = false;
			}

			if ((segment.at(0).y - r == 0) && n)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x) * (std::abs(segment.at(0).x) - segment.at(0).x))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y - r) * (std::abs(segment.at(0).y) - segment.at(0).y - r)))));

				//std::cout << "N: " << distance / rows << std::endl;
				linepos.push_back(sf::Vector2f(segment.at(0).x+0.5, segment.at(0).y));
				linepos.push_back(sf::Vector2f(segment.at(0).x+0.5, segment.at(0).y - r));
				n = false;
			}

			if ((segment.at(0).y + r == cols) && s)
			{
				float distance = float(std::sqrt(float(float((std::abs(segment.at(0).x) - segment.at(0).x) * (std::abs(segment.at(0).x) - segment.at(0).x))
					+ float((std::abs(segment.at(0).y) - segment.at(0).y + r) * (std::abs(segment.at(0).y) - segment.at(0).y + r)))));

				//std::cout << "S: " << distance / rows << std::endl;
				linepos.push_back(sf::Vector2f(segment.at(0).x + 0.5, segment.at(0).y));
				linepos.push_back(sf::Vector2f(segment.at(0).x + 0.5, segment.at(0).y + r));
				s = false;
			}
		}
		return linepos;
	}

	void Update()//int appleX, int appleY)
	{
		//showInputVarS = false;

		if (isAi)
			movesLeft--;
		
		

		if (movesLeft <= 0 && isAi)
		{
			setIsAlive(false);
		}

		if (isAi)
		{
			think(apples.at(0).getPos().x, apples.at(0).getPos().y);
		}


		//fitness += 1;
		moves++;

		if (segmentSize() == 2)
		{
			if (lastPositionsForExecute.at(0) == sf::Vector2i(segment.at(0).x + vel.x, segment.at(0).y + vel.y))
			{
				//isAlive = false;
			}
		}

		for (int i = lastPositions.size() - 1; i > 0; i--)
		{
			if (segmentSize() <= 3)
			{
				lastPositionsForExecute.clear();
				lastPositionsForExecute.push_back(sf::Vector2i(segment.at(i - 1).x, segment.at(i - 1).y));
			}
			segment.at(i).x = segment.at(i - 1).x;
			segment.at(i).y = segment.at(i - 1).y;
		}

		segment.at(0).x += vel.x;
		segment.at(0).y += vel.y;

		calcFitness(); //working
	}

	int segmentSize()
	{
		return segment.size();
	}

	sf::Vector2i getPos(int i)
	{
		return segment.at(i);
	}

	void calcFitness() {
		//fitness is based on length and lifetime
		if (segmentSize() < 10) {
			fitness = floor(moves * moves * pow(2, (floor(segmentSize()))));
		}
		else {
			//grows slower after 10 to stop fitness from getting stupidly big
			//ensure greater than len = 9
			fitness = moves * moves;
			//fitness *= pow(2, 10);
			fitness *= (segmentSize() - 9);
		}
	}

	void calculateFitness()
	{
		//fitness = moves * moves * std::pow(2, score);
		//fitness = moves * pow(2,segmentSize());
		//fitness = segmentSize();
		fitness = segmentSize() * segmentSize() + moves;
	}

	void ResetMovesLeft()
	{
		/*if (segmentSize() < 11)
			movesLeftAmount = movesLeftAmountHolder;
		else if (segmentSize() < 15)
			movesLeftAmount = movesLeftAmountHolder * 3;
		else if (segmentSize() < 20)
			movesLeftAmount = movesLeftAmountHolder * 5;

		else
		{
			movesLeftAmount = movesLeftAmountHolder * 7;
		}
		*/

		movesLeftAmount = segmentSize() * 10 + movesLeftAmountHolder;

		movesLeft = movesLeftAmount;
	}

	void turnInputOutputShowUp(bool inp)
	{
		showInputVarS = inp;
	}

	int leftMovesNumber()
	{
		return movesLeft;
	}

	int MaxleftMovesNumber()
	{
		return movesLeftAmount;
	}

	void addScore(int val)
	{
		score += val;
		ResetMovesLeft();
	}

	double getFitness()
	{
		return fitness;
	}

	sf::Vector2i applePos()
	{
		return apples.at(0).getPos();
	}

	sf::Color getColor()
	{
		return color;
	}

	bool Mutated()
	{
		return mutateMutations;
	}

private:
	double fitness=0;
	double score=0;

	float mutationAmount = 0;
	float mutationChance = 0;
	
	int rows;
	int cols;
	int moves = 0;
	int movesLeftAmountHolder = 50;
	int movesLeftAmount = 50;
	int movesLeft = movesLeftAmount;

	bool mutateMutations = true;
	bool isAi;
	bool showInputVarS = false;
	bool isAlive = true;

	sf::Color color;
	
	std::vector<Net> nn;
	std::vector<Apple> apples;
	std::vector<sf::Vector2i> segment;
	std::vector<sf::Vector2i> lastPositions;
	std::vector<sf::Vector2i> lastPositionsForExecute;
	
	sf::Vector2i vel;
};
