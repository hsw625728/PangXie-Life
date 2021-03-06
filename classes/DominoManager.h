#ifndef __DOMINO_MANAGER_H__
#define __DOMINO_MANAGER_H__

#include <vector>
#include "cocos2d.h"
#include "Singleton.h"

#define EDITOR_UI_TILE_TYPE_MAX 8
typedef std::vector<unsigned char> TileMapStr;
struct TileMap
{
	TileMapStr map;
	unsigned int mTileWidth;
	unsigned int mTileHeight;
};
typedef std::vector<TileMap> TileMaps;

class DominoManager: public Singleton<DominoManager>
{
public:
	DominoManager();
	~DominoManager();

	unsigned int getTileWidth(int index) const;
	unsigned int getTileHeight(int index) const;
	unsigned int getCurEditorTileIndex() const;
	int getCurEditorTileType() const;
	int getEditorTileType(int index) const;
	TileMap& getTileMap(int index);
	int getTileMapsCount() const;
	void addTileMap(const TileMap& map);
	void deleteTileMap(int index);

	void setTileWidth(int index, unsigned int w);
	void setTileHeight(int index, unsigned int h);
	void setCurEditorTileIndex(unsigned int index);
	int setEditorTileType(int index, unsigned int type);

	cocos2d::CCPoint calculateCoordinate(float x, float y, float mapWidth, float mapHeight);

private:
	TileMaps mTileMaps;
	unsigned int mCurEditorTileIndex;
	unsigned int mEditorTileType[EDITOR_UI_TILE_TYPE_MAX];
};


#endif