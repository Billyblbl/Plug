/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** ObjectModule
*/

#ifndef OBJECTMODULE_HPP_
#define OBJECTMODULE_HPP_

#include <memory>
#include <functional>
#include "DynLib.hpp"
#include "Modules.hpp"
#include "Raw.hpp"

namespace Plug {

    template<typename Interface>
    class ObjectModule {
        public:

            using Object = Interface;

            using ObjectCreator = std::function<Object*()>;
            using ObjectDestructor = std::function<void(Object *)>;

            using ObjectPtr = std::unique_obj<Object, ObjectDestructor>;
            using ModulePtr = typename decltype(Modules::Raw())::ElementPtr;

            ObjectModule() = default;

            ObjectModule(const std::string &path):
                _mod(Modules::Raw().load(path))
                _obj(
                    std::make_unique<Object>(
                        _mod.callSymbol<ObjectCreator>(Modules::CreateObject),
                        _mod.getFctSymbol<ObjectDestructor>(Modules::DestroyObject)
                    )
                )
            {}

            ~ObjectModule() = default;

    		///
    		///@brief object dereferencer operator
    		///
    		///
    		Object	&operator*()
    		{
    			return *_obj;
    		}

    		///
    		///@brief member dereferencer operator
    		///
    		///
    		Object	*operator->()
    		{
    			return _obj.get();
    		}

    		///
    		///@brief object dereferencer const operator
    		///
    		///
    		const Object	&operator*() const
    		{
    			return *_obj;
    		}

    		///
    		///@brief member dereferencer const operator
    		///
    		///
    		const Object	*operator->() const
    		{
    			return _obj.get();
    		}

            ///
            ///@brief Object converter operator
            ///
            ///
    		operator Object&()
    		{
    			return *_obj;
    		}

            ///
            ///@brief Object const converter operator
            ///
            ///
    		operator const Object&() const
    		{
    			return *_obj;
    		}

        protected:
        private:

            ModulePtr   _mod;
            ObjectPtr   _obj;
    };

    namespace Modules {

        template<typename Interface>
        Cache<ObjectModule<Interface>>  &Objects()
        {
            static Cache<ObjectModule<Interface>>   objs;
            return objs;
        }

    }

}

#endif /* !OBJECTMODULE_HPP_ */
