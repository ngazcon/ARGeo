#pragma once
namespace argeo
{
	enum class UniformType
	{
		Float,
		Int,
		IntVector2,
		IntVector3,
		IntVector4,

		Bool,
		BoolVector2,
		BoolVector3,
		BoolVector4,

		FloatVector2,
		FloatVector3,
		FloatVector4,

		FloatMatrix22,
		FloatMatrix33,
		FloatMatrix44,
		FloatMatrix23,
		FloatMatrix24,
		FloatMatrix32,
		FloatMatrix34,
		FloatMatrix42,
		FloatMatrix43,

		Sampler1D,
		Sampler2D,
		Sampler2DRect,
		Sampler2DRectShadow,
		Sampler3D,
		SamplerCube,
		Sampler1DShadow,
		Sampler2DShadow,
		Sampler2DExternal,

		Sampler1DArray,
		Sampler2DArray,
		Sampler1DArrayShadow,
		Sampler2DArrayShadow,
		SamplerCubeShadow,

		IntSampler1D,
		IntSampler2D,
		IntSampler2DRect,
		IntSampler3D,
		IntSamplerCube,
		IntSampler1DArray,
		IntSampler2DArray,

		UnsignedIntSampler1D,
		UnsignedIntSampler2D,
		UnsignedIntSampler2DRect,
		UnsignedIntSampler3D,
		UnsignedIntSamplerCube,
		UnsignedIntSampler1DArray,
		UnsignedIntSampler2DArray,
	};
}