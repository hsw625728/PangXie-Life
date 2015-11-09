#include "NetWork.h"
#include "Protocol.h"
#include "GameScene.h"
#include "Player.h"
#include "DominoManager.h"

#define LIFE_SOCKET_ID 1

using namespace cocos2d;
using namespace extension;

template<> NetManager* Singleton<NetManager>::msSingleton = 0;

NetManager::NetManager()
{
}

NetManager::~NetManager()
{
}

bool NetManager::init()
{
	// 创建SOCKET管理器
	CREATE_TCPSOCKETMGR(this);
	// 创建并添加SOCKET，参数：服务器IP，端口，自定义的SOCKET_ID标识
	sSocketMgr.createSocket("123.57.53.187", 13001, LIFE_SOCKET_ID);
	// 注册协议，参数：包头，回调函数
	sSocketMgr.register_process(0x10, SCT_CALLBACK_2(NetManager::processLogin, this));
	// 注册消息截获事件，注册此事件后可以截获收到的所有消息，若回调函数返回true则本次事件会继续分发注册过的协议，返回false则不分发
	sSocketMgr.register_all_process(SCT_CALLBACK_1(NetManager::processAll, this));
	sSocketMgr.register_disconnect(SCT_CALLBACK_3(NetManager::onDisconnect, this));
	
	return true;
}
void NetManager::cs_Login()
{
	// 定义协议包
	WorldPacket packet;
	packet.clear();
	// 设置协议头
	packet.SetOpcode(MSG_CS_LOGIN);
	// 加入字符串数据(uint8表示字符串长度所占字节，此处为1字节)
	packet.AppendPacketString<uint8>(Player::getSingleton().getID());
	// 设置协议长度
	packet.SetLength(packet.size());
	// 发送数据
	sSocketMgr.SendPacket(LIFE_SOCKET_ID, &packet);
}
void NetManager::processLogin(int tag, WorldPacket& packet)
{
	uint16_t state;
	std::string loginmsg;
	std::string usrname;
	std::string nickname;
	std::string birthday;
	packet >> state;
	packet.GetPacketString<uint8_t>(loginmsg);

	//状态0表示验证通过
	if (state == 0)
	{
		packet.GetPacketString<uint8_t>(usrname);
		packet.GetPacketString<uint8_t>(nickname);
		packet.GetPacketString<uint8_t>(birthday);
	}
	else
	{
		return;
	}

	Player::getSingleton().setUsrname(usrname.c_str());
	Player::getSingleton().setNickname(nickname.c_str());
	Player::getSingleton().setBirthday(birthday.c_str());

	NetManager::getSingleton().cs_GetSchedule();
}

void NetManager::cs_Nickname()
{
}

void NetManager::processNicknameRes(int tag, WorldPacket& packet)
{
}

void NetManager::cs_PlayerState()
{
}
void NetManager::processPlayerStateRes(int tag, WorldPacket& packet)
{
}
void NetManager::cs_GetSchedule()
{
	WorldPacket packet;
	packet.clear();
	packet.SetOpcode(MSG_CS_GET_SCHEDULE);
	packet.AppendPacketString<uint8>(Player::getSingleton().getID());
	packet.SetLength(packet.size());
	sSocketMgr.SendPacket(LIFE_SOCKET_ID, &packet);
}
void NetManager::processGetScheduleRes(int tag, WorldPacket& packet)
{
	//typedef std::pair<uint32_t, std::string> DailyAction
	DailyAction act;
	uint32_t actionNum = 0;
	packet >> actionNum;
	for (uint16_t i = 0; i < actionNum; i++)
	{
		packet >> act.first;
		packet.GetPacketString<uint8_t>(act.second);
		Player::getSingleton().addAction(act);
	}
	GameScene::getSingleton().updateScheduleListView();
}
void NetManager::processPlayerUpdateState(int tag, WorldPacket& packet)
{
	std::string msg;
	packet.GetPacketString<uint8_t>(msg);
	GameScene::getSingleton().updatePlayerState(msg);
}
void NetManager::cs_DominoUploadTilemap()
{
	TileMap& tilemap = DominoManager::getSingleton().getTileMap();
	WorldPacket packet;
	packet.clear();
	packet.SetOpcode(MSG_CS_DOMINO_UPLOAD_TILEMAP);
	packet.AppendPacketString<uint8>(Player::getSingleton().getID());
	packet << (int)1;
	packet << (int)tilemap.size();
	packet.writeVector(tilemap);
	packet.SetLength(packet.size());
	sSocketMgr.SendPacket(LIFE_SOCKET_ID, &packet);
}
void NetManager::processDominoUploadTilemapRes(int tag, WorldPacket& packet)
{
	short ret = 0;
	packet >> ret;
	//ret == 0 表示保存成功
	//保存成功状态
	GameScene::getSingleton().touchEventEditorUISaveRes(ret);
}
void NetManager::cs_DominoDownloadTilemap()
{
	WorldPacket packet;
	packet.clear();
	packet.SetOpcode(MSG_CS_DOMINO_DOWNLOAD_TILEMAP);
	packet.AppendPacketString<uint8>(Player::getSingleton().getID());
	packet << (int)1;
	packet.SetLength(packet.size());
	sSocketMgr.SendPacket(LIFE_SOCKET_ID, &packet);
}
void NetManager::processDominoDownloadTilemapRes(int tag, WorldPacket& packet)
{
	short ret = 0;
	packet >> ret;
	if (ret == 0)
	{
		std::string tilemapstr;
		packet.GetPacketString<uint8>(tilemapstr);
		TileMap& tilemap = DominoManager::getSingleton().getTileMap();
		tilemap.clear();
		tilemap.assign(tilemapstr.begin(), tilemapstr.end());
		GameScene::getSingleton().updateDominoTilemap();
	}
}
bool NetManager::processAll(int tag, int cmd, WorldPacket& packet)
{
	switch (cmd)
	{
	case MSG_SC_LOGIN_RES:
		processLogin(tag, packet);
		break;
	case MSG_SC_CREATE_NICKNAME_RES:
		processNicknameRes(tag, packet);
		break;
	case MSG_SC_PULL_STATE_RES:
		processPlayerStateRes(tag, packet);
		break;
	case MSG_SC_GET_SCHEDULE_RES:
		processGetScheduleRes(tag, packet);
		break;
	case MSG_SC_UPDATE_PLAYER_STATE:
		processPlayerUpdateState(tag, packet);
		break;
	case MSG_SC_DOMINO_UPLOAD_RES:
		processDominoUploadTilemapRes(tag, packet);
		break;
	case MSG_SC_DOMINO_DOWNLOAD_TILEMAP_RES:
		processDominoDownloadTilemapRes(tag, packet);
		break;
	default:
		break;
	}
	return true;
}

void NetManager::onDisconnect(int tag)
{
}