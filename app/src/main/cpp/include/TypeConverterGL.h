#pragma once

#include "UniformType.h"
#include "ShaderVertexAttributeType.h"
#include "ComponentDataType.h"
#include "BufferTarget.h"
#include "BufferHint.h"
#include "EnableCap.h"
#include "FacetCulling.h"
#include "RasterizationMode.h"
#include "StencilFace.h"
#include "StencilTestFace.h"
#include "DepthTestFunction.h"
#include "SourceBlendingFactor.h"
#include "DestinationBlendingFactor.h"
#include "BlendEquation.h"
#include "ClearBuffers.h"
#include "IndexDataType.h"
#include "PrimitiveType.h"
#include "TextureTarget.h"
#include "TextureFormat.h"
#include "TextureMinificationFilter.h"
#include "TextureMagnificationFilter.h"
#include "TextureWrap.h"
#include "PixelBufferHint.h"
#include "ImageDataType.h"
#include "ImageFormat.h"
#include "FrameBufferAttachment.h"
#include "BufferAccess.h"
#include "VertexAttributeType.h"
#include "RenderBufferFormat.h"

namespace argeo
{
	class TypeConverterGL
	{
	public:

		static unsigned int ToEnum(RenderBufferFormat type);
		static unsigned int ToEnum(VertexAttributeType type);
		static unsigned int ToEnum(BufferAccess type);
		static unsigned int ToEnum(ImageFormat type);
		static unsigned int ToEnum(ImageDataType type);
		static unsigned int ToEnum(UniformType type);
		static unsigned int ToEnum(IndexDataType type);
		static unsigned int ToEnum(ShaderVertexAttributeType type);
		static unsigned int ToEnum(ComponentDataType type);
		static unsigned int ToEnum(BufferHint type);
		static unsigned int ToEnum(BufferTarget type);
		static unsigned int ToEnum(EnableCap type);
		static unsigned int ToEnum(CullFace type);
		static unsigned int ToEnum(WindingOrder type);
		static unsigned int ToEnum(RasterizationMode type);
		static unsigned int ToEnum(StencilFace type);
		static unsigned int ToEnum(StencilOperation type);
		static unsigned int ToEnum(StencilTestFunction type);
		static unsigned int ToEnum(SourceBlendingFactor type);
		static unsigned int ToEnum(DestinationBlendingFactor type);
		static unsigned int ToEnum(BlendEquation type);
		static unsigned int ToEnum(DepthTestFunction type);
		static unsigned int ToEnum(ClearBuffers type);
		static unsigned int ToEnum(PrimitiveType type);
		static unsigned int ToEnum(FrameBufferAttachment type);
			   
		static unsigned int ToEnum(TextureTarget type);
		static unsigned int TextureToPixelFormat(TextureFormat type);
		static unsigned int TextureToPixelType(TextureFormat type);
		static unsigned int ToEnum(TextureFormat type);
		static unsigned int ToEnum(TextureWrap type);
		static unsigned int ToEnum(TextureMagnificationFilter type);
		static unsigned int ToEnum(TextureMinificationFilter type);

		static BufferHint ReadPixelHintToBufferHint(PixelBufferHint pixel_hint);
		static BufferHint WritePixelHintToBufferHint(PixelBufferHint pixel_hint);
		static UniformType ToUniformType(unsigned int type);
		static ShaderVertexAttributeType ToShaderVertexAttributeType(unsigned int type);
		static ComponentDataType ToComponentDataType(unsigned int type);
		static BufferTarget ToBufferTarget(unsigned int type);
		static BufferHint ToBufferHint(unsigned int type);
	};
}
