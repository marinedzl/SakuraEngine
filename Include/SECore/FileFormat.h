#pragma once

namespace MeshFile
{
	struct Head
	{
		unsigned int version;
		unsigned int indexCount;
		unsigned int vertexCount;
		unsigned int blockCount;
	};

	struct Block
	{
		enum Type
		{
			Face,
			Position,
			Normal,
			Tangent,
			TexCoords0,
			TexCoords1,
			TexCoords2,
			BoneWeights,
		};
		Type type;
		unsigned int size;
	};
}

namespace AnimationFile
{
	struct Head
	{
		unsigned int version;
		unsigned int frameCount;
		unsigned int boneCount;
	};
}

namespace SkeletonFile
{
	enum
	{
		MaxBoneName = 32,
	};

	struct Head
	{
		unsigned int version;
		unsigned int boneCount;
	};

	struct Bone
	{
		char name[MaxBoneName];
		float pos[3];
		float rot[4];
	};
}
