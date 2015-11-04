#ifndef ___NET_WORK_H__
#define __NET_WORK_H__

#include "Singleton.h"
#include "network/TcpSocket.h"

class NetManager : public Singleton < NetManager >, public cocos2d::Node
{
public:
	NetManager();
	~NetManager();
	bool init();

	bool processAll(int tag, int cmd, WorldPacket& packet);
	void onDisconnect(int tag);
public:
	void cs_Login();
	void processLogin(int tag, WorldPacket& packet);
	void cs_Nickname();
	void processNicknameRes(int tag, WorldPacket& packet);
	void cs_PlayerState();
	void processPlayerStateRes(int tag, WorldPacket& packet);
	void cs_GetSchedule();
	void processGetScheduleRes(int tag, WorldPacket& packet);
	void processPlayerUpdateState(int tag, WorldPacket& packet);
	void cs_DominoUploadTilemap();
	void processDominoUploadTilemapRes(int tag, WorldPacket& packet);
	void cs_DominoDownloadTilemap();
	void processDominoDownloadTilemapRes(int tag, WorldPacket& packet);
};

#endif