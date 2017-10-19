#pragma once

namespace argeo
{
	///
	/// Buffer access modes for buffer map method.
	///
	enum BufferAccess : unsigned int
	{
		ReadBit				= 1 << 0,
		WriteBit			= 1 << 1,
		PersistentBit		= 1 << 2,
		CoherentBit			= 1 << 3,
		InvalidateBufferBit = 1 << 4,
		UnsynchronizedBit   = 1 << 5,
	};

	inline BufferAccess operator&(
		const BufferAccess& lhs, 
		const BufferAccess& rhs)
	{
		return (BufferAccess)(static_cast<int>(lhs)& static_cast<int>(rhs));
	}

	inline BufferAccess operator|(
		const BufferAccess& lhs, 
		const BufferAccess& rhs)
	{
		return (BufferAccess)(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	inline BufferAccess& operator|=(
		BufferAccess& lhs, 
		const BufferAccess& rhs)
	{
		lhs = (BufferAccess)(static_cast<int>(lhs) | static_cast<int>(rhs));
		return lhs;
	}
}
