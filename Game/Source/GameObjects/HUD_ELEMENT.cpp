#include "GamePCH.h"
#include "HUD_Element.h"

HUD_ELEMENT::HUD_ELEMENT()
{
	b_FollowsCamera = true;
	b_ScreenOffset = true;
	m_CameraToFollow = CAMERA::GetPlayerCamera();
}

vec2 HUD_ELEMENT::GetHUDAnchor()
{
	vec2 anchor(0.0f, 0.0f);

	if (b_FollowsCamera)
		if (m_CameraToFollow != nullptr)
			anchor += m_CameraToFollow->GetPosition();

	if (b_ScreenOffset)
		anchor += GetScreenOffset();

	return anchor;
}
