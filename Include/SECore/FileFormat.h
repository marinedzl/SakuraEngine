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
