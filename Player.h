#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include "Singleton.h"

typedef std::pair<unsigned int, std::string> DailyAction;
typedef std::vector<DailyAction> DailyActions;

class Player : public Singleton<Player>
{
public:
	Player();
	~Player();
	const char* getID() const;
	const char* getUsrname() const;
	void setUsrname(const char* name);
	const char* getNickname() const;
	void setNickname(const char* name);
	const char* getBirthday() const;
	void setBirthday(const char* birthday);
	void addAction(DailyAction& act);
	bool hasSchedule() const;
	const DailyActions& getDailySchedule() const;

private:
	char mID[33];
	char mUsrname[64];
	char mNickname[32];
	char mBirthday[64];
	DailyActions mSchedule;
};


#endif