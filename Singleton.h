#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

// Copied frome Ogre::Singleton, created by Steve Streeting for Ogre

template <typename T> class Singleton
{
protected:
	static T* msSingleton;

public:
	Singleton(){
		assert(!msSingleton);
		msSingleton = static_cast< T* >(this);
	}
	~Singleton(){
		assert(msSingleton);
		msSingleton = 0;
	}
	static T& getSingleton(){
		assert(msSingleton);
		return (*msSingleton);
	}
	static T* getSingletonPtr(){
		return msSingleton;
	}
};

#endif