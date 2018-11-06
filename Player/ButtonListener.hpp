#ifndef BUTTON_LISTENER_HPP
#define BUTTON_LISTENER_HPP

class ButtonListener {
public:
    virtual void buttonPressed( unsigned int & buttonnumber ) = 0;
    
};

#endif