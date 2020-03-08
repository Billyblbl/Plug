/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** static_string
*/

#ifndef COMPILETIMESTR_HPP_
#define COMPILETIMESTR_HPP_

#include <string>
#include <string_view>

//local toolbox
//integer sequence stuff

template<long unsigned... Idxs>
struct Sequence {};

//make sequence of [N+1] from sequence of [N]
template<typename T>
struct Append;

template<long unsigned... Idxs>
struct Append<Sequence<Idxs...>> {
	using type = Sequence<Idxs..., sizeof...(Idxs)>;
};

//make sequence from size & implementation helper template
template<long unsigned size>
struct MakeSequenceImpl;

template<>
struct MakeSequenceImpl<0> {
	using type = Sequence<>;
};

template<long unsigned size>
using MakeSequence = typename MakeSequenceImpl<size>::type;

template<long unsigned size>
struct MakeSequenceImpl {
	using type = typename Append<MakeSequence<size - 1>>::type;
};
//end toolbox

//forward declarations

template<long unsigned sizeStr = 1>
class static_string;

template<long unsigned sizeStr>
constexpr static_string<sizeStr>	MakeStaticStr(const char (&str)[sizeStr]);


//mainly here to have compile time string concatenation for the grammar-like stuff
template<long unsigned sizeStr>
class static_string {
	public:
		constexpr static_string(const char (&str)[sizeStr]):
			static_string(str, MakeSequence<sizeStr>{})
		{}

		template<long unsigned size1, long unsigned size2>
		constexpr static_string(const static_string<size1> &t1, const static_string<size2> &t2):
			static_string(t1, t2, MakeSequence<size1 - 1>{}, MakeSequence<size2 - 1>{})
		{
			static_assert(size1 + size2 - 1 == sizeStr, "static_string sizeStr discrepancy, sizeStr != size1 + size2");
		}

		~static_string() = default;

		template<long unsigned other_size>
		constexpr decltype(auto)	operator+(const static_string<other_size> &rhs) const
		{
			return static_string<sizeStr + other_size - 1>(*this, rhs);
		}

		template<long unsigned other_size>
		constexpr decltype(auto)	operator+(const char (&str)[other_size]) const
		{
			return static_string<sizeStr + other_size - 1>(*this, MakeStaticStr(str));
		}

		constexpr operator std::string() const
		{
			return _str;
		}

		constexpr operator std::string_view() const
		{
			return _str;
		}

		constexpr std::string_view	getView() const
		{
			return _str;
		}

		constexpr std::string			getStr() const
		{
			return _str;
		}

		constexpr const char	*c_str() const
		{
			return _str;
		}

		constexpr std::size_t	size() const
		{
			return sizeStr;
		}

		constexpr char	operator[](long unsigned idx) const
		{
			return _str[idx];
		}

	protected:
	private:

		template<std::size_t... idxs>
		constexpr static_string(const char (&str)[sizeStr], Sequence<idxs...>):
			_str{str[idxs]...}
		{}

		template<long unsigned size1, long unsigned size2, std::size_t... idx1s, std::size_t... idx2s>
		constexpr static_string(const static_string<size1> &t1, const static_string<size2> &t2, Sequence<idx1s...>, Sequence<idx2s...>):
			_str{t1[idx1s]..., t2[idx2s]..., '\0'}
		{
			static_assert(size1 + size2 - 1 == sizeStr, "static_string sizeStr discrepancy, sizeStr != size1 + size2");
		}

		char	_str[sizeStr];
};

//make a static_string of the right sizeStr from a literal string
template<long unsigned sizeStr>
constexpr static_string<sizeStr>		MakeStaticStr(const char (&str)[sizeStr])
{
	return static_string<sizeStr>(str);
}

#endif /* !COMPILETIMESTR_HPP_ */
