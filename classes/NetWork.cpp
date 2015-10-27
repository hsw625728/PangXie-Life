#include "NetWork.h"
#include "Protocol.h"
#include "GameScene.h"
#include "Player.h"

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
	// ����SOCKET������
	CREATE_TCPSOCKETMGR(this);
	// ����������SOCKET��������������IP���˿ڣ��Զ����SOCKET_ID��ʶ
	sSocketMgr.createSocket("123.57.53.187", 13001, LIFE_SOCKET_ID);
	// ע��Э�飬��������ͷ���ص�����
	sSocketMgr.register_process(0x10, SCT_CALLBACK_2(NetManager::processLogin, this));
	// ע����Ϣ�ػ��¼���ע����¼�����Խػ��յ���������Ϣ�����ص���������true�򱾴��¼�������ַ�ע�����Э�飬����false�򲻷ַ�
	sSocketMgr.register_all_process(SCT_CALLBACK_1(NetManager::processAll, this));
	sSocketMgr.register_disconnect(SCT_CALLBACK_3(NetManager::onDisconnect, this));
	
	return true;
}
void NetManager::cs_Login()
{
	// ����Э���
	WorldPacket packet;
	packet.clear();
	// ����Э��ͷ
	packet.SetOpcode(MSG_CS_LOGIN);
	// �����ַ�������(uint8��ʾ�ַ���������ռ�ֽڣ��˴�Ϊ1�ֽ�)
	packet.AppendPacketString<uint8>(Player::getSingleton().getID());
	// ����Э�鳤��
	packet.SetLength(packet.size());
	// ��������
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

	//״̬0��ʾ��֤ͨ��
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
	default:
		break;
	}
	return true;
}

void NetManager::onDisconnect(int tag)
{
}