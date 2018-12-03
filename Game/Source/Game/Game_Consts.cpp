#include "GamePCH.h"
#include "GamePCH.h"
#include "Game_Consts.h"

ShaderProgram* SHADERS::TEXTURE_SHADER_PROGRAM = nullptr;
ShaderProgram* SHADERS::DEBUG_SHADER_PROGRAM = nullptr;
ShaderProgram* SHADERS::CANVAS_SHADER_PROGRAM = nullptr;

Mesh* MESHES::TILEMESH_DEFAULT_SIZE = nullptr;
Mesh* MESHES::TILEMESH_WALL_SIZE = nullptr;

Camera* CAMERA::PLAYER_CAMERA = nullptr;

void CAMERA::CreateCameras(Game* game)
{
	PLAYER_CAMERA = new Camera(game); 
	PLAYER_CAMERA->SetOwner(game->GetPlayer());
	PLAYER_CAMERA->SetProjection(PROJECTION);
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

void MESHES::CreateDefaultMeshes()
{
	TILEMESH_DEFAULT_SIZE = new Mesh(TILE::TILE_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
	TILEMESH_WALL_SIZE = new Mesh(TILE::WALL_MESH, TILE::TILE_VERT_COUNT, GL_TRIANGLE_STRIP);
}

void MESHES::DestroyMeshes()
{
	delete TILEMESH_DEFAULT_SIZE;
	delete TILEMESH_WALL_SIZE;
}

void MESHES::SetMeshDebugMode(bool value)
{
	TILEMESH_WALL_SIZE->SetDrawDebugLines(value);
	TILEMESH_DEFAULT_SIZE->SetDrawDebugLines(value);
}
