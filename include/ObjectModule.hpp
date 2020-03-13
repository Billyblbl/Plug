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

///@file

namespace Plug {

    ///
    ///@brief Wrapper for module models in which the business code is an object created by the module library
    ///
    ///@tparam Interface Interface of the module model
    ///
    template<typename Interface>
    class ObjectModule {
        public:

            ///
            ///@brief Object type
            ///
            ///
            using Object = Interface;

            ///
            ///@brief Object creator function wrapper type
            ///
            ///
            using ObjectCreator = std::function<Object*()>;

            ///
            ///@brief Object destructor function wrapper type
            ///
            ///
            using ObjectDestructor = std::function<void(Object *)>;

            ///
            ///@brief Object internal owning pointer type
            ///
            ///
            using ObjectPtr = std::unique_obj<Object, ObjectDestructor>;

            ///
            ///@brief Object internal owning pointer type
            ///
            ///
            using ModulePtr = typename decltype(Modules::Raw())::ElementPtr;

            ///
            ///@brief Cache type for object module models
            ///
            ///
            using CacheType = Cache<ObjectModule>;

            ///
            ///@brief Construct a new Object Module
            ///
            ///
            ObjectModule() = default;

            ///
            ///@brief Construct a new Object Module
            ///
            /// Construction using default obejct creator/destructor pair
            ///
            ///@param path Path to the module's dynamic library file
            ///
            ObjectModule(const std::string &path):
                _mod(Modules::Raw().load(path))
                _obj(
                    std::make_unique<Object>(
                        _mod.callSymbol<ObjectCreator>(Modules::CreateObject),
                        _mod.getFctSymbol<ObjectDestructor>(Modules::DestroyObject)
                    )
                )
            {}

            ///
            ///@brief Construct a new Object Module object
            ///
            /// Construction using creator/destructor pair associated with identifier
            ///
            ///@param path Path to the module's dynamic library file
            ///@param identifier Identifier of the module object to load
            ///
            ObjectModule(const std::string &path, const std::string &identifier):
                _mod(Modules::Raw().load(path))
                _obj(
                    std::make_unique<Object>(
                        _mod.callSymbol<ObjectCreator>(Modules::CreateNamedObject(identifier)),
                        _mod.getFctSymbol<ObjectDestructor>(Modules::DestroyNamedObject(identifer))
                    )
                )
            {}

            ///
            ///@brief Destroy the Object Module
            ///
            ///
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

    ///
    ///@brief Cache type for object module models of object type Interface
    ///
    ///@tparam Interface interface of the object type managed by the cache
    ///
    template<typename Interface>
    using ObjectCache = typename ObjectModule<Interface>::CacheType;

    namespace Modules {

        ///
        ///@brief Global cache access template for Object module models
        ///
        ///@tparam Interface Interface of the module model
        ///
        template<typename Interface>
        ObjectCache<Interface>  &Objects()
        {
            static Cache<ObjectModule<Interface>>   objs;
            return objs;
        }

    }

}

#endif /* !OBJECTMODULE_HPP_ */
