#include "stdafx.h"
#include "MeshRenderer.h"

MeshRenderer gMeshRenderer;

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Release()
{
	delete this;
}

bool MeshRenderer::Init()
{
	return false;
}

void MeshRenderer::Begin()
{
}

void MeshRenderer::End()
{

}

void MeshRenderer::Draw(IRenderer::Entity* entity)
{
}
