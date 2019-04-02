#ifndef ENGINE_TIMER_H
#define ENGINE_TIMER_H

class EngineTimer
{
public:
    virtual float totalTime() = 0;
	virtual float deltaTime() = 0;

	virtual void reset() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void tick() = 0;
	virtual float getFPS() = 0;
};

#endif