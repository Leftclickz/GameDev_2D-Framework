#include "GamePCH.h"

#include "GameObjects/PlayerController.h"

void PlayerController::OnEvent(Event* pEvent)
{
    if( pEvent->GetEventType() == EventType_Input )
    {
#if WIN32
        InputEvent* pInput = (InputEvent*)pEvent;

        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard )
        {
            int keyID = pInput->GetID();

            if( pInput->GetInputState() == InputState_Pressed )
            {
                if( keyID == VK_UP    || keyID == 'W' ) { m_Forward = true; }
                if( keyID == VK_DOWN  || keyID == 'S' ) { m_Reverse = true; }
                if( keyID == VK_LEFT  || keyID == 'A' ) { m_TurnLeft = true; }
                if( keyID == VK_RIGHT || keyID == 'D' ) { m_TurnRight = true; }
            }

            if( pInput->GetInputState() == InputState_Released )
            {
                if( keyID == VK_UP    || keyID == 'W' ) { m_Forward = false; }
                if( keyID == VK_DOWN  || keyID == 'S' ) { m_Reverse = false; }
                if( keyID == VK_LEFT  || keyID == 'A' ) { m_TurnLeft = false; }
                if( keyID == VK_RIGHT || keyID == 'D' ) { m_TurnRight = false; }
            }
        }
#endif
    }
}
