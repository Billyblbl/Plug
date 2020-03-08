/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** Cache
*/

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>

namespace Plug {

    ///
    ///@brief Cache tempalte
    ///
    /// This object serves to keep references to already loaded instances of potentially shared elements.
    /// A cached entry is considered expired when all ElementPtr's have been cleared
    ///
    ///@tparam element type of element cached
    ///
    template<typename Element>
    class Cache {
        public:

            ///
            ///@brief Construct a new Cache
            ///
            ///
            Cache() = default;

            ///
            ///@brief Destroy the Cache
            ///
            ///
            ~Cache() = default;

            ///
            ///@brief Unique key type used to access a reference to an element
            ///
            ///
            using ElementKey = std::string;

            ///
            ///@brief Owning participant Pointer type to a loaded element
            ///
            ///
            using ElementPtr = std::shared_ptr<Element>;

            ///
            ///@brief element Entry referencing a previously loaded element
            ///
            ///
            using ElementEntry = std::weak_ptr<Element>;

            ///
            ///@brief Internal map type
            ///
            ///
            using ElementMap = std::unordered_map<std::string, ElementEntry>;

            ///
            ///@brief Pushes an element in the cache and associate it with uniqueKey
            ///
            ///Pushes an element in the cache and associate it with uniqueKey
            ///
            ///@param uniqueKey key to associate with the element
            ///@param element element to add to the cache
            ///
            ///@warning throws if uniqueKey is already associated with another element
            ///
            void            push(const ElementKey &uniqueKey, ElementPtr element)
            {
                if (!tryPush(uniqueKey, element))
                    throw std::runtime_error(std::string(__func__) + " : cannot push in place of existing element '" + uniqueKey + "'");
            }

            ///
            ///@brief Attempts to push an element in the cache and associate it with uniqueKey
            ///
            /// Pushes an element in the cache and associate it with uniqueKey if no element is already associated with uniqueKey, do nothing otherwise
            ///
            ///@param uniqueKey key to associate with the element
            ///@param element element to add to the cache
            ///@return true if the element was successfuly pushed
            ///@return false if an element is already associated with uniqueKey
            ///
            bool            tryPush(const ElementKey &uniqueKey, ElementPtr element)
            {
                return _elements.try_emplace(uniqueKey, element).second;
            }

            ///
            ///@brief Pushes an element in the cache and associate it with uniqueKey
            ///
            /// Pushes an element in the cache and associate it with uniqueKey.
            /// If an element is already associated with uniqueKey, it is removed from the cache and replaced with element
            ///
            ///@param uniqueKey key to associate with the element
            ///@param element element to add to the cache
            ///
            void            forcePush(const ElementKey &uniqueKey, ElementPtr element)
            {
                _elements[uniqueKey] = element;
            }

            ///
            ///@brief Removes an element from the cache
            ///
            /// Removes the element from the cache associated with uniqueKey
            ///
            ///@param uniqueKey Key associated with the element to remove
            ///
            ///@warning throws if uniqueKey is not contained in the cache
            ///@warning throws if the element associated with uniqueKey is not expired
            ///
            void            pop(const ElementKey &uniqueKey)
            {
                if (!tryPop(uniqueKey))
                    throw std::runtime_error(std::string(__func__) + " : cannot remove cache element '" + uniqueKey + "' because it is still referenced elsewhere");
            }

            ///
            ///@brief Attempts to removes an element from the cache
            ///
            /// Removes the element from the cache associated with uniqueKey if it is expired, do nothing otherwise
            ///
            ///@param uniqueKey Key associated with the element to remove
            ///@return true if the element was successfully removed
            ///@return false if the element was not expired
            ///
            ///@warning throws if uniqueKey is not contained in the cache
            ///
            bool            tryPop(const ElementKey &uniqueKey)
            {
                auto &element = _elements.at(uniqueKey);
                if (!element.expired())
                    return false;
                _elements.erase(uniqueKey);
                return true;
            }

            ///
            ///@brief Removes an element from the cache
            ///
            /// Removes the element from the cache associated with uniqueKey
            ///
            ///@param uniqueKey Key associated with the element to remove
            ///@note If the element associated with uniqueKey is still referenced elswhere, they will remain loaded, this function only removes the cache entry
            ///
            void            forcePop(const ElementKey &uniqueKey)
            {
                _elements.erase(uniqueKey);
            }

            ///
            ///@brief Gets an element in the cache
            ///
            /// Gets the element in the cache associated with uniqueKey
            ///
            ///@param uniqueKey Key associated with the element to get
            ///@return ElementPtr Desired element
            ///
            ///@warning throws if uniqueKey is not contained in the cache
            ///@warning throws if the element associated with uniqueKey is expired
            ///
            ElementPtr       get(const ElementKey &uniqueKey)
            {
                return _elements.at(uniqueKey);
            }

            ///
            ///@brief Gets an element in the cache
            ///
            /// Gets the element in the cache associated with uniqueKey.
            /// Readonly access
            ///
            ///@param uniqueKey Key associated with the element to get
            ///@return ElementPtr Desired element
            ///
            ///@warning throws if uniqueKey is not contained in the cache
            ///@warning throws if the element associated with uniqueKey is expired
            ///
            const ElementPtr get(const ElementKey &uniqueKey) const
            {
                return _elements.at(uniqueKey);
            }

            ///
            ///@brief Loads an element in the cache
            ///
            /// Loads from construction arguments
            ///
            ///@tparam ElementType Type of the elemnt in the cache. must be Element or derived from Element. Default to Element
            ///@tparam Args Construction argument types
            ///@param uniqueKey Key associated with the element to load
            ///@param args Construction arguments
            ///@return std::shared_ptr<ElementType> desired element
            ///
            template<typename ElementType = Element, typename... Args>
            std::shared_ptr<ElementType>    load(const ElementKey &uniqueKey, Args&&... args)
            {
                static_assert(std::is_base_of_v<Element, ElementType>, "ElementType must be Element or derived from Element");
                auto it = _elements.find(uniqueKey);
                if (it != _elements.end()) {
                    auto element = it.second.lock();
                    if (element)
                        return element;
                }
                auto element = std::make_shared<ElementType>(args);
                _elements[uniqueKey] = element;
                return element;
            }

            ///
            ///@brief Loads an element in the cache
            ///
            /// Loads from the key itself
            ///
            ///@tparam ElementType Type of the elemnt in the cache. must be Element or derived from Element. Default to Element
            ///@param uniqueKey Key associated with the element to load
            ///@return std::shared_ptr<ElementType> desired element
            ///
            template<typename ElementType = Element>
            std::shared_ptr<ElementType>    load(const ElementKey &uniqueKey)
            {
                static_assert(std::is_base_of_v<Element, ElementType>, "ElementType must be Element or derived from Element");
                auto it = _elements.find(uniqueKey);
                if (it != _elements.end()) {
                    auto element = it.second.lock();
                    if (element)
                        return element;
                }
                auto element = std::make_shared<ElementType>(uniqueKey);
                _elements[uniqueKey] = element;
                return element;
            }

            ///
            ///@brief Reloads the cache entry of an element
            ///
            /// Reloads the element associated with uniqueKey from construction arguments
            ///
            ///@tparam Element Element type to reload, default to Element
            ///@tparam Args Construction argument types
            ///@param uniqueKey Key associated with the element to reload
            ///@param args Construction arguments
            ///@return td::shared_ptr<ElementType> desired element
            ///
            template<typename ElementType = Element, typename... Args>
            std::shared_ptr<ElementType>    reload(const ElementKey &uniqueKey, Args&&... args)
            {
                forcePush(uniqueKey, std::make_shared<ElementType>(args));
                return _elements[uniqueKey];
            }

             ///
            ///@brief Reloads the cache entry of an element
            ///
            /// Reloads the element associated with uniqueKey from the key itself
            ///
            ///@tparam Element Element type to reload, default to Element
            ///@param uniqueKey Key associated with the element to reload
            ///@return  std::shared_ptr<ElementType> desired element
            ///
            template<typename ElementType = Element>
            std::shared_ptr<ElementType>    reload(const ElementKey &uniqueKey)
            {
                forcePush(uniqueKey, std::make_shared<ElementType>(uniqueKey));
                return _elements[uniqueKey];
            }

            ///
            ///@brief Removes all expired entries from the cache
            ///
            ///
            void            shrink()
            {
                std::vector<std::string>    expired;
                for (auto &[key, entry] : _elements) {
                    if (entry.expired())
                        expired.push_back(key);
                }
                for (auto &key : expired)
                    _elements.erase(key);
            }

            ///
            ///@brief Check if the cache contains a key
            ///
            ///@param uniqueKey key to find
            ///@return true if the key is present in the cache
            ///@return false if the key is absent from the cache
            ///
            bool            contains(const ElementKey &uniqueKey) const
            {
                return (_elements.find() != _elements.end());
            }

            ///
            ///@brief Range iterator
            ///
            ///
            decltype(auto)  begin()
            {
                return _elements.begin();
            }

            ///
            ///@brief Range iterator
            ///
            ///
            decltype(auto)  begin() const
            {
                return _elements.begin();
            }

            ///
            ///@brief Range iterator
            ///
            ///
            decltype(auto)  end()
            {
                return _elements.end();
            }

            ///
            ///@brief Range iterator
            ///
            ///
            decltype(auto)  end() const
            {
                return _elements.end();
            }

            ///
            ///@brief Indexing operator
            ///
            /// Access to an element in the cache.
            /// Read only access
            /// Effectively the same as a call to `cache.get(uniqueKey)`
            ///
            ///@param uniqueKey Key associated with the element to get
            ///@return const ElementPtr Desired element
            ///
            const ElementPtr    operator[](const ElementKey &uniqueKey) const
            {
                return get(uniqueKey);
            }

            ///
            ///@brief Indexing operator
            ///
            /// Access to an element in the cache, and load it using uniqueKey if it as not contained or if it is expired
            /// Read-Write access
            /// Effectively the same as a call to `cache.load(uniqueKey)`
            ///
            ///@param uniqueKey Key associated with the element to load
            ///@return ElementPtr Desired element
            ///
            ElementPtr          operator[](const ElementKey &uniqueKey)
            {
                return load(uniqueKey);
            }

        protected:
        private:

        ElementMap   _elements;

    };

}


#endif /* !CACHE_HPP_ */
