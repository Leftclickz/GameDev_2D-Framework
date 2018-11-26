#include "GamePCH.h"
#include "GamePCH.h"

ShaderProgram* SHADERS::TEXTURE_SHADER_PROGRAM = nullptr;
ShaderProgram* SHADERS::DEBUG_SHADER_PROGRAM = nullptr;
ShaderProgram* SHADERS::CANVAS_SHADER_PROGRAM = nullptr;

Camera* CAMERA::PLAYER_CAMERA = nullptr;

void CAMERA::CreateCameras(Game* game)
{
	PLAYER_CAMERA = new Camera(game); 
	PLAYER_CAMERA->SetOwner(game->GetPlayer());
	PLAYER_CAMERA->SetProjection(1 / HALF_SCREEN);
}

void CAMERA::DestroyCameras()
{
	if (PLAYER_CAMERA != nullptr)
		delete PLAYER_CAMERA;
	PLAYER_CAMERA = nullptr;
}

Camera* CAMERA::GetPlayerCamera()
{
	return PLAYER_CAMERA; 
}
