#ifndef FRAME_EVENT_H
#define FRAME_EVENT_H

#include "Event.h"

class EventFrameBegin : public Event
{
public:
	EventFrameBegin();
	virtual ~EventFrameBegin();
};

#endif