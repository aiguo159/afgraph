/*
  single object scheduler, implementation

  Aiguo Fei, UCLA 2001

  derived and simplified from a more general event scheduler
    done at Bell Labs in 1998
*/

#include <algorithm>
#include <string>

#include <news/sscheduler.h>

namespace news
{

bool SScheduler::schedule_event( SSEvent &event )
{
	// put an event in eventlist
	if( event.get_time() >= m_Time ) {
		m_EventList.push( event );
		return true;
	}
	return false;
}

bool SScheduler::de_schedule_event( SSEvent &event )
{
	// remove an event from list
	afl::sorted_list<SSEvent>::iterator it =
		std::find( m_EventList.begin(), m_EventList.end(), event );
	if( it != m_EventList.end() ) {
		// got to remove the associated pkt
		if( it->m_pPkt ) {
			delete it->m_pPkt;
		}
		m_EventList.erase( it );
		return true;
	}
	return false; // no such event
}


bool SSimulator::run( void )
{
	if( !m_pObj ) {
		return false;
	}

	if( !m_pObj->init( this, m_Time ) ) {
		return false;
	}

	while( m_Time < m_EndTime && !m_EventList.empty() ) {
		// get an event to dispatch
		SSEvent event = m_EventList.front();
		m_EventList.pop_front();
		// remove that event from eventlist
		// remove it first because the networkobj which handles that event
		// may insert event to trigger at the same time of that event
		m_Time = event.get_time(); // update the time of the scheduler

		m_pObj->handle_event( event );
	}

	// clear all events left
	while( !m_EventList.empty() ) {
		// get an event to dispatch
		SSEvent event = m_EventList.front();
		m_EventList.pop_front();
		if( event.get_pkt() ) {
			delete event.get_pkt();
		}
	}

	m_pObj->stop( m_Time );

	return true;
}

} // end of namespace news
