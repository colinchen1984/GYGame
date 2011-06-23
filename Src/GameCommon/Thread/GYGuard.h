/////////////////////////////////////////////
// create time: 2011/6/23 23:13
// author:	colin chen
// file name:	GYGuard
// file type:	h
////////////////////////////////////////////

#ifndef __GYGUARD_H__
#define __GYGUARD_H__

//using declare

//foward declare

template<typename lock>
class GYGuard :
{
	lock& _l;
public:
	GYGuard(lock& l) : _l(l) 
	{ 
		_l.Acquire(); 
	};
	
	~GYGuard() 
	{ 
		_l.Release(); 
	};

};

#endif
