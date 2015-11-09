#ifndef __DOMINO_MANAGER_H__
#define __DOMINO_MANAGER_H__

#include <vector>
#include "cocos2d.h"
#include "Singleton.h"

#define EDITOR_UI_TILE_TYPE_MAX 8

class DominoManager: public Singleton<DominoManager>
{
public:
	DominoManager();
	~DominoManager();

	unsigned int getTileWidth() const;
	unsigned int getTileHeight() const;
	unsigned int getCurEditorTileIndex() const;
	int getCurEditorTileType() const;
	int getEditorTileType(int index) const;

	void setTileWidth(unsigned int w);
	void setTileHeight(unsigned int h);
	void setCurEditorTileIndex(unsigned int index);
	int setEditorTileType(int index, unsigned int type);

	cocos2d::CCPoint calculateCoordinate(float x, float y, float mapWidth, float mapHeight);

private:
	std::vector<char> mTileMap;
	unsigned int mTileWidth;
	unsigned int mTileHeight;
	unsigned int mCurEditorTileIndex;
	unsigned int mEditorTileType[EDITOR_UI_TILE_TYPE_MAX];
};


#endif