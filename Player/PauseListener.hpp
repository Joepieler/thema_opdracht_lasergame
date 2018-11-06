#ifndef PAUSE_LISTENER_HPP
#define PAUSE_LISTENER_HPP

class PauseListener{
public:
	virtual void pauseDetected(int pause_length) = 0;
};

#endif // PAUSE_LISTENER_HPP