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
#else

	#include <memory>
	#include <dlfcn.h>
#endif


namespace Plug
{

	class DynLib {
		public:

			DynLib(const std::string &path);

		#if defined(_WIN32)
			~DynLib();
		#else
			~DynLib() = default;
		#endif

			template<typename Callable = long(), typename... Args>
			decltype(auto)	callSymbol(const std::string &key, Args&&... args)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
			#if defined(_WIN32)	//Windows
				auto	thing = GetProcAddress(_handle, key.c_str());
				if (thing == NULL)
					throw std::runtime_error(std::string(__func__) + " : " + (char *)GetLastError());
			#else
				auto	*prevErr = dlerror();
				if (prevErr != nullptr)
					std::cerr << "Warn: when loading call to \"" << symbol << "\", detected previous unhandled error: " << prevErr << '\n';
				auto	*thing = dlsym(_handle.get(), symbol.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);
			#endif
				Functional	callable(reinterpret_cast<FctPtr>(thing));
				return callable(std::forward<Args>(args)...);
			}


			template<typename T>
			decltype(auto)	getSymbol(const std::string &symbol)
			{
				using Ptr = T*;
			#if defined(_WIN32)
				auto	thing = GetProcAddress(_handle, key.c_str());
				if (thing == NULL)
					throw std::runtime_error(std::string(__func__) + " : " + (char *)GetLastError());
			#else
				auto	*prevErr = dlerror();
				if (prevErr != nullptr)
					std::cerr << "Warn: when loading pointer to \"" << symbol << "\", detected previous unhandled error: " << prevErr << '\n';
				auto	*thing = dlsym(_handle.get(), symbol.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);

			#endif
				return *(reinterpret_cast<Ptr>(thing));
			}

			template<typename Callable>
			decltype(auto)	getFctSymbol(const std::string &symbol)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
			#if defined(_WIN32)
				auto	thing = GetProcAddress(_handle, key.c_str());
				if (thing == NULL)
					throw std::runtime_error(std::string(__func__) + " : " + (char *)GetLastError());
			#else
				auto	*prevErr = dlerror();
				if (prevErr != nullptr)
					std::cerr << "Warn: when loading function pointer to \"" << symbol << "\", detected previous unhandled error: " << prevErr << '\n';
				auto	*thing = dlsym(_handle.get(), symbol.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);

			#endif

				return Functional(reinterpret_cast<FctPtr>(thing));
			}


		protected:
		private:

		#if defined(_WIN32)
			HINSTANCE	_handle;
		#else //UNIX
			std::unique_ptr<void, std::function<void(void *)>>	_handle;
		#endif /* !OS check */

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

	};


} // namespace Plug

#endif /* !DYNLIB_HPP_ */

