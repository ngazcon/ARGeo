#pragma once

namespace argeo
{
	class PolylineBucket;

	struct VertexArrayBucketLocator
	{
		VertexArrayBucketLocator(
			unsigned int count,
			unsigned int offset,
			PolylineBucket* bucket)
			: count(count)
			, offset(offset)
			, bucket(bucket)
		{ }

		unsigned int count;
		unsigned int offset;
		PolylineBucket* bucket;
	};
}