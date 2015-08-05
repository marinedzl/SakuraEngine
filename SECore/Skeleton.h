#pragma once

class Skeleton : public TRefObject<RefObject>
{
public:
	Skeleton();
	virtual ~Skeleton();
	virtual bool LoadFromFile(const char* filename);
private:
	size_t mBoneCount;
	std::vector<Matrix> mInverseTMs;
};
