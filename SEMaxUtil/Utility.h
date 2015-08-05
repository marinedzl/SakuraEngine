#pragma once

std::string WStr2MStr(const std::wstring& src);
std::wstring MStr2WStr(const std::string& src);

bool CreateMultiDir(const TCHAR* lpDir);

namespace MaxPlugin
{
	struct Vec2
	{
		float x, y;
	};

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

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

	bool LoadBinaryFile(buffer& buff, const TCHAR* filename);
	bool LoadTextFile(buffer& buff, const TCHAR* filename);
	bool LoadJsonFromFile(const TCHAR* filename, Json::Value& root);
	bool SaveJsonToFile(const TCHAR* filename, const Json::Value& root);

	struct Utility
	{
		static void ToVec3(Vec3& dst, const Point3& src)
		{
			dst.x = src.x;
			dst.y = src.y;
			dst.z = src.z;
		}

		static void ToVec4(Vec4& dst, const Quat& src)
		{
			dst.x = src.x;
			dst.y = src.y;
			dst.z = src.z;
			dst.w = src.w;
		}
	};
}
