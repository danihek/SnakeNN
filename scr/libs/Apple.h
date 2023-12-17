#include <iostream>
#include "SFML-2.6.1/include/SFML/Graphics.hpp"

class Apple
{
public:
	Apple(int x, int y)
	{
		positions.x = x;
		positions.y = y;
	}

	~Apple()
	{

	}

	sf::Vector2i getPos()
	{
		return positions;
	}

private:
	sf::Vector2i positions;
};
