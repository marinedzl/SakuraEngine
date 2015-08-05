#pragma once

class Skeleton : public TRefObject<RefObject>
{
public:
	Skeleton();
	virtual ~Skeleton();
	virtual bool LoadFromFile(const char* filename);
	size_t GetBoneCount() const { return mBoneCount; }
	const Matrix& GetInverseTM(size_t index) const { return mInverseTMs[index]; }
private:
	size_t mBoneCount;
	std::vector<Matrix> mInverseTMs;
};
