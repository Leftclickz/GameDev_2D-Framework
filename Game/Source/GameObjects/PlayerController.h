#ifndef __PlayerController_H__
#define __PlayerController_H__

class Event;

class PlayerController
{
protected:
    bool m_Forward;
    bool m_Reverse;
    bool m_TurnLeft;
    bool m_TurnRight;

public:
    void OnEvent(Event* pEvent);

    bool IsForwardHeld() { return m_Forward; }
    bool IsReverseHeld() { return m_Reverse; }
    bool IsTurnLeftHeld() { return m_TurnLeft; }
    bool IsTurnRightHeld() { return m_TurnRight; }
};


#endif //__PlayerController_H__
