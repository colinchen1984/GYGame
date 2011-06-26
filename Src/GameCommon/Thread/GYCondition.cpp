/////////////////////////////////////////////
// create time: 2011/6/26 8:17
// author:	colin chen
// file name:	GYCondition
// file type:	cpp
////////////////////////////////////////////
#ifdef LINUX64
#include "GYCondition.h"

GYThreadCondition::GYThreadCondition()
{
	pthread_mutex_init(&cond_mutex,NULL);
	pthread_cond_init(&cond,NULL);
};

GYThreadCondition::~GYThreadCondition()
{
	pthread_mutex_destroy(&cond_mutex);
	pthread_cond_destroy(&cond);
}

GYVOID GYThreadCondition::Acquire()
{
	pthread_cond_wait(&cond, &cond_mutex);
};

GYVOID GYThreadCondition::Signal()
{
	pthread_cond_wait(&cond, &cond_mutex);
}
#endif
