#ifndef IKEYPAD_H
#define IKEYPAD_H

class IKeypad
{
    public:
        virtual ~IKeypad() = default;
        virtual bool EnteringPin() = 0;
};


#endif