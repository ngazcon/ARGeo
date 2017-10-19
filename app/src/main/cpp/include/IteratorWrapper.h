/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#pragma once


namespace argeo {

	template <typename T, typename IteratorType, typename ValType>
	class IteratorWrapper
	{

	private:
		IteratorWrapper();

	protected:
		IteratorType mBegin;
		IteratorType mCurrent;
		IteratorType mEnd;


	public:

		typedef ValType ValueType;
		typedef ValType* PointerType;

		typedef IteratorType iterator;

		typedef IteratorType const_iterator;


		IteratorWrapper(IteratorType start, IteratorType last)
			: mBegin(start), mCurrent(start), mEnd(last)
		{
		}


		bool hasMoreElements() const
		{
			return mCurrent != mEnd;
		}


		void moveNext()
		{
			++mCurrent;
		}

		const IteratorType& begin() { return mBegin; }


		IteratorType& current() { return mCurrent; }

		const IteratorType& end() { return mEnd; }


	};



	template <typename T, typename IteratorType>
	class VectorIteratorWrapper : public IteratorWrapper<T, IteratorType, typename  T::value_type>
	{

	public:
		typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::ValueType ValueType;
		typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::PointerType PointerType;


		VectorIteratorWrapper(IteratorType start, IteratorType last)
			: IteratorWrapper<T, IteratorType, typename T::value_type>(start, last)
		{
		}


		ValueType peekNext() const
		{
			return *(this->mCurrent);
		}

		PointerType peekNextPtr()  const
		{
			return &(*(this->mCurrent));
		}

		ValueType getNext()
		{
			return *(this->mCurrent++);
		}

	};


	template <typename T>
	class VectorIterator : public VectorIteratorWrapper<T, typename T::iterator> {
	public:
		VectorIterator(typename T::iterator start, typename T::iterator last)
			: VectorIteratorWrapper<T, typename T::iterator>(start, last)
		{ }

		explicit VectorIterator(T& c)
			: VectorIteratorWrapper<T, typename T::iterator>(c.begin(), c.end())
		{ }
	};

	template <typename T>
	class ConstVectorIterator : public VectorIteratorWrapper<T, typename T::const_iterator> {
	public:
		ConstVectorIterator(typename T::const_iterator start, typename T::const_iterator last)
			: VectorIteratorWrapper<T, typename T::const_iterator>(start, last)
		{ }

		explicit ConstVectorIterator(const T& c)
			: VectorIteratorWrapper<T, typename T::const_iterator>(c.begin(), c.end())
		{ }
	};

	template <typename T, typename IteratorType>
	class MapIteratorWrapper : public IteratorWrapper<T, IteratorType, typename T::mapped_type>
	{

	public:
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::ValueType ValueType;
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::PointerType PointerType;

		typedef typename T::value_type PairType;
		typedef typename T::key_type KeyType;

		MapIteratorWrapper(IteratorType start, IteratorType last)
			: IteratorWrapper<T, IteratorType, typename T::mapped_type>(start, last)
		{
		}

		KeyType peekNextKey(void) const
		{
			return this->mCurrent->first;
		}


		ValueType peekNextValue() const
		{
			return this->mCurrent->second;
		}


		const PointerType peekNextValuePtr()  const
		{
			return &(this->mCurrent->second);
		}


		ValueType getNext()
		{
			return ((this->mCurrent++)->second);
		}


	};




	template <typename T>
	class MapIterator : public MapIteratorWrapper<T, typename T::iterator> {
	public:

		MapIterator(typename T::iterator start, typename T::iterator last)
			: MapIteratorWrapper<T, typename T::iterator>(start, last)
		{
		}

		explicit MapIterator(T& c)
			: MapIteratorWrapper<T, typename T::iterator>(c.begin(), c.end())
		{
		}

	};


	template <typename T>
	class ConstMapIterator : public MapIteratorWrapper<T, typename T::const_iterator> {
	public:

		ConstMapIterator(typename T::const_iterator start, typename T::const_iterator last)
			: MapIteratorWrapper<T, typename T::const_iterator>(start, last)
		{
		}

		explicit ConstMapIterator(const T& c)
			: MapIteratorWrapper<T, typename T::const_iterator>(c.begin(), c.end())
		{
		}
	};
}
