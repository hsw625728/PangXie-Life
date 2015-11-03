#include "DominoManager.h"

template<> DominoManager* Singleton<DominoManager>::msSingleton = 0;

DominoManager::DominoManager()
{
	for (int i = 0; i < EDITOR_UI_TILE_TYPE_MAX; i++)
	{
		mEditorTileType[i] = i + 1;
	}
}

DominoManager::~DominoManager()
{
}

unsigned int DominoManager::getTileWidth() const
{
	return mTileWidth;
}

unsigned int DominoManager::getTileHeight() const
{
	return mTileHeight;
}

unsigned int DominoManager::getCurEditorTileIndex() const
{
	return mCurEditorTileIndex;
}

int DominoManager::getCurEditorTileType() const
{
	if (mCurEditorTileIndex < 0 || mCurEditorTileIndex >= EDITOR_UI_TILE_TYPE_MAX)
	{
		return -1;
	}
	return mEditorTileType[mCurEditorTileIndex];
}

int DominoManager::getEditorTileType(int index) const
{
	if (index < 0 || index >= EDITOR_UI_TILE_TYPE_MAX)
	{
		return -1;
	}
	return mEditorTileType[index]; 
}

void DominoManager::setTileWidth(unsigned int w)
{
	mTileWidth = w;
}

void DominoManager::setTileHeight(unsigned int h)
{
	mTileHeight = h;
}

void DominoManager::setCurEditorTileIndex(unsigned int index)
{
	mCurEditorTileIndex = index;
}

int DominoManager::setEditorTileType(int index, unsigned int type)
{
	if (index < 0 || index >= EDITOR_UI_TILE_TYPE_MAX)
	{
		return -1;
	}
	mEditorTileType[index] = type;
	return 0;
}

cocos2d::CCPoint DominoManager::calculateCoordinate(float x, float y, float mapWidth, float mapHeight)
{
	cocos2d::CCPoint index;
	double distanse, sin1, sin11, sin22, cos11, cos1;
	int d2x, d2y;
	double mystatic5 = sqrt(5.0);
	double mystatic = 16 * mystatic5;
	if (x > mapWidth / 2)
	{
		distanse = sqrt((x - mapWidth / 2) * (x - mapWidth / 2) + (mapHeight - y) * (mapHeight - y));
		sin1 = (mapHeight - y) / distanse;
		cos1 = (x - mapWidth / 2) / distanse;
		sin11 = (sin1 * 2 - cos1) / mystatic5;
		cos11 = (sin1 + cos1 * 2) / mystatic5;
		d2y = distanse * 5 / 4 * sin11 / mystatic;
		sin22 = (2 * sin1 + cos1) / mystatic5;
		d2x = distanse * 5 / 4 * sin22 / mystatic;
	}
	else
	{
		distanse = sqrt((mapWidth / 2 - x) * (mapWidth / 2 - x) + (mapHeight - y) * (mapHeight - y));
		sin1 = (mapHeight - y) / distanse;
		cos1 = (mapWidth / 2 - x) / distanse;
		sin11 = (sin1 * 2 - cos1) / mystatic5;
		cos11 = (sin1 + cos1 * 2) / mystatic5;
		d2x = distanse * 5 / 4 * sin11 / mystatic;
		sin22 = (2 * sin1 + cos1) / mystatic5;
		d2y = distanse * 5 / 4 * sin22 / mystatic;
	}
	index.x = d2x;
	index.y = d2y;
	return index;
}