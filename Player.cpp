#include "Player.h"
#include "string.h"

template<> Player* Singleton<Player>::msSingleton = 0;

Player::Player()
{
	strcpy(mID, "9c927504a1147d0e4269fcbddede9438");
	mSchedule.reserve(32);

	//mSchedule.push_back(DailyAction(1, "spider1"));
	//mSchedule.push_back(DailyAction(2, "spider3"));
	//mSchedule.push_back(DailyAction(3, "spider4"));
	//mSchedule.push_back(DailyAction(4, "spider5"));
	//mSchedule.push_back(DailyAction(5, "spider6"));
	//mSchedule.push_back(DailyAction(6, "spider7"));
	//mSchedule.push_back(DailyAction(7, "spider8"));
	//mSchedule.push_back(DailyAction(8, "spider9"));
	//mSchedule.push_back(DailyAction(9, "spider0"));
	//mSchedule.push_back(DailyAction(10, "spider10"));
	//mSchedule.push_back(DailyAction(11, "spider11"));
	//mSchedule.push_back(DailyAction(12, "spider12"));
}

Player::~Player()
{
}


const char* Player::getID() const
{
	return mID;
}

const char* Player::getUsrname() const
{
	return mUsrname;
}

void Player::setUsrname(const char* name)
{
	memcpy(mUsrname, name, strlen(name));
}

const char* Player::getNickname() const
{
	return mNickname;
}

void Player::setNickname(const char* name)
{
	memcpy(mNickname, name, strlen(name));
}

const char* Player::getBirthday() const
{
	return mBirthday;
}

void Player::setBirthday(const char* birthday)
{
	memcpy(mBirthday, birthday, strlen(birthday));
}

void Player::addAction(DailyAction& act)
{
	mSchedule.push_back(act);
}
bool Player::hasSchedule() const
{
	return (mSchedule.size() != 0);
}
const DailyActions& Player::getDailySchedule() const
{
	return mSchedule;
}