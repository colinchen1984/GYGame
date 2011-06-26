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
	pthread_mutex_init(&m_conditionMutex,NULL);
	pthread_cond_init(&m_condition,NULL);
};

GYThreadCondition::~GYThreadCondition()
{
	pthread_mutex_destroy(&m_conditionMutex);
	pthread_cond_destroy(&m_condition);
}

GYVOID GYThreadCondition::Acquire()
{
	pthread_cond_wait(&m_condition, &m_conditionMutex);
};

GYVOID GYThreadCondition::Signal()
{
	pthread_cond_wait(&m_condition, &m_conditionMutex);
}
#endif
