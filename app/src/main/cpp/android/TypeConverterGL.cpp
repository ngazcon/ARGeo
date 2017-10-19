#include "TypeConverterGL.h"
#include "GL.h"

#include <stdexcept>

namespace argeo
{
	unsigned int TypeConverterGL::ToEnum(RenderBufferFormat type)
	{
		switch (type)
		{
		case RenderBufferFormat::DepthComponent16:
			return GL_DEPTH_COMPONENT16;
		case RenderBufferFormat::DepthStencil:
			return GL_DEPTH_STENCIL;
		}
	}

	unsigned int TypeConverterGL::ToEnum(FrameBufferAttachment type)
	{
		switch (type)
		{
		case FrameBufferAttachment::ColorAttachment0:
			return GL_COLOR_ATTACHMENT0;
		case FrameBufferAttachment::ColorAttachment1:
			return GL_COLOR_ATTACHMENT1;
		case FrameBufferAttachment::ColorAttachment2:
			return GL_COLOR_ATTACHMENT2;
		case FrameBufferAttachment::ColorAttachment3:
			return GL_COLOR_ATTACHMENT3;
		case FrameBufferAttachment::ColorAttachment4:
			return GL_COLOR_ATTACHMENT4;
		case FrameBufferAttachment::ColorAttachment5:
			return GL_COLOR_ATTACHMENT5;
		case FrameBufferAttachment::ColorAttachment6:
			return GL_COLOR_ATTACHMENT6;
		case FrameBufferAttachment::ColorAttachment7:
			return GL_COLOR_ATTACHMENT7;
		case FrameBufferAttachment::ColorAttachment8:
			return GL_COLOR_ATTACHMENT8;
		case FrameBufferAttachment::ColorAttachment9:
			return GL_COLOR_ATTACHMENT9;
		case FrameBufferAttachment::ColorAttachment10:
			return GL_COLOR_ATTACHMENT10;
		case FrameBufferAttachment::ColorAttachment11:
			return GL_COLOR_ATTACHMENT11;
		case FrameBufferAttachment::ColorAttachment12:
			return GL_COLOR_ATTACHMENT12;
		case FrameBufferAttachment::ColorAttachment13:
			return GL_COLOR_ATTACHMENT13;
		case FrameBufferAttachment::ColorAttachment14:
			return GL_COLOR_ATTACHMENT14;
		case FrameBufferAttachment::ColorAttachment15:
			return GL_COLOR_ATTACHMENT15;
		case FrameBufferAttachment::DepthAttachment:
			return GL_DEPTH_ATTACHMENT;
		case FrameBufferAttachment::DepthStencilAttachment:
			return GL_DEPTH_STENCIL_ATTACHMENT;

		}
	}

	BufferHint TypeConverterGL::ReadPixelHintToBufferHint(PixelBufferHint pixel_hint)
	{
		switch (pixel_hint)
		{
		case PixelBufferHint::Dynamic:
			return BufferHint::DynamicRead;
		case PixelBufferHint::Static:
			return BufferHint::StaticRead;
		case PixelBufferHint::Stream:
			return BufferHint::StreamRead;
		}

		throw std::invalid_argument("The value is not supported");
	}

	BufferHint TypeConverterGL::WritePixelHintToBufferHint(PixelBufferHint pixel_hint)
	{
		switch (pixel_hint)
		{
		case PixelBufferHint::Dynamic:
			return BufferHint::DynamicDraw;
		case PixelBufferHint::Static:
			return BufferHint::StaticDraw;
		case PixelBufferHint::Stream:
			return BufferHint::StreamDraw;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(EnableCap type)
	{
		switch (type)
		{
		case EnableCap::Blend:
			return GL_BLEND;
		/*
		case EnableCap::ClipDistance0:
			return GL_CLIP_DISTANCE0;
		case EnableCap::ClipDistance1:
			return GL_CLIP_DISTANCE1;
		case EnableCap::ClipDistance2:
			return GL_CLIP_DISTANCE2;
		case EnableCap::ClipDistance3:
			return GL_CLIP_DISTANCE3;
		case EnableCap::ClipDistance4:
			return GL_CLIP_DISTANCE4;
		case EnableCap::ClipDistance5:
			return GL_CLIP_DISTANCE5;
		case EnableCap::ColorLogicOp:
			return GL_COLOR_LOGIC_OP;
		*/
		case EnableCap::CullFace:
			return GL_CULL_FACE;
		/*
		case EnableCap::DepthClamp:
			return GL_DEPTH_CLAMP;
		*/
		case EnableCap::DepthTest:
			return GL_DEPTH_TEST;
		/*
		case EnableCap::Dither:
			return GL_ALPHA_TEST;
		case EnableCap::FramebufferSRGB:
			return GL_FRAMEBUFFER_SRGB;
		case EnableCap::LineSmooth:
			return GL_LINE_SMOOTH;
		case EnableCap::Multisample:
			return GL_MULTISAMPLE;
		*/
		case EnableCap::PolygonOffsetFill:
			return GL_POLYGON_OFFSET_FILL;
		/*
		case EnableCap::PolygonOffsetLine:
			return GL_POLYGON_OFFSET_LINE;
		case EnableCap::PolygonOffsetPoint:
			return GL_POLYGON_OFFSET_POINT;
		case EnableCap::PrimitiveRestart:
			return GL_PRIMITIVE_RESTART;
		*/
		case EnableCap::SampleAlphaToCoverage:
			return GL_SAMPLE_ALPHA_TO_COVERAGE;
		/*
		case EnableCap::SampleAlphaToOne:
			return GL_SAMPLE_ALPHA_TO_ONE;
		*/
		case EnableCap::SampleCoverage:
			return GL_SAMPLE_COVERAGE;
		case EnableCap::ScissorTest:
			return GL_SCISSOR_TEST;
		case EnableCap::StencilTest:
			return GL_STENCIL_TEST;
		/*
		case EnableCap::TextureCubeMapSeamless:
			return GL_TEXTURE_CUBE_MAP_SEAMLESS;
		case EnableCap::ProgramPointSize:
			return GL_PROGRAM_POINT_SIZE;
		*/
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::Lines:
			return GL_LINES;
		/*
		case PrimitiveType::LinesAdjacency:
			return GL_LINES_ADJACENCY;
		*/
		case PrimitiveType::LineStrip:
			return GL_LINE_STRIP;
		/*
		case PrimitiveType::LineStripAdjacency:
			return GL_LINE_STRIP_ADJACENCY;
		*/
		case PrimitiveType::LineLoop:
			return GL_LINE_LOOP;
		case PrimitiveType::Points:
			return GL_POINTS;
		/*
		case PrimitiveType::Quads:
			return GL_QUADS;
		case PrimitiveType::QuadStrip:
			return GL_QUAD_STRIP;
		*/
		case PrimitiveType::Triangles:
			return GL_TRIANGLES;
		/*
		case PrimitiveType::TrianglesAdjacency:
			return GL_TRIANGLES_ADJACENCY;
		*/
		case PrimitiveType::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		/*
		case PrimitiveType::TriangleStripAdjacency:
			return GL_TRIANGLE_STRIP_ADJACENCY;
		*/
		case PrimitiveType::TriangleFan:
			return GL_TRIANGLE_FAN;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(BlendEquation type)
	{
		switch (type)
		{
		case BlendEquation::Add:
			return GL_FUNC_ADD;
		case BlendEquation::ReverseSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;
		case BlendEquation::Subtract:
			return GL_FUNC_SUBTRACT;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(IndexDataType type)
	{
		switch (type)
		{
		case IndexDataType::UnsignedInteger:
			return GL_UNSIGNED_INT;
		case IndexDataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(DestinationBlendingFactor type)
	{
		switch (type)
		{
		case DestinationBlendingFactor::ConstantAlpha:
			return GL_CONSTANT_ALPHA;
		case DestinationBlendingFactor::ConstantColor:
			return GL_CONSTANT_COLOR;
		case DestinationBlendingFactor::DestinationAlpha:
			return GL_DST_ALPHA;
		case DestinationBlendingFactor::DestinationColor:
			return GL_DST_COLOR;
		case DestinationBlendingFactor::One:
			return GL_ONE;
		case DestinationBlendingFactor::OneMinusConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case DestinationBlendingFactor::OneMinusConstantColor:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case DestinationBlendingFactor::OneMinusDestinationAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case DestinationBlendingFactor::OneMinusDestinationColor:
			return GL_ONE_MINUS_DST_COLOR;
		case DestinationBlendingFactor::OneMinusSourceAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case DestinationBlendingFactor::SourceAlpha:
			return GL_SRC_ALPHA;
		case DestinationBlendingFactor::Zero:
			return GL_ZERO;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(SourceBlendingFactor type)
	{
		switch (type)
		{
		case SourceBlendingFactor::ConstantAlpha:
			return GL_CONSTANT_ALPHA;
		case SourceBlendingFactor::ConstantColor:
			return GL_CONSTANT_COLOR;
		case SourceBlendingFactor::DestinationAlpha:
			return GL_DST_ALPHA;
		case SourceBlendingFactor::DestinationColor:
			return GL_DST_COLOR;
		case SourceBlendingFactor::One:
			return GL_ONE;
		case SourceBlendingFactor::OneMinusConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case SourceBlendingFactor::OneMinusConstantColor:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case SourceBlendingFactor::OneMinusDestinationAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case SourceBlendingFactor::OneMinusDestinationColor:
			return GL_ONE_MINUS_DST_COLOR;
		case SourceBlendingFactor::OneMinusSourceAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case SourceBlendingFactor::SourceAlpha:
			return GL_SRC_ALPHA;
		case SourceBlendingFactor::SourceAlphaSaturate:
			return GL_SRC_ALPHA_SATURATE;
		case SourceBlendingFactor::Zero:
			return GL_ZERO;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(BufferAccess type)
	{
		unsigned int value = 0;

		if ((type & BufferAccess::ReadBit) != 0)
			value |= GL_MAP_READ_BIT;

		//if ((type & BufferAccess::CoherentBit) != 0)
		//	value |= GL_MAP_COHERENT_BIT;
		//
		//if ((type & BufferAccess::PersistentBit) != 0)
		//	value |= GL_MAP_PERSISTENT_BIT;

		if ((type & BufferAccess::WriteBit) != 0)
			value |= GL_MAP_WRITE_BIT;

		if ((type & BufferAccess::InvalidateBufferBit) != 0)
			value |= GL_MAP_INVALIDATE_BUFFER_BIT;

		if ((type & BufferAccess::UnsynchronizedBit) != 0)
			value |= GL_MAP_INVALIDATE_BUFFER_BIT;

		return value;
	}

	unsigned int TypeConverterGL::ToEnum(ClearBuffers type)
	{
		switch (type)
		{
		case ClearBuffers::All:
			return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		case ClearBuffers::ColorBuffer:
			return GL_COLOR_BUFFER_BIT;
		case ClearBuffers::ColorAndDepthBuffer:
			return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		case ClearBuffers::ColorAndStencilBuffer:
			return GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		case ClearBuffers::DepthAndStencilBuffer:
			return GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		case ClearBuffers::DepthBuffer:
			return GL_DEPTH_BUFFER_BIT;
		case ClearBuffers::StencilBuffer:
			return GL_STENCIL_BUFFER_BIT;
		}
	}

	unsigned int TypeConverterGL::ToEnum(StencilFace type)
	{
		switch (type)
		{
		case StencilFace::Back:
			return GL_BACK;
		case StencilFace::Front:
			return GL_FRONT;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(DepthTestFunction type)
	{
		switch (type)
		{
		case DepthTestFunction::Always:
			return GL_ALWAYS;
		case DepthTestFunction::Equal:
			return GL_EQUAL;
		case DepthTestFunction::Greater:
			return GL_GREATER;
		case DepthTestFunction::GreaterOrEqual:
			return GL_GEQUAL;
		case DepthTestFunction::Less:
			return GL_LESS;
		case DepthTestFunction::LessOrEqual:
			return GL_LEQUAL;
		case DepthTestFunction::Never:
			return GL_NEVER;
		case DepthTestFunction::NotEqual:
			return GL_NOTEQUAL;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(StencilOperation type)
	{
		switch (type)
		{
		case StencilOperation::Decrement:
			return GL_DECR;
		case StencilOperation::DecrementWrap:
			return GL_DECR_WRAP;
		case StencilOperation::Increment:
			return GL_INCR;
		case StencilOperation::IncrementWrap:
			return GL_INCR_WRAP;
		case StencilOperation::Invert:
			return GL_INVERT;
		case StencilOperation::Keep:
			return GL_KEEP;
		case StencilOperation::Replace:
			return GL_REPLACE;
		case StencilOperation::Zero:
			return GL_ZERO;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(StencilTestFunction type)
	{
		switch (type)
		{
		case StencilTestFunction::Always:
			return GL_ALWAYS;
		case StencilTestFunction::Equal:
			return GL_EQUAL;
		case StencilTestFunction::Greater:
			return GL_GREATER;
		case StencilTestFunction::GreaterOrEqual:
			return GL_GEQUAL;
		case StencilTestFunction::Less:
			return GL_LESS;
		case StencilTestFunction::LessOrEqual:
			return GL_LEQUAL;
		case StencilTestFunction::Never:
			return GL_NEVER;
		case StencilTestFunction::NotEqual:
			return GL_NOTEQUAL;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(CullFace type)
	{
		switch (type)
		{
		case CullFace::Back:
			return GL_BACK;
		case CullFace::Front:
			return GL_FRONT;
		case CullFace::FrontAndBack:
			return GL_FRONT_AND_BACK;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(RasterizationMode type)
	{
		switch (type)
		{
			/*
			case RasterizationMode::Fill:
				return GL_FILL;
			case RasterizationMode::Line:
				return GL_LINE;
			case RasterizationMode::Point:
				return GL_POINT;
			*/
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(WindingOrder type)
	{
		switch (type)
		{
		case WindingOrder::Clockwise:
			return GL_CW;
		case WindingOrder::CounterClockwise:
			return GL_CCW;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(UniformType type)
	{
		switch (type)
		{
		case UniformType::Bool:
			return GL_BOOL;
		case UniformType::Float:
			return GL_FLOAT;
		case UniformType::FloatVector2:
			return GL_FLOAT_VEC2;
		case UniformType::FloatVector3:
			return GL_FLOAT_VEC3;
		case UniformType::FloatVector4:
			return GL_FLOAT_VEC4;
		case UniformType::FloatMatrix22:
			return GL_FLOAT_MAT2;
		case UniformType::FloatMatrix23:
			return GL_FLOAT_MAT2x3;
		case UniformType::FloatMatrix24:
			return GL_FLOAT_MAT2x4;
		case UniformType::FloatMatrix33:
			return GL_FLOAT_MAT3;
		case UniformType::FloatMatrix32:
			return GL_FLOAT_MAT3x2;
		case UniformType::FloatMatrix34:
			return GL_FLOAT_MAT3x4;
		case UniformType::FloatMatrix44:
			return GL_FLOAT_MAT4;
		case UniformType::FloatMatrix42:
			return GL_FLOAT_MAT4x2;
		case UniformType::FloatMatrix43:
			return GL_FLOAT_MAT4x3;
		/*
		case UniformType::Sampler1D:
			return GL_SAMPLER_1D;
		case UniformType::Sampler1DArray:
			return GL_SAMPLER_1D_ARRAY;
		case UniformType::Sampler1DArrayShadow:
			return GL_SAMPLER_1D_ARRAY_SHADOW;
		case UniformType::Sampler1DShadow:
			return GL_SAMPLER_1D_SHADOW;
		*/
		case UniformType::Sampler2D:
			return GL_SAMPLER_2D;
		case UniformType::Sampler2DExternal:
			return GL_SAMPLER_EXTERNAL_OES;
		case UniformType::Sampler2DArray:
			return GL_SAMPLER_2D_ARRAY;
		case UniformType::Sampler2DArrayShadow:
			return GL_SAMPLER_2D_ARRAY_SHADOW;
		case UniformType::Sampler2DShadow:
			return GL_SAMPLER_2D_SHADOW;
		/*
		case UniformType::Sampler2DRect:
			return GL_SAMPLER_2D_RECT;
		case UniformType::Sampler2DRectShadow:
			return GL_SAMPLER_2D_RECT_SHADOW;
		*/
		case UniformType::Sampler3D:
			return GL_SAMPLER_3D;
		case UniformType::SamplerCube:
			return GL_SAMPLER_CUBE;
		case UniformType::SamplerCubeShadow:
			return GL_SAMPLER_CUBE_SHADOW;
		/*
		case UniformType::UnsignedIntSampler1D:
			return GL_UNSIGNED_INT_SAMPLER_1D;
		case UniformType::UnsignedIntSampler1DArray:
			return GL_UNSIGNED_INT_SAMPLER_1D_ARRAY;
		*/
		//case UniformType::IntSampler1D:
		//	return GL_INT_SAMPLER_1D;
		//case UniformType::IntSampler1DArray:
		//	return GL_INT_SAMPLER_1D_ARRAY;
		case UniformType::IntSampler2D:
			return GL_INT_SAMPLER_2D;
		case UniformType::IntSampler2DArray:
			return GL_INT_SAMPLER_2D_ARRAY;
		//case UniformType::IntSampler2DRect:
		//	return GL_INT_SAMPLER_2D_RECT;
		case UniformType::IntSampler3D:
			return GL_INT_SAMPLER_3D;
		case UniformType::IntSamplerCube:
			return GL_INT_SAMPLER_CUBE;
		case UniformType::UnsignedIntSampler2D:
			return GL_UNSIGNED_INT_SAMPLER_2D;
		case UniformType::UnsignedIntSampler2DArray:
			return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY;
		/*
		case UniformType::UnsignedIntSampler2DRect:
			return GL_UNSIGNED_INT_SAMPLER_2D_RECT;
		*/
		case UniformType::UnsignedIntSampler3D:
			return GL_UNSIGNED_INT_SAMPLER_3D;
		case UniformType::UnsignedIntSamplerCube:
			return GL_UNSIGNED_INT_SAMPLER_CUBE;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(BufferHint type)
	{
		switch (type)
		{
		case BufferHint::DynamicRead:
			return GL_DYNAMIC_READ;
		case BufferHint::DynamicDraw:
			return GL_DYNAMIC_DRAW;
		case BufferHint::DynamicCopy:
			return GL_DYNAMIC_COPY;
		case BufferHint::StaticDraw:
			return GL_STATIC_DRAW;
		case BufferHint::StaticCopy:
			return GL_STATIC_COPY;
		case BufferHint::StaticRead:
			return GL_STATIC_READ;
		case BufferHint::StreamDraw:
			return GL_STREAM_DRAW;
		case BufferHint::StreamCopy:
			return GL_STREAM_COPY;
		case BufferHint::StreamRead:
			return GL_STREAM_READ;
		}

		throw std::invalid_argument("The value is not supported");
	}


	unsigned int TypeConverterGL::ToEnum(ComponentDataType type)
	{
		switch (type)
		{
		case ComponentDataType::Byte:
			return GL_BYTE;
		case ComponentDataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		case ComponentDataType::Float:
			return GL_FLOAT;
		case ComponentDataType::HalfFloat:
			return GL_HALF_FLOAT;
		case ComponentDataType::Short:
			return GL_SHORT;
		case ComponentDataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		case ComponentDataType::Int:
			return GL_INT;
		case ComponentDataType::UnsignedInt:
			return GL_UNSIGNED_INT;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(TextureTarget type)
	{
		switch (type)
		{
		/*
		case TextureTarget::Texture1D:
			return GL_TEXTURE_1D;
		case TextureTarget::Texture1DArray:
			return GL_TEXTURE_1D_ARRAY;
		*/
		case TextureTarget::TextureExternal:
			return GL_TEXTURE_EXTERNAL_OES;
		case TextureTarget::Texture2D:
			return GL_TEXTURE_2D;
		case TextureTarget::Texture2DArray:
			return GL_TEXTURE_2D_ARRAY;
		case TextureTarget::Texture3D:
			return GL_TEXTURE_3D;
		case TextureTarget::TextureCubeMap:
			return GL_TEXTURE_CUBE_MAP;
		/*
		case TextureTarget::TextureRectangle:
			return GL_TEXTURE_RECTANGLE;
		*/
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(BufferTarget type)
	{
		switch (type)
		{
		case BufferTarget::ElementArray:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferTarget::VertexArray:
			return GL_ARRAY_BUFFER;
		case BufferTarget::Uniform:
			return GL_UNIFORM_BUFFER;
		/*
		case BufferTarget::AtomicCounter:
			return GL_ATOMIC_COUNTER_BUFFER;
		*/
		case BufferTarget::CopyRead:
			return GL_COPY_READ_BUFFER;
		case BufferTarget::CopyWrite:
			return GL_COPY_WRITE_BUFFER;
		/*
		case BufferTarget::DispatchIndirect:
			return GL_DISPATCH_INDIRECT_BUFFER;
		case BufferTarget::DrawIndirect:
			return GL_DRAW_INDIRECT_BUFFER;
		*/
		case BufferTarget::PixelPack:
			return GL_PIXEL_PACK_BUFFER;
		case BufferTarget::PixelUnPack:
			return GL_PIXEL_UNPACK_BUFFER;
		/*
		case BufferTarget::Query:
			return GL_QUERY_BUFFER;
		case BufferTarget::ShaderStorage:
			return GL_SHADER_STORAGE_BUFFER;
		case BufferTarget::Texture:
			return GL_TEXTURE_BUFFER;
		*/
		case BufferTarget::TransformFeedback:
			return GL_TRANSFORM_FEEDBACK_BUFFER;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(ShaderVertexAttributeType type)
	{
		switch (type)
		{
		case ShaderVertexAttributeType::Float:
			return GL_FLOAT;
		case ShaderVertexAttributeType::FloatVector2:
			return GL_FLOAT_VEC2;
		case ShaderVertexAttributeType::FloatVector3:
			return GL_FLOAT_VEC3;
		case ShaderVertexAttributeType::FloatVector4:
			return GL_FLOAT_VEC4;
		}

		throw std::invalid_argument("The value is not supported");
	}


	UniformType TypeConverterGL::ToUniformType(unsigned int type)
	{
		switch (type)
		{
		case GL_BOOL:
			return UniformType::Bool;
		case GL_FLOAT:
			return UniformType::Float;
		case GL_FLOAT_VEC2:
			return UniformType::FloatVector2;
		case GL_FLOAT_VEC3:
			return UniformType::FloatVector3;
		case GL_FLOAT_VEC4:
			return UniformType::FloatVector4;
		case GL_FLOAT_MAT2:
			return UniformType::FloatMatrix22;
		case GL_FLOAT_MAT2x3:
			return UniformType::FloatMatrix23;
		case GL_FLOAT_MAT2x4:
			return UniformType::FloatMatrix24;
		case GL_FLOAT_MAT3:
			return UniformType::FloatMatrix33;
		case GL_FLOAT_MAT3x2:
			return UniformType::FloatMatrix32;
		case GL_FLOAT_MAT3x4:
			return UniformType::FloatMatrix34;
		case GL_FLOAT_MAT4:
			return UniformType::FloatMatrix44;
		case GL_FLOAT_MAT4x2:
			return UniformType::FloatMatrix42;
		case GL_FLOAT_MAT4x3:
			return UniformType::FloatMatrix43;
		/*
		case GL_SAMPLER_1D:
			return UniformType::Sampler1D;
		case GL_SAMPLER_1D_ARRAY:
			return UniformType::Sampler1DArray;
		case GL_SAMPLER_1D_ARRAY_SHADOW:
			return UniformType::Sampler1DArrayShadow;
		case GL_SAMPLER_1D_SHADOW:
			return UniformType::Sampler1DShadow;
		*/
		case GL_SAMPLER_EXTERNAL_OES:
			return UniformType::Sampler2DExternal;
		case GL_SAMPLER_2D:
			return UniformType::Sampler2D;
		case GL_SAMPLER_2D_ARRAY:
			return UniformType::Sampler2DArray;
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			return UniformType::Sampler2DArrayShadow;
		case GL_SAMPLER_2D_SHADOW:
			return UniformType::Sampler2DShadow;
		/*
		case GL_SAMPLER_2D_RECT:
			return UniformType::Sampler2DRect;
		case GL_SAMPLER_2D_RECT_SHADOW:
			return UniformType::Sampler2DRectShadow;
		*/
		case GL_SAMPLER_3D:
			return UniformType::Sampler3D;
		case GL_SAMPLER_CUBE:
			return UniformType::SamplerCube;
		case GL_SAMPLER_CUBE_SHADOW:
			return UniformType::SamplerCubeShadow;
		//case GL_INT_SAMPLER_1D:
		//	return UniformType::IntSampler1D;
		//case GL_INT_SAMPLER_1D_ARRAY:
		//	return UniformType::IntSampler1DArray;
		case GL_INT_SAMPLER_2D:
			return UniformType::IntSampler2D;
		case GL_INT_SAMPLER_2D_ARRAY:
			return UniformType::IntSampler2DArray;
		//case GL_INT_SAMPLER_2D_RECT:
		//	return UniformType::IntSampler2DRect;
		case GL_INT_SAMPLER_3D:
			return UniformType::IntSampler3D;
		case GL_INT_SAMPLER_CUBE:
			return UniformType::IntSamplerCube;
		/*
		case GL_UNSIGNED_INT_SAMPLER_1D:
			return UniformType::UnsignedIntSampler1D;
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
			return UniformType::UnsignedIntSampler1DArray;
		*/
		case GL_UNSIGNED_INT_SAMPLER_2D:
			return UniformType::UnsignedIntSampler2D;
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
			return UniformType::UnsignedIntSampler2DArray;
		/*
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
			return UniformType::UnsignedIntSampler2DRect;
		*/
		case GL_UNSIGNED_INT_SAMPLER_3D:
			return UniformType::UnsignedIntSampler3D;
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
			return UniformType::UnsignedIntSamplerCube;
		}

		throw std::invalid_argument("The value is not supported");
	}

	ShaderVertexAttributeType TypeConverterGL::ToShaderVertexAttributeType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return ShaderVertexAttributeType::Float;
		case GL_FLOAT_VEC2:
			return ShaderVertexAttributeType::FloatVector2;
		case GL_FLOAT_VEC3:
			return ShaderVertexAttributeType::FloatVector3;
		case GL_FLOAT_VEC4:
			return ShaderVertexAttributeType::FloatVector4;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(TextureMinificationFilter type)
	{
		switch (type)
		{
		case TextureMinificationFilter::Linear:
			return GL_LINEAR;
		case TextureMinificationFilter::LinearMipmapLinear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureMinificationFilter::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureMinificationFilter::Nearest:
			return GL_NEAREST;
		case TextureMinificationFilter::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		case TextureMinificationFilter::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(TextureWrap type)
	{
		switch (type)
		{
		case TextureWrap::Clamp:
			return GL_CLAMP_TO_EDGE;
		case TextureWrap::Repeat:
			return GL_REPEAT;
		case TextureWrap::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(TextureMagnificationFilter type)
	{
		switch (type)
		{
		case TextureMagnificationFilter::Linear:
			return GL_LINEAR;
		case TextureMagnificationFilter::Nearest:
			return GL_NEAREST;
		}

		throw std::invalid_argument("The value is not supported");
	}

	ComponentDataType TypeConverterGL::ToComponentDataType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return ComponentDataType::Float;
		case GL_HALF_FLOAT:
			return ComponentDataType::HalfFloat;
		case GL_SHORT:
			return ComponentDataType::Short;
		case GL_UNSIGNED_SHORT:
			return ComponentDataType::UnsignedShort;
		case GL_INT:
			return ComponentDataType::Int;
		case GL_UNSIGNED_INT:
			return ComponentDataType::UnsignedInt;
		case GL_BYTE:
			return ComponentDataType::Byte;
		case GL_UNSIGNED_BYTE:
			return ComponentDataType::UnsignedByte;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::TextureToPixelType(TextureFormat textureFormat)
	{
		// TODO:  Not tested exhaustively
		switch (textureFormat)
		{
		case TextureFormat::RedGreenBlue8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::RedGreenBlue16:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::RedGreenBlueAlpha8:
			return GL_UNSIGNED_BYTE;
		/*
		case TextureFormat::RedGreenBlue10A2:
			return GL_UNSIGNED_INT_10_10_10_2;
		*/
		case TextureFormat::RedGreenBlueAlpha16:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::Depth16:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::Depth24:
			return GL_UNSIGNED_INT;
		case TextureFormat::Red8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::Red16:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::RedGreen8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::RedGreen16:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::Red16f:
			return GL_HALF_FLOAT;
		case TextureFormat::Red32f:
			return GL_FLOAT;
		case TextureFormat::RedGreen16f:
			return GL_HALF_FLOAT;
		case TextureFormat::RedGreen32f:
			return GL_FLOAT;
		case TextureFormat::Red8i:
			return GL_BYTE;
		case TextureFormat::Red8ui:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::Red16i:
			return GL_SHORT;
		case TextureFormat::Red16ui:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::Red32i:
			return GL_INT;
		case TextureFormat::Red32ui:
			return GL_UNSIGNED_INT;
		case TextureFormat::RedGreen8i:
			return GL_BYTE;
		case TextureFormat::RedGreen8ui:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::RedGreen16i:
			return GL_SHORT;
		case TextureFormat::RedGreen16ui:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::RedGreen32i:
			return GL_INT;
		case TextureFormat::RedGreen32ui:
			return GL_UNSIGNED_INT;
		case TextureFormat::RedGreenBlueAlpha32f:
			return GL_FLOAT;
		case TextureFormat::RedGreenBlue32f:
			return GL_FLOAT;
		case TextureFormat::RedGreenBlueAlpha16f:
			return GL_HALF_FLOAT;
		case TextureFormat::RedGreenBlue16f:
			return GL_HALF_FLOAT;
		case TextureFormat::Depth24Stencil8:
			return GL_UNSIGNED_INT_24_8;
		case TextureFormat::Red11fGreen11fBlue10f:
			return GL_FLOAT;
		case TextureFormat::RedGreenBlue9E5:
			return GL_FLOAT;
		case TextureFormat::SRedGreenBlue8:
		case TextureFormat::SRedGreenBlue8Alpha8:
			return GL_BYTE;
		case TextureFormat::Depth32f:
		case TextureFormat::Depth32fStencil8:
			return GL_FLOAT;
		case TextureFormat::RedGreenBlueAlpha32ui:
		case TextureFormat::RedGreenBlue32ui:
			return GL_UNSIGNED_INT;
		case TextureFormat::RedGreenBlueAlpha16ui:
		case TextureFormat::RedGreenBlue16ui:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::RedGreenBlueAlpha8ui:
		case TextureFormat::RedGreenBlue8ui:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::RedGreenBlueAlpha32i:
		case TextureFormat::RedGreenBlue32i:
			return GL_UNSIGNED_INT;
		case TextureFormat::RedGreenBlueAlpha16i:
		case TextureFormat::RedGreenBlue16i:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::RedGreenBlueAlpha8i:
		case TextureFormat::RedGreenBlue8i:
			return GL_UNSIGNED_BYTE;
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::TextureToPixelFormat(TextureFormat textureFormat)
	{
		// TODO:  Not tested exhaustively
		switch (textureFormat)
		{
		case TextureFormat::RedGreenBlue8:
		case TextureFormat::RedGreenBlue16:
			return GL_RGB;
		case TextureFormat::RedGreenBlueAlpha8:
		case TextureFormat::RedGreenBlue10A2:
		case TextureFormat::RedGreenBlueAlpha16:
			return GL_RGBA;
		case TextureFormat::Depth16:
		case TextureFormat::Depth24:
			return GL_DEPTH_COMPONENT;
		case TextureFormat::Red8:
		case TextureFormat::Red16:
			return GL_RED;
		case TextureFormat::RedGreen8:
		case TextureFormat::RedGreen16:
			return GL_RG;
		case TextureFormat::Red16f:
		case TextureFormat::Red32f:
			return GL_RED;
		case TextureFormat::RedGreen16f:
		case TextureFormat::RedGreen32f:
			return GL_RG;
		case TextureFormat::Red8i:
		case TextureFormat::Red8ui:
		case TextureFormat::Red16i:
		case TextureFormat::Red16ui:
		case TextureFormat::Red32i:
		case TextureFormat::Red32ui:
			return GL_RED_INTEGER;
		case TextureFormat::RedGreen8i:
		case TextureFormat::RedGreen8ui:
		case TextureFormat::RedGreen16i:
		case TextureFormat::RedGreen16ui:
		case TextureFormat::RedGreen32i:
		case TextureFormat::RedGreen32ui:
			return GL_RG_INTEGER;
		case TextureFormat::RedGreenBlueAlpha32f:
			return GL_RGBA;
		case TextureFormat::RedGreenBlue32f:
			return GL_RGB;
		case TextureFormat::RedGreenBlueAlpha16f:
			return GL_RGBA;
		case TextureFormat::RedGreenBlue16f:
			return GL_RGB;
		case TextureFormat::Depth24Stencil8:
			return GL_DEPTH_STENCIL;
		case TextureFormat::Red11fGreen11fBlue10f:
		case TextureFormat::RedGreenBlue9E5:
			return GL_RGB;
		case TextureFormat::SRedGreenBlue8:
			return GL_RGB_INTEGER;
		case TextureFormat::SRedGreenBlue8Alpha8:
			return GL_RGBA_INTEGER;
		case TextureFormat::Depth32f:
			return GL_DEPTH_COMPONENT;
		case TextureFormat::Depth32fStencil8:
			return GL_DEPTH_STENCIL;
		case TextureFormat::RedGreenBlueAlpha32ui:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue32ui:
			return GL_RGB_INTEGER;
		case TextureFormat::RedGreenBlueAlpha16ui:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue16ui:
			return GL_RGB_INTEGER;
		case TextureFormat::RedGreenBlueAlpha8ui:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue8ui:
			return GL_RGB_INTEGER;
		case TextureFormat::RedGreenBlueAlpha32i:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue32i:
			return GL_RGB_INTEGER;
		case TextureFormat::RedGreenBlueAlpha16i:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue16i:
			return GL_RGB_INTEGER;
		case TextureFormat::RedGreenBlueAlpha8i:
			return GL_RGBA_INTEGER;
		case TextureFormat::RedGreenBlue8i:
			return GL_RGB_INTEGER;
		}

		throw std::invalid_argument("textureFormat");
	}

	unsigned int TypeConverterGL::ToEnum(TextureFormat type)
	{
		switch (type)
		{
		case TextureFormat::RedGreenBlue8:
			return GL_RGB8;
		case TextureFormat::RedGreenBlueAlpha8:
			return GL_RGBA8;
		case TextureFormat::RedGreenBlue10A2:
			return GL_RGB10_A2;
		case TextureFormat::Depth16:
		case TextureFormat::Depth24:
			return GL_DEPTH_COMPONENT;
		case TextureFormat::Red8:
			return GL_R8;
		case TextureFormat::RedGreen8:
			return GL_RG8;
		case TextureFormat::Red16f:
			return GL_R16F;
		case TextureFormat::Red32f:
			return GL_R32F;
		case TextureFormat::RedGreen16f:
			return GL_RG16F;
		case TextureFormat::RedGreen32f:
			return GL_RG32F;
		case TextureFormat::Red8i:
			return GL_R8I;
		case TextureFormat::Red8ui:
			return GL_R8UI;
		case TextureFormat::Red16i:
			return GL_R16I;
		case TextureFormat::Red16ui:
			return GL_R16UI;
		case TextureFormat::Red32i:
			return GL_R32I;
		case TextureFormat::Red32ui:
			return GL_R32UI;
		case TextureFormat::RedGreen8i:
			return GL_RG8I;
		case TextureFormat::RedGreen8ui:
			return GL_RG8UI;
		case TextureFormat::RedGreen16i:
			return GL_RG16I;
		case TextureFormat::RedGreen16ui:
			return GL_RG16UI;
		case TextureFormat::RedGreen32i:
			return GL_RG32I;
		case TextureFormat::RedGreen32ui:
			return GL_RG32UI;
		case TextureFormat::RedGreenBlueAlpha32f:
			return GL_RGBA32F;
		case TextureFormat::RedGreenBlue32f:
			return GL_RGB32F;
		case TextureFormat::RedGreenBlueAlpha16f:
			return GL_RGBA16F;
		case TextureFormat::RedGreenBlue16f:
			return GL_RGB16F;
		case TextureFormat::Depth24Stencil8:
			return GL_DEPTH24_STENCIL8;
		case TextureFormat::Red11fGreen11fBlue10f:
			return GL_R11F_G11F_B10F;
		case TextureFormat::RedGreenBlue9E5:
			return GL_RGB9_E5;
		case TextureFormat::SRedGreenBlue8:
			return GL_SRGB8;
		case TextureFormat::SRedGreenBlue8Alpha8:
			return GL_SRGB8_ALPHA8;
		case TextureFormat::Depth32f:
			return GL_DEPTH_COMPONENT32F;
		case TextureFormat::Depth32fStencil8:
			return GL_DEPTH32F_STENCIL8;
		case TextureFormat::RedGreenBlueAlpha32ui:
			return GL_RGBA32UI;
		case TextureFormat::RedGreenBlue32ui:
			return GL_RGB32UI;
		case TextureFormat::RedGreenBlueAlpha16ui:
			return GL_RGBA16UI;
		case TextureFormat::RedGreenBlue16ui:
			return GL_RGB16UI;
		case TextureFormat::RedGreenBlueAlpha8ui:
			return GL_RGBA8UI;
		case TextureFormat::RedGreenBlue8ui:
			return GL_RGB8UI;
		case TextureFormat::RedGreenBlueAlpha32i:
			return GL_RGBA32I;
		case TextureFormat::RedGreenBlue32i:
			return GL_RGB32I;
		case TextureFormat::RedGreenBlueAlpha16i:
			return GL_RGBA16I;
		case TextureFormat::RedGreenBlue16i:
			return GL_RGB16I;
		case TextureFormat::RedGreenBlueAlpha8i:
			return GL_RGBA8I;
		case TextureFormat::RedGreenBlue8i:
			return GL_RGB8I;
		}

		throw std::invalid_argument("format");
	}
	
	unsigned int TypeConverterGL::ToEnum(ImageDataType type)
	{
		switch (type)
		{
		case ImageDataType::Byte:
			return GL_BYTE;
		case ImageDataType::Float:
			return GL_FLOAT;
		case ImageDataType::Float32UnsignedInt248Reversed:
			return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		case ImageDataType::HalfFloat:
			return GL_HALF_FLOAT;
		case ImageDataType::Int:
			return GL_INT;
		case ImageDataType::Short:
			return GL_SHORT;
		case ImageDataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
			/*
			case ImageDataType::UnsignedByte233Reversed:
			return GL_UNSIGNED_BYTE_2_3_3_REV;
			case ImageDataType::UnsignedByte332:
			return GL_UNSIGNED_BYTE_3_3_2;
			*/
		case ImageDataType::UnsignedInt:
			return GL_UNSIGNED_INT;
			/*
			case ImageDataType::UnsignedInt1010102:
			return GL_UNSIGNED_INT_10_10_10_2;
			*/
		case ImageDataType::UnsignedInt10F11F11FReversed:
			return GL_UNSIGNED_INT_10F_11F_11F_REV;
		case ImageDataType::UnsignedInt2101010Reversed:
			return GL_UNSIGNED_INT_2_10_10_10_REV;
		case ImageDataType::UnsignedInt248:
			return GL_UNSIGNED_INT_24_8;
		case ImageDataType::UnsignedInt5999Reversed:
			return GL_UNSIGNED_INT_5_9_9_9_REV;
			/*
			case ImageDataType::UnsignedInt8888:
			return GL_UNSIGNED_INT_8_8_8_8;
			case ImageDataType::UnsignedInt8888Reversed:
			return GL_UNSIGNED_INT_8_8_8_8_REV;
			*/
		case ImageDataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
			/*
			case ImageDataType::UnsignedShort1555Reversed:
			return GL_UNSIGNED_SHORT_1_5_5_5_REV;
			*/
		case ImageDataType::UnsignedShort4444:
			return GL_UNSIGNED_SHORT_4_4_4_4;
			/*
			case ImageDataType::UnsignedShort4444Reversed:
			return GL_UNSIGNED_SHORT_4_4_4_4_REV;
			*/
		case ImageDataType::UnsignedShort5551:
			return GL_UNSIGNED_SHORT_5_5_5_1;
		case ImageDataType::UnsignedShort565:
			return GL_UNSIGNED_SHORT_5_6_5;
			/*
			case ImageDataType::UnsignedShort565Reversed:
			return GL_UNSIGNED_SHORT_5_6_5_REV;
			*/
		}

		throw std::invalid_argument("The value is not supported");
	}

	unsigned int TypeConverterGL::ToEnum(ImageFormat type)
	{
		switch (type)
		{
		case ImageFormat::Blue:
			return GL_BLUE;
			/*
			case ImageFormat::BlueGreenRed:
			return GL_BGR;
			case ImageFormat::BlueGreenRedAlpha:
			return GL_BGRA;
			case ImageFormat::BlueGreenRedAlphaInteger:
			return GL_BGRA_INTEGER;
			case ImageFormat::BlueGreenRedInteger:
			return GL_BGR_INTEGER;
			case ImageFormat::BlueInteger:
			return GL_BLUE_INTEGER;
			*/
		case ImageFormat::DepthComponent:
			return GL_DEPTH_COMPONENT;
		case ImageFormat::DepthStencil:
			return GL_DEPTH_STENCIL;
		case ImageFormat::Green:
			return GL_GREEN;
			/*
			case ImageFormat::GreenInteger:
			return GL_GREEN_INTEGER;
			*/
		case ImageFormat::Red:
			return GL_RED;
		case ImageFormat::RedGreen:
			return GL_RG;
		case ImageFormat::RedGreenBlue:
			return GL_RGB;
		case ImageFormat::RedGreenBlueAlpha:
			return GL_RGBA;
		case ImageFormat::RedGreenBlueAlphaInteger:
			return GL_RGBA_INTEGER;
		case ImageFormat::RedGreenBlueInteger:
			return GL_RGB_INTEGER;
		case ImageFormat::RedGreenInteger:
			return GL_RG_INTEGER;
		case ImageFormat::RedInteger:
			return GL_RED_INTEGER;
			/*
			case ImageFormat::StencilIndex:
			return GL_STENCIL_INDEX;
			*/
		}

		throw std::invalid_argument("The value is not supported");
	}
	
	}
