#include "stdafx.h"
#include "CameraCtrl.h"
#include "CharaCtrl.h"
#include "Game.h"

Game::Game()
	: mCameraCtrl(nullptr)
	, mCharaCtrl(nullptr)
{
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_DELETE(mCameraCtrl);
	SAFE_DELETE(mCharaCtrl);
	delete this;
}

bool Game::Init(SECore::Core* core)
{
	return true;
}

bool Game::EditorPlay(SECore::Scene* scene)
{
	mScene = scene;

	GameObject* mainChara = scene->FindEntity("main chara");
	GameObject* weapon = scene->FindEntity("weapon");
	Light* light = scene->FindLight("chara light");

	if (mainChara)
	{
		mMainChara = mainChara;
		mCharaCtrl = new CharaCtrl(scene, *mainChara, weapon, light);
	}

	mCameraCtrl = new CameraCtrl(scene->GetCamera());

	return true;
}

void Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (mCameraCtrl)
		mCameraCtrl->WndProc(hWnd, message, wParam, lParam);

	if (mMainChara)
		mCharaCtrl->WndProc(hWnd, message, wParam, lParam);
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
