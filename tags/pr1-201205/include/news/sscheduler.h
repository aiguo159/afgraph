/*@copyright

Copyright (c) 2001 Aiguo Fei, University of California, Los Angeles
Copyright (c) 2012 Aiguo Fei
All rights reserved.

Permission to use and re-distribute this code in source and binary forms
with or without modifications is therefore granted provided that
the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Information regarding where to obtain the original source code must be
      included with the redistrbution.

*NO WARRANTY*
THIS CODE OR PROGRAM IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND CONTRIBUTOR(S)
"AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE. THE COPYRIGHT HOLDER(S) AND CONTRIBUTOR(S) MUST NOT BE
HELD RESPONSIBLE FOR ANY DAMAGE OR UNDESIRED EFFECT CAUSED DIRECTLY OR
INDIRECTLY BY USE, MISUSE OR ABUSE OF THIS CODE OR PROGRAM.

*/


/**@file sscheduler.h

  single object scheduler

  Aiguo Fei, UCLA 2001

  derived and simplified from a more general event scheduler
    done at Bell Labs in 1998
*/

#ifndef _NEWS_SSCHEDULER_H_
#define _NEWS_SSCHEDULER_H_

#include <afl/sorted_list.hpp>

/**@defgroup news NEWS: a New nEtWork Simulator
*/

/** a New nEtWork Simulator.

*/
namespace news
{

/**@defgroup ssch single object scheduler
@ingroup news
*/
/**@{*/


// type definitions
/// timestamp
typedef double TimeS;
/// elapsed time
typedef double TimeE;
// use these type definitions, so it will be easy to change them to other
// more complicated types (such as use real time as timestamp)


/** basic definition for a packet object.
  All packet objects should be derived from it and
  implement the clone() member function.
*/
class PktObj
{
public:
	PktObj()
	{ }
	virtual ~PktObj()
	{}
	virtual PktObj *clone( void ) = 0;
};

/** Event type for a single object scheduler.
*/
class SSEvent
{
public:
	SSEvent( int ntype = -1, TimeS stime = -1,
			 TimeS etime = -1, PktObj *pp = NULL, int key = 0 ) {
		m_nEventType = ntype;
		m_ScheduleTime = stime;
		m_EventTime = etime;
		m_pPkt = pp;
		m_nKey = key;
	}

	inline int get_type() const {
		return m_nEventType;
	}
	inline int get_key() const {
		return m_nKey;
	}
	inline TimeS get_time() const {
		return m_EventTime;
	}
	inline TimeS get_stime() const {
		return m_ScheduleTime;
	}
	inline PktObj *get_pkt() {
		return m_pPkt;
	}
	//  void set_time(TimeS time) { m_EventTime=time; }

	inline friend bool operator < ( const SSEvent &lhs, const SSEvent &rhs )
	// the less-than operator, needed to tell which event to go first
	{
		return ( lhs.m_EventTime < rhs.m_EventTime );
	}

	inline friend bool operator > ( const SSEvent &lhs, const SSEvent &rhs ) {
		return ( lhs.m_EventTime > rhs.m_EventTime );
	}

	inline friend bool operator != ( const SSEvent &lhs, const SSEvent &rhs ) {
		return (( lhs.m_nEventType != rhs.m_nEventType )
				|| ( lhs.m_nKey != rhs.m_nKey ) );
	}

	inline friend bool operator == ( const SSEvent &lhs, const SSEvent &rhs ) {
		return ( lhs.m_nEventType == rhs.m_nEventType
				 && lhs.m_nKey == rhs.m_nKey );
	}

private:
	int m_nEventType;
	int m_nKey;

	TimeS m_EventTime; // time to be handled
	TimeS m_ScheduleTime; // time when it is scheduled

	PktObj *m_pPkt;
	// the packet related to this event, e.g. a packet a gateway should forward
	// can be used to pass information from generator to target
	// can be null for some event types

	friend class SScheduler;
};

/** Single object scheduler.
*/
class SScheduler
{
public:
	SScheduler( TimeS starttime ) {
		m_Time = starttime;
	}
	TimeS get_time( ) {
		return m_Time;
	}

	bool schedule_event( SSEvent &event );

	bool de_schedule_event( SSEvent &event );

protected:
	TimeS m_Time;
	afl::sorted_list<SSEvent> m_EventList;
	// events are kept in order of eventtime (not schedule time)
};


/** Basic network object for the single object scheduler.
*/
class SSNetObj
{
public:
	SSNetObj( )
	{  }

	virtual ~SSNetObj( )
	{ }

	virtual int handle_event( SSEvent & ) = 0;
	// every network object should have this function defined to handle
	// events passed to it
	// make it an abstract class, so no network object can be declared
	// without implementing something useful

	inline virtual bool init( SScheduler *psch, TimeS ctime ) {
		if( !psch ) {
			return false;
		}
		m_pscheduler = psch;
		return true;
	}
	inline virtual int stop( TimeS ctime ) {
		return 0;
	}

protected:
	SScheduler *m_pscheduler;
	//scheduler to which a networkobj can send event scheduling request

};


/** Single object simulator.
How to use:
*/
class SSimulator: public SScheduler
{
public:
	SSimulator( SSNetObj *pobj, TimeS stime, TimeS etime )
		: SScheduler( stime ), m_pObj( pobj ), m_EndTime( etime )
	{  }

	bool run( void );

protected:
	SSNetObj *m_pObj;
	TimeS m_EndTime; // time the simulation should terminate
};

/**@}*/

} // end of namespace news

#endif
