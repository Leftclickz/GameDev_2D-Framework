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

	void SetUp(bool value = false) { m_Forward = false; }
	void SetDown(bool value = false) { m_Reverse = false; }
	void SetLeft(bool value = false) { m_TurnLeft = false; }
	void SetRight(bool value = false) { m_TurnRight = false; }
};


#endif //__PlayerController_H__
