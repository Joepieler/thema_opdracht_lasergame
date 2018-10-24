#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

struct ir_msg {
	uint8_t player;
	uint8_t weapon;
};

class msg_listener {
public:
	virtual void msg_received( const uint16_t & msg ) = 0;
};

#endif // MSG_LISTENER_HPP