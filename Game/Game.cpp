#include "stdafx.h"
#include "CameraCtrl.h"
#include "CharaCtrl.h"
#include "Game.h"

Game gGame;

Game::Game()
	: mCameraCtrl(nullptr)
	, mCharaCtrl(nullptr)
	, mCamera(nullptr)
	, m_pRenderTarget(nullptr)
{
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_DELETE(mCameraCtrl);
	SAFE_DELETE(mCharaCtrl);
	SAFE_RELEASE(mCamera);
	SAFE_RELEASE(m_pRenderTarget);
}

bool Game::Init(SECore::Core* core, HWND hWnd)
{
	mCore = core;
	m_pRenderTarget = core->CreateRenderTarget(hWnd);
	return true;
}

bool Game::EditorPlay(SECore::Scene* scene)
{
	mScene = scene;

	GameObject* mainChara = scene->FindEntity("main chara");
	GameObject* weapon = scene->FindEntity("weapon");
	Light* light = scene->FindLight("chara light");

	mCamera = mCore->CreateCamera();

	if (mainChara)
	{
		mMainChara = mainChara;
		mCharaCtrl = new CharaCtrl(scene, *mainChara, weapon, light);
	}

	mCameraCtrl = new CameraCtrl(mCamera);

	return true;
}

void Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (mCameraCtrl)
		mCameraCtrl->WndProc(hWnd, message, wParam, lParam);

	if (mMainChara)
		mCharaCtrl->WndProc(hWnd, message, wParam, lParam);
}

void Game::Draw()
{
	if (mScene && m_pRenderTarget)
	{
		m_pRenderTarget->Begin();
		mScene->Draw(mCamera, m_pRenderTarget);
		m_pRenderTarget->End();
	}
}

void Game::Update(float deltaTime)
{
	Vector3 offset(0, 2, 0);

	if (mCharaCtrl)
		mCharaCtrl->Update(deltaTime);

	if (mCameraCtrl)
	{
		mCameraCtrl->Update(deltaTime);
		if (mMainChara)
			mCameraCtrl->FocusOn(mMainChara->GetTransform().position + offset);
	}
}
