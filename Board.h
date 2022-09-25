#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <fstream>
struct Tile
{
	Tile* topLeft = nullptr;
	Tile* topMid = nullptr;
	Tile* topRight = nullptr;
	Tile* midLeft = nullptr;
	Tile* midRight = nullptr;
	Tile* botLeft =  nullptr;
	Tile* botMid = nullptr;
	Tile* botRight = nullptr;
	bool revealed = false;
	bool bombed = false;
	bool flagged = false;
	bool numbered = false;
	int x;
	int y;
};
class Board
{
	std::vector<std::vector<Tile>> board;
	std::vector<std::vector<sf::Sprite>> sprTiles;
	int r;
	int c;
	int mines;
	bool debugMode = false;
	int gameState = 0;

	int tilesRev = 0;
	int flagCount = 0;
public:
	Board(int h, int w, int m);

	Tile getTile(int i, int j);
	std::vector<std::vector<sf::Sprite>>&getBoardTiles();
	
	std::vector<std::vector<Tile>>& getBoardInfo();
	bool getDebug();
	int getGameState();
	int getTilesRev();
	int getMineCount();
	int getFlagCount();
	void setFlagCount(int i);
	void setDebug();
	void setGameState(int i);
	void LoadSprites(int r, int c);
	void LoadTiles(int r, int c);
	void LoadNumbers(int r, int c);
	void RevealTile(int x, int y);
	void AddFlag(int x, int y);
	sf::Sprite getAdjacentTotal(int x, int y);
	int getAdjacentNumber(int x, int y);
	void showAllMines();

	void loadFromFile(string path);

};