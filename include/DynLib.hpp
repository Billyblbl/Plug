/*
** EPITECH PROJECT, 2019
** rtype
** File description:
** DynLib
*/

#ifndef DYNLIB_HPP_
#define DYNLIB_HPP_

#include <functional>


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

	class DynLib {
		public:

			DynLib(const std::string &path);

			~DynLib() = default;

			template<typename Callable, typename... Args>
			auto	call(const std::string &key, Args&&... args)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
				dlerror();
				auto	*thing = dlsym(_handle.get(), key.c_str());
				auto	*err = dlerror();
				if (err != nullptr)
					throw std::runtime_error(std::string(__func__) + " : " + err);
				Functional	callable(reinterpret_cast<FctPtr>(thing));
				return callable(std::forward<Args>(args)...);
			}

			//UNIX only
			template<typename Callable>
			auto	get(const std::string &key)
			{
				using Functional = std::function<Callable>;
				using FctPtr = FunctionalTarget<Callable>;
				dlerror();
				auto	*thing = dlsym(_handle.get(), key.c_str());
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
