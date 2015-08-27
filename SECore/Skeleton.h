#pragma once

class Skeleton : public TRefObject<SECore::Skeleton>
{
public:
	Skeleton();
	virtual ~Skeleton();
	virtual bool LoadFromFile(const char* filename);
	size_t GetBoneCount() const { return mBoneCount; }
	const Matrix& GetInverseTM(size_t index) const { return mInverseTMs[index]; }
	UINT GetBoneID(const char* name) const;
private:
	typedef std::map<std::string, UINT> BoneNames;
private:
	size_t mBoneCount;
	BoneNames mBoneNames;
	std::vector<Matrix> mInverseTMs;
};
