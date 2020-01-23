/*
** EPITECH PROJECT, 2019
** rtype
** File description:
** DynLib
*/

#ifndef DYNLIB_HPP_
#define DYNLIB_HPP_

#include <functional>
#include <iostream>


	#if defined(_WIN32)	//Windows

		#include <windows.h>

namespace Plug
{

	class DynLib {
		public:

			DynLib(const std::string &path);

			template<typename Callable = long(), typename... Args>
			auto	call(const std::string &key, Args&&... args)
			{
				auto	thing = GetProcAddress(_handle, key);
				if (thing == NULL)
					throw std::runtime_error(std::string(__func__) + " : " + GetLastError());
				return std::function<Callable>(thing)(std::forward<Args>(args)...);
			}

			~DynLib();

		protected:
		private:


		HINSTANCE	_handle;

	};

	#else //UNIX

		#include <memory>
		#include <dlfcn.h>

namespace Plug
{

	///
	///@brief Dynamic library wrapper
	///
	///
	class DynLib {
		public:

			DynLib(const std::string &path);

			~DynLib() = default;

			///
			///@brief Calls a function in the library
			///
			/// Calls the function associated with symbol, forwarding it args
			/// using Callable as its signature
			///
			///@tparam Callable Function signature
			///@tparam Args Arguments types
			///@param symbol Symbol of the function
			///@param args Arduments forwarded to the function
			///@return auto Return of the called function
			///
			template<typename Callable, typename... Args>
			auto	call(const std::string &symbol, Args&&... args)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
				auto	*prevErr = dlerror();
				if (prevErr != nullptr)
					std::cerr << "Warn: when loading call to \"" << symbol << "\", detected previous unhandled error: " << prevErr << '\n';
				auto	*thing = dlsym(_handle.get(), symbol.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);
				Functional	callable(reinterpret_cast<FctPtr>(thing));
				return callable(std::forward<Args>(args)...);
			}

			//UNIX only
			template<typename Callable>
			auto	get(const std::string &symbol)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
				auto	*prevErr = dlerror();
				if (prevErr != nullptr)
					std::cerr << "Warn: when loading function pointer to \"" << symbol << "\", detected previous unhandled error: " << prevErr << '\n';
				auto	*thing = dlsym(_handle.get(), symbol.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);
				return Functional(reinterpret_cast<FctPtr>(thing));
			}

		protected:
		private:

			template<typename T>
			struct FunctionalTraits {};

			template<typename R, typename... Args>
			struct FunctionalTraits<std::function<R(Args...)>> {
				using resultType = R;
				using signature = R(*)(Args...);
				struct argumentTypes {
					using asTuple = std::tuple<Args...>;

					template<size_t index>
					using arg = std::tuple_element_t<index, asTuple>;
				};
			};

			template<typename Callable>
			using FunctionalTarget = typename FunctionalTraits<std::function<Callable>>::signature;

			std::unique_ptr<void, std::function<void(void *)>>	_handle;

	};



	#endif /* !OS check */

} // namespace Plug

#endif /* !DYNLIB_HPP_ */

