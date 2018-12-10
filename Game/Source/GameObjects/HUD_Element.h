#pragma once

class Camera;

//interface to give objects implementation to attach to the camera seamlessly
class HUD_ELEMENT

{

public:

	HUD_ELEMENT();

	virtual void SetToFollowCamera(bool value = false) { b_FollowsCamera = value; }
	virtual void SetCameraToFollow(Camera* camera = CAMERA::GetPlayerCamera()) { m_CameraToFollow = camera; }

	virtual bool IsFollowingCamera() { return b_FollowsCamera; }
	virtual Camera* GetCameraToFollow() { return m_CameraToFollow; }

	virtual void SetToOffsetScreen(bool value = true) { b_ScreenOffset = value; }
	virtual bool IsOffsettingScreen() { return b_ScreenOffset; }

	virtual vec2 GetScreenOffset() { return -HALF_SCREEN; }

protected:

	virtual vec2 GetHUDAnchor();

	bool b_FollowsCamera;
	bool b_ScreenOffset;
	Camera* m_CameraToFollow;

};