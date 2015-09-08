// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "Base.h"


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "../SECore/SECore.h"
#include "IGame.h"

typedef SECore::Color Color;
typedef SECore::Vector2 Vector2;
typedef SECore::Vector3 Vector3;
typedef SECore::Vector4 Vector4;
typedef SECore::Quat Quat;
typedef SECore::Matrix Matrix;
typedef SECore::Texture Texture;
typedef SECore::Camera Camera;
typedef SECore::Scene::Entity GameObject;
typedef SECore::Light Light;
typedef SECore::Animation Animation;
typedef SECore::Transform Transform;
typedef SECore::Ray Ray;
typedef SECore::RaycastHit RaycastHit;
typedef SECore::RigidBody RigidBody;
typedef SECore::CharacterController CharacterController;
typedef SECore::Core Core;
typedef SECore::Scene Scene;
typedef SECore::RenderTarget RenderTarget;
