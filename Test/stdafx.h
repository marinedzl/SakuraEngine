// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#include <timeapi.h>
#include <WinUser.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <DirectXMath.h>
using namespace DirectX;
#include "Base.h"
#include "../SECore/SECore.h"
#include <json\json.h>

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

class AnimationPlayer;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

class buffer
{
public:
	buffer();
	~buffer();
	void clear();
	void resize(size_t size);
	char* ptr() { return _ptr; }
	const char* ptr() const { return _ptr; }
	size_t size() const { return _size; }
private:
	char* _ptr;
	size_t _size;
};

bool LoadBinaryFile(buffer& buff, const char* filename);
bool LoadTextFile(buffer& buff, const char* filename);
