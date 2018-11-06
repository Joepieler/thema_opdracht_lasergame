#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

struct ir_msg {
	uint8_t player;
	uint8_t data;
};

class MsgListener {
public:
	virtual void msgReceived( const ir_msg & msg ) = 0;
};

#endif // MSG_LISTENER_HPP