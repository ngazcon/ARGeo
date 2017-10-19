#pragma once

#include <future>

using namespace std;

namespace argeo
{
	template <typename T, typename Work>
	auto then(future<T> f, Work w) -> future<decltype(w(f.get()))>
	{
		return async([](future<T> f, Work w)
		{ return w(f.get()); }, move(f), move(w));
	}

	template <typename Work>
	auto then(future<void> f, Work w) -> future<decltype(w())>
	{
		return async([](future<void> f, Work w)
		{ f.wait(); return w(); }, move(f), move(w));
	}

	template <typename T, typename Work>
	auto then(shared_future<T> f, Work w) -> shared_future<decltype(w(f.get()))>
	{
		return async([](shared_future<T> f, Work w)
		{ return w(f.get()); }, f, w);
	}

	template <typename Work>
	auto then(shared_future<void> f, Work w) -> shared_future<decltype(w())>
	{
		return async([](shared_future<void> f, Work w)
		{ f.wait(); return w(); }, f, w);
	}
}

