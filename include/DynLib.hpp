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

	///
	///@brief Dynamic library wrapper
	///
	/// Provides a common for .so and .dll model of dynamic library object with access to the library's symbols
	///
	class DynLib {
		public:

			///
			///@brief Construct a new Dyn Lib
			///
			///@param path path to the dynamic library file
			///
			DynLib(const std::string &path);


		#if defined(_WIN32)
			///
			///@brief Destroy the Dyn Lib
			///
			///
			~DynLib();

			static constexpr auto	FileExtension = "dll";
		#else
			///
			///@brief Destroy the Dyn Lib
			///
			///
			~DynLib() = default;

			static constexpr auto	FileExtension = "so";

		#endif

			///
			///@brief Calls a symbol as a funtion defined by Callable
			///
			///@tparam Callable function signature of the callable object associated with symbol
			///@tparam Args Types of the arguments to forward to the call
			///@param symbol Symbol string of the callable to call
			///@param args Arguments to forward to the call
			///@return decltype(auto) return of the called Callable
			///
			template<typename Callable = long(), typename... Args>
			decltype(auto)	callSymbol(const std::string &symbol, Args&&... args)
			{
				using Functional = FunctionalWrapper<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
			#if defined(_WIN32)	//Windows
				auto	thing = GetProcAddress(_handle, symbol.c_str());
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

			///
			///@brief Get a Symbol's object
			///
			/// Get a reference to the object associated with symbol
			///
			///@tparam T Type of the object
			///@param symbol Symbol associated with the desired object
			///@return decltype(auto) Reference to the desired object
			///
			template<typename T>
			decltype(auto)	getSymbol(const std::string &symbol)
			{
				using Ptr = T*;
			#if defined(_WIN32)
				auto	thing = GetProcAddress(_handle, symbol.c_str());
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

			///
			///@brief Get a function
			///
			/// Gets the function associated with symbol as defined by the signature Callable
			///
			///@tparam Callable Signature of the function
			///@param symbol Symbol associated with the desire function
			///@return decltype(auto) Callable wrapper of the function associated with symbol
			///
			template<typename Callable>
			decltype(auto)	getFctSymbol(const std::string &symbol)
			{
				using Functional = FunctionalWrapper<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
			#if defined(_WIN32)
				auto	thing = GetProcAddress(_handle, symbol.c_str());
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
				using pointer = R(*)(Args...);
				using resultType = R;
				struct argumentTypes {
					using asTuple = std::tuple<Args...>;

					template<size_t index>
					using arg = std::tuple_element_t<index, asTuple>;
				};
				using wrapper = std::function<R(Args...)>;
			};

			template<typename R, typename... Args>
			struct FunctionalTraits<R(Args...)> {
				using pointer = R(*)(Args...);
				using resultType = R;
				struct argumentTypes {
					using asTuple = std::tuple<Args...>;

					template<size_t index>
					using arg = std::tuple_element_t<index, asTuple>;
				};
				using wrapper = std::function<R(Args...)>;
			};

			template<typename Callable>
			using FunctionalTarget = typename FunctionalTraits<Callable>::pointer;

			template<typename Callable>
			using FunctionalWrapper = typename FunctionalTraits<Callable>::wrapper;

	};


} // namespace Plug

#endif /* !DYNLIB_HPP_ */

