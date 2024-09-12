#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <random>
#include <string>
#include "Snake.h"

int SNAKESFACTOR = 2;

int randomnumber(int mini, int maxi)
{
	return rand() % (maxi - mini + 1) + mini;
}

class Board
{
public:

	Board(int _rows, int _cols, int _SCREEN_WIDTH, int _SCREEN_HEIGHT, int _blockSize, int _population, bool _isAi, bool _preTrainedModels)
	{
		rows = _rows;
		cols = _cols;
		SCREEN_WIDTH = _SCREEN_WIDTH;
		SCREEN_HEIGHT = _SCREEN_HEIGHT;
		blockSize = _blockSize;
		isAi = _isAi;
		srand(time(NULL));

		SNAKESFACTOR = _population;

		preTrainedModels = _preTrainedModels;

		if (!font.loadFromFile("fonts/arial.ttf"))
		{
			std::cerr << "cannot load font!" << std::endl;
		}

		if (isAi)
		{
			for (int i = 0; i < SNAKESFACTOR; i++)
			{
				if (!_preTrainedModels)
				{
					SpawnSnake();
				}
				else
				{
					//SpawnSnake();
				}
			}
		}
		else
		{
			SpawnSnake();
		}
	}

	~Board()
	{

	}

	void SetUpModels()
	{
		std::vector<unsigned> topology;
		topology.push_back(16); // working = {16, 8, 6 ,4}
		topology.push_back(8);
		topology.push_back(6);
		topology.push_back(4);
		
		Net __net = readNet(currentModelFilename, topology);
		
		loadedModel.push_back(__net);

		for (int i = 0; i < SNAKESFACTOR; i++)
		{
			SpawnSpecificSnake(loadedModel.at(0));
		}
	}

	void setModelFilename(std::string ___filename, bool _evolveOrShow)
	{
		evolveOrShowModel = _evolveOrShow;
		currentModelFilename = ___filename;
	}

	void MoveSnake(bool ver, int dir, int arr)
	{
		if (snakes.size() >= 1)
		{
			if (ver)
			{
				snakes.at(arr).mVeritcal(dir);
			}
			else
			{
				snakes.at(arr).mHorizontal(dir);
			}
		}
	}

	void turnSnake(bool ver)
	{
		if (snakes.size() >= 1)
		{
			if (ver)
			{
				snakes.at(0).turnLeft();
			}
			else
			{
				snakes.at(0).turnRight();
			}
		}
	}

	int nbOfSnakesAlive()
	{
		int alv = 0;
		for (int snak = 0; snak < snakes.size(); snak++)
		{
			if (snakes.at(snak).isAlive_())
			{
				alv++;
			}
		}

		return alv;
	}

	void turnRaycasts()
	{
		if (raycastState)
			raycastState = false;
		else
			raycastState = true;
	}

	void Update(bool slowUpdate, bool drawing)
	{
		if (nbOfSnakesAlive() > 0)
		{
			int tempIndexforInput = 0;
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					snakes.at(snak).turnInputOutputShowUp(false);
					if (tempIndexforInput < snakes.at(snak).segmentSize())
					{
						tempIndexforInput = snak;/////////////////////////////
					}
				}
			}
			if (!___stateOfNumbers && drawing)
				snakes.at(tempIndexforInput).turnInputOutputShowUp(showInputVar);

			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).getFitness() > FitnessPerGen)
					FitnessPerGen = snakes.at(snak).getFitness();

				if (snakes.at(snak).isAlive_())
				{
					bool canGoFurther = true;
					//snakes.at(snak).Update(apples.at(0).getPos().x, apples.at(0).getPos().y);
					snakes.at(snak).Update();

					if (snakes.at(snak).getPos(0).x < 0 || snakes.at(snak).getPos(0).x >= rows || snakes.at(snak).getPos(0).y < 0 || snakes.at(snak).getPos(0).y >= cols)
					{
						snakes.at(snak).setIsAlive(false);
					}
					else
					{
						for (int i = 1; i < snakes.at(snak).segmentSize(); i++)
						{
							if (snakes.at(snak).getPos(0) == snakes.at(snak).getPos(i))
							{
								snakes.at(snak).setIsAlive(false);
								canGoFurther = false;
							}
						}
					}

					if (canGoFurther)
					{
						if (snakes.at(snak).getPos(0) == snakes.at(snak).applePos()) /// remeber!!!
						{
							snakes.at(snak).SpawnApple();

							if (isAi)
							{
								snakes.at(snak).addScore(10);

							}
							else
							{

							}
							snakes.at(snak).AddSegment();
						}
					}
				}
			}
		}
		else
		{
			if (isAi)
			{
				// GENERATIONS DOWN HERE
				int bestSize = 0;
				int bestIndex=-1;

				unsigned sumFitness = 0;
				unsigned avrFitness = 0;
				
				unsigned sumSize = 0;
				double avrSize = 0;

				std::vector<Net> male;

				for (int snak = 0; snak < snakes.size(); snak++)
				{
					snakes.at(snak).turnInputOutputShowUp(false);

					sumFitness += snakes.at(snak).getFitness();
					sumSize += snakes.at(snak).segmentSize();

					if (snakes.at(snak).getFitness() > lastBestFitness)
					{
						male.clear();
						male.push_back(snakes.at(snak).getNN());
						bestSize = snakes.at(snak).segmentSize();
						lastBestFitness = snakes.at(snak).getFitness();
					}
				}

				avrSize = (sumSize / double(snakes.size()));
				avrFitness = sumFitness / snakes.size();

				if (male.size() > 0 && lastBestFitness > FitnessPerGen)
				{
					bestNetwork.clear();
					bestNetwork.push_back(male.at(0));
				}

				snakes.clear();
				
				if (preTrainedModels)
				{
					for (int snak = 0; snak < SNAKESFACTOR; snak++)
					{
							if (snak % 2 == 0)
							{
								if (male.size() > 0)
								{
									LoadSpawnSpecificSnake(male.at(0)); 
								}
								else
									LoadSpawnSpecificSnake(loadedModel.at(0));
							}
							else
							{
								if (bestNetwork.size()>0)
									SpawnSpecificSnake(bestNetwork.at(0));
								else
									LoadSpawnSpecificSnake(loadedModel.at(0));
							}
					}
				}
				else
				{
					for (int snak = 0; snak < SNAKESFACTOR; snak++)
					{
						if (true)//snak % 2 == 0)
						{
							if (male.size() > 0)
							{
								SpawnSpecificSnake(male.at(0));
							}
							else
							{
								std::cout << "error: temp bestNetwork size is equal or less to 0\n";
								SpawnSnake();
							}
						}
						else
						{
							if (bestNetwork.size() > 0)
							{
								SpawnSpecificSnake(bestNetwork.at(0));
							}
							else
							{
								std::cout << "error: bestNetwork size is equal or less to 0\n";
								SpawnSnake();
							}
						}
					}
				}

				if (lastBestFitness > FitnessPerGen)
					FitnessPerGen = lastBestFitness;

				if (lastSaveNetFittness < FitnessPerGen)
				{
					lastSaveNetFittness = FitnessPerGen;
					counterOfGenerations = 0;
					if (bestNetwork.size() > 0)
					bestNetwork.at(0).saveNet(std::to_string(generation)
						+ "-"
						+ std::to_string(int(FitnessPerGen)));
				}
				
				generation++;

				if (bestSize > bestSizeOverGen)
					bestSizeOverGen = bestSize;
				std::cout << "Generation: " << generation << " | C. Best Score: " << lastBestFitness << 
					" | H. Score: " << FitnessPerGen << " | C. Best Size: " << bestSize << " | H. Size: " << bestSizeOverGen << 
					" | avr.Fitness:: " << avrFitness <<  " | avr.Size:: " << avrSize << std::endl;
				counterOfGenerations++;
				lastBestFitness = 0.0f;
				lastHighestIndex = 0;
			}
			else
			{
				snakes.clear();
				SpawnSnake();
			}
		}

	}
	void SpawnApple()
	{
		int tX = randomnumber(1, rows - 1);
		int tY = randomnumber(1, cols - 1);
		apples.push_back(Apple(tX, tY));
	}

	void SpawnAppleInElement(int id)
	{
		int tX = randomnumber(1, rows - 1);
		int tY = randomnumber(1, cols - 1);

		if (apples.size() > id)
			apples.insert(apples.begin() + id, Apple(tX, tY));
		else
			SpawnApple();
	}

	void showInput()
	{
		if (showInputVar)
			showInputVar = false;
		else
			showInputVar = true;
	}

	void eraseSnake(int er)
	{
		if (snakes.size() + er < snakes.size())
			snakes.erase(snakes.begin() + er);
	}

	void applesErase(int er)
	{
		if (apples.size() + er < apples.size())
			apples.erase(apples.begin() + er);
	}

	void drawNet(sf::RenderWindow* window, int id, float scale)
	{
		sf::Text txt("ID: " + std::to_string(id),font,35);

		int gridY = SCREEN_HEIGHT / 4;
		int gridX = SCREEN_WIDTH - (SCREEN_WIDTH / 3) - (rows * blockSize) / 2;

		Net dNet = snakes.at(id).getNN();

		float radiusC = (blockSize / 3) * scale;
		float xDist = (SCREEN_WIDTH / 32) * scale; int xFac = (SCREEN_WIDTH / 12) * scale;
		float yDist = 0; int yFac = (SCREEN_HEIGHT/25)*scale;

		float maxY = 0;	float minX = (xDist - radiusC * 2) * scale;
		float maxX = gridX;	float minY = 1000*scale;
		sf::Color circColor = sf::Color::Blue;
		sf::Color circBColor = sf::Color::Yellow;
		
		sf::CircleShape circ(radiusC);
		circ.setFillColor(sf::Color::Blue);
		circ.setOrigin(circ.getRadius(), circ.getRadius());
		circ.setOutlineColor(sf::Color::White);
		circ.setOutlineThickness((circ.getRadius() / 10)*scale);

		sf::RectangleShape bg;

		std::vector<std::vector<sf::Vector2f>> handler;
		std::vector<double> results;
		
		dNet.getResult(results);
		unsigned IndexRes = 0;
		double maxyTempRes = 0;

		for (int i = 0; i < results.size(); i++)
		{
			if (maxyTempRes >= results.at(i))
			{
				maxyTempRes = results.at(i);
				IndexRes = i;
			}
		}

		for (unsigned i = 0; i < dNet.m_layers.size(); i++)
		{
			std::vector<sf::Vector2f> t;
			handler.push_back(t);
			for (unsigned j = 0; j < dNet.m_layers.at(i).size()-1; j++)
			{
				yDist += yFac;
				if (maxY < yDist)
					maxY = yDist;
				
				handler.at(i).push_back(sf::Vector2f(xDist, gridY - dNet.m_layers.at(i).size() * radiusC + yDist));
			}
			if (i + 1 < dNet.m_layers.size())
			{
				yDist = ((dNet.m_layers.size()+1)*radiusC+ (dNet.m_layers.size() + 1) * yFac)/2;
				xDist += xFac;
			}
			if (maxX < xDist)
				maxX = xDist;
		}

		bg.setSize(sf::Vector2f(xDist,maxY));
		bg.setPosition(sf::Vector2f(minX, minY));
		bg.setOutlineColor(sf::Color::White);
		bg.setOutlineThickness(2);
		bg.setFillColor(sf::Color(112, 113, 115));

		txt.setCharacterSize(30);
		txt.setStyle(sf::Text::Bold);
		float idPlacement = txt.findCharacterPos(txt.getString().getSize() - 1).x - txt.findCharacterPos(0).x;
		txt.setPosition(maxX-50-idPlacement, minY + txt.getCharacterSize() - 5);
		txt.setFillColor(sf::Color::White);
		window->draw(txt);

		for (unsigned i = 0; i < handler.size(); i++)
		{
			for (unsigned j = 0; j < handler.at(i).size(); j++)
			{
				if(i < handler.size()-1)
				{
					for (unsigned we = 0; we < handler.at(i + 1).size(); we++)
					{
						int n = dNet.m_layers.at(i).at(j).getWeightByID(we)*255;

						sf::Vertex line[] =
						{
							sf::Vertex(handler.at(i).at(j),sf::Color(n,n/2,n/2)),
							sf::Vertex(handler.at(i + 1).at(we),sf::Color(255,n,n))
						};
						window->draw(line, 2, sf::Lines);
					}

				}
				circ.setPosition(handler.at(i).at(j));
				
				int c = 255 - ((dNet.m_layers.at(i).at(j).getOutputVal()+1.f) * 127.5);

				circ.setFillColor(sf::Color(c,c,c));
				
				if (i == handler.size() - 1 && j == IndexRes)
				{
					circ.setOutlineColor(sf::Color(222, 255, 36));
					circ.setOutlineThickness(circ.getRadius() / 2);
					window->draw(circ);

					circ.setOutlineColor(sf::Color::White);
					circ.setOutlineThickness(circ.getRadius() / 10);
				}
				else
				{
					window->draw(circ);
				}
			}
		}
	}

	void SpawnSnake()
	{
		std::vector<unsigned> topology;

		topology.push_back(16); // working = {16, 8, 6 ,4}
		topology.push_back(8);
		topology.push_back(6);
		topology.push_back(4);
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, topology, isAi));
		snakes.back().MutateCreature();
	}

	void SpawnSpecificSnake(Net _NN)
	{
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, _NN, isAi));
		snakes.back().MutateCreature();
	}

	void LoadSpawnSpecificSnake(Net _NN)
	{
		int tX = randomnumber(2, rows - 2);
		int tY = randomnumber(2, cols - 2);
		snakes.push_back(Snake(tX, tY, rows, cols, _NN, isAi));
		snakes.back().MutateCreature();
	}

	void showBrain()
	{
		if (brainShowin)
			brainShowin = false;
		else
			brainShowin = true;
	}

	void draw(sf::RenderWindow* window, bool stateofNumbers, float scale)
	{
		___stateOfNumbers = stateofNumbers;
		int gridX;

		if (brainShowin)
			gridX = SCREEN_WIDTH - (SCREEN_WIDTH / 3) - (rows * blockSize) / 2;
		else
			gridX = SCREEN_WIDTH / 2 - (rows * blockSize) / 2;
		
		int gridY = SCREEN_HEIGHT / 2 - (cols * blockSize) / 2;

		int tempIndex = 0;
		for (int snak = 0; snak < snakes.size(); snak++)
		{
			if (snakes.at(snak).isAlive_())
			{
				if (tempIndex < snakes.at(snak).segmentSize())
				{
					tempIndex = snak;/////////////////////////////
				}
			}
		}

		if (tempIndex > lastHighestIndex)
		{
			lastHighestIndex = tempIndex;
		}
		if (!snakes.at(lastHighestIndex).isAlive_())
		{
			lastHighestIndex = tempIndex;
		}

		tempIndex = lastHighestIndex;

		// background
		sf::RectangleShape background(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		background.setFillColor(sf::Color(30, 30, 30));
		background.setPosition(0, 0);
		window->draw(background);

		//map
		sf::RectangleShape tCell(sf::Vector2f(blockSize, blockSize));
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				tCell.setPosition(gridX + r * blockSize, gridY + c * blockSize);
				tCell.setFillColor(sf::Color(166, 166, 166));
				tCell.setOutlineThickness(2);//blockSize / 50);
				tCell.setOutlineColor(sf::Color::White);
				window->draw(tCell);
			}
		}

		//text
		if (snakes.size() >= 1)
		{
			if (!stateofNumbers)
			{
				sf::Text numberOfPoints("Generation: " 
					+ std::to_string(generation) 
					+ " | Alive: " + std::to_string(nbOfSnakesAlive()) 
					+ " | Population: " + std::to_string(snakes.size()) + " | BestScore: " 
					+ std::to_string((FitnessPerGen)) + " | CurrentSnake ID: " + std::to_string(tempIndex) 
					+ "| Size: " + std::to_string(snakes.at(tempIndex).segmentSize() - 1) 
					+ "| Fitness: " + std::to_string(int(snakes.at(tempIndex).getFitness()))
					+ "| LeftMoves: " + std::to_string(snakes.at(tempIndex).leftMovesNumber()) + "-" + std::to_string(snakes.at(tempIndex).MaxleftMovesNumber()),
					font, 20);
				numberOfPoints.setCharacterSize(15);
				numberOfPoints.setStyle(sf::Text::Bold);
				numberOfPoints.setPosition(10, 10);
				numberOfPoints.setFillColor(sf::Color::White);
				window->draw(numberOfPoints);
			}
			else
			{
				sf::Text numberOfPoints("Generation: "
					+ std::to_string(generation)
					+ " | Alive: " + std::to_string(nbOfSnakesAlive())
					+ " | Population: " + std::to_string(snakes.size()) + " | BestScore: "
					+ std::to_string((FitnessPerGen)), font, 20);
				numberOfPoints.setCharacterSize(15);
				numberOfPoints.setStyle(sf::Text::Bold);
				numberOfPoints.setPosition(10, 10);
				numberOfPoints.setFillColor(sf::Color::White);
				window->draw(numberOfPoints);
			}
		}
		else
		{
			sf::Text numberOfPoints("Points: 0 | Generation: " + std::to_string(generation) + " | Alive: " + std::to_string(snakes.size()), font, 20);
			numberOfPoints.setCharacterSize(15);
			numberOfPoints.setStyle(sf::Text::Bold);
			numberOfPoints.setPosition(10, 10);
			numberOfPoints.setFillColor(sf::Color::White);
			window->draw(numberOfPoints);
		}

		//snake
		sf::RectangleShape snakeShape(sf::Vector2f(blockSize, blockSize));

		if (stateofNumbers)
		{
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					for (int i = 0; i < snakes.at(snak).segmentSize(); i++)
					{
						snakeShape.setPosition(gridX + snakes.at(snak).getPos(i).x * blockSize, gridY + snakes.at(snak).getPos(i).y * blockSize);
						snakeShape.setFillColor(snakes.at(snak).getColor());
						snakeShape.setOutlineThickness(2);//blockSize / 50);
						snakeShape.setOutlineColor(sf::Color::Green);
						window->draw(snakeShape);

						if (raycastState)//raycasts
						{
							std::vector<sf::Vector2f> v1 = snakes.at(snak).linePos();
							
							for (int y = 0; y < v1.size(); y += 2)
							{
								v1.at(y).x = gridX + (v1.at(y).x * blockSize);
								v1.at(y).y = gridY + (v1.at(y).y * blockSize);

								v1.at(y + 1).x = gridX + (v1.at(y + 1).x * blockSize);
								v1.at(y + 1).y = gridY + (v1.at(y + 1).y * blockSize);

								sf::Vertex v[] =
								{
									sf::Vertex(v1.at(y),sf::Color::Red),
									sf::Vertex(v1.at(y + 1),sf::Color::Red)
								};
								window->draw(v, 2, sf::Lines);
							}
						}
					}
				}
			}
		}
		else
		{
			if (snakes.at(tempIndex).isAlive_())
			{
				snakes.at(tempIndex).turnInputOutputShowUp(showInputVar);
				for (int i = 0; i < snakes.at(tempIndex).segmentSize(); i++)
				{
					snakeShape.setPosition(gridX + snakes.at(tempIndex).getPos(i).x * blockSize, gridY + snakes.at(tempIndex).getPos(i).y * blockSize);
					snakeShape.setFillColor(snakes.at(tempIndex).getColor());
					snakeShape.setOutlineThickness(2);//blockSize / 50);
					snakeShape.setOutlineColor(sf::Color::Green);
					window->draw(snakeShape);
					if (raycastState)//raycasts
					{
						std::vector<sf::Vector2f> v1 = snakes.at(tempIndex).linePos();

						for (int y = 0; y < v1.size(); y += 2)
						{
							v1.at(y).x = gridX + (v1.at(y).x * blockSize);
							v1.at(y).y = gridY + (v1.at(y).y * blockSize);

							v1.at(y + 1).x = gridX + (v1.at(y + 1).x * blockSize);
							v1.at(y + 1).y = gridY + (v1.at(y + 1).y * blockSize);

							sf::Vertex v[] =
							{
								sf::Vertex(v1.at(y),sf::Color::Red),
								sf::Vertex(v1.at(y + 1),sf::Color::Red)
							};
							window->draw(v, 2, sf::Lines);
						}
					}
				}
			}
		}

		//apple
		sf::RectangleShape appleShape(sf::Vector2f(blockSize, blockSize));
		appleShape.setOutlineThickness(2);//blockSize / 50);
		appleShape.setOutlineColor(sf::Color(255, 28, 28));

		if (stateofNumbers)
		{
			for (int snak = 0; snak < snakes.size(); snak++)
			{
				if (snakes.at(snak).isAlive_())
				{
					appleShape.setPosition(gridX + snakes.at(snak).applePos().x * blockSize, gridY + snakes.at(snak).applePos().y * blockSize);
					appleShape.setFillColor(sf::Color(238, 114, 114));
					window->draw(appleShape);
				}
			}
		}
		else
		{
			appleShape.setPosition(gridX + snakes.at(tempIndex).applePos().x * blockSize, gridY + snakes.at(tempIndex).applePos().y * blockSize);
			appleShape.setFillColor(sf::Color(238, 114, 114));
			window->draw(appleShape);
		}

		if (brainShowin)
			drawNet(window,tempIndex,scale);
	}

	bool checkFitness(double f)
	{
		if (f > lastBestFitness)
		{
			lastBestFitness = f;
		}
		if (f > FitnessPerGen)
		{
			return true;
		}
		return false;
	}

private:
	unsigned FitnessPerGen = 0;
	unsigned lastBestFitness = 0;

	std::string currentModelFilename="";

	sf::Font font;

	bool isAi;
	bool preTrainedModels;
	bool evolveOrShowModel;
	bool raycastState=false;
	bool showInputVar = false;
	bool ___stateOfNumbers = false;
	bool brainShowin = false;

	int lastHighestIndex = 0;
	int generation = 0;
	int counterOfGenerations = 0;
	int lastSaveNetFittness = 0;
	int rows;
	int cols;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int blockSize;
	int bestSizeOverGen = 0;

	std::vector<Snake> bestPerGen;
	std::vector<Net> female;
	std::vector<Net> bestNetwork;
	std::vector<Net> loadedModel;
	std::vector<Snake> snakes;
	std::vector<Apple> apples;
};
