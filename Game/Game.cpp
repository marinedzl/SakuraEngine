#include "stdafx.h"
#include "CameraCtrl.h"
#include "CharaCtrl.h"
#include "Game.h"

Game gGame;

Game::Game()
	: mCameraCtrl(nullptr)
	, mCharaCtrl(nullptr)
	, mCamera(nullptr)
	, mRT(nullptr)
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
	SAFE_RELEASE(mRT);
}

bool Game::Init(SECore::Core* core, HWND hWnd, int w, int h)
{
	this->hWnd = hWnd;
	mCore = core;
	mRT = core->CreateRenderTarget(hWnd, w, h);
	return true;
}

void Game::Resize(int w, int h)
{
	if (w == 0 || h == 0)
		return;
	if (mRT)
	{
		if (mRT->GetWidth() == w && mRT->GetHeight() == h)
			return;
		SAFE_RELEASE(mRT);
	}
	mRT = mCore->CreateRenderTarget(hWnd, w, h);
	mCamera->SetView(mRT->GetWidth(), mRT->GetHeight());
}

bool Game::EditorPlay(SECore::Scene* scene)
{
	mScene = scene;

	GameObject* mainChara = scene->FindEntity("main chara");
	GameObject* weapon = scene->FindEntity("weapon");
	Light* light = scene->FindLight("chara light");

	mCamera = mCore->CreateCamera();
	mCamera->SetView(mRT->GetWidth(), mRT->GetHeight());

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
	if (mScene && mRT)
	{
		mScene->Draw(mCamera, mRT);
	}
}

void Game::Update(float deltaTime)
{
	Vector3 offset(0, 2, 0);

	if (mScene)
		mScene->Update(deltaTime);

	if (mCharaCtrl)
		mCharaCtrl->Update(deltaTime);

	if (mCameraCtrl)
	{
		mCameraCtrl->Update(deltaTime);
		if (mMainChara)
			mCameraCtrl->FocusOn(mMainChara->GetTransform().position + offset);
	}
}
