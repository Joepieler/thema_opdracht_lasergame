#ifndef BUTTON_LISTENER_HPP
#define BUTTON_LISTENER_HPP


class ButtonListener{
public:
    virtual void buttonDetected(const char * buttonname)=0;
    
};

#endif
