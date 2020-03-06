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

namespace Plug {

    template<typename Element>
    class Cache {
        public:
            Cache() = default;
            ~Cache() = default;

            using ElementKey = std::string;
            using ElementPtr = std::shared_ptr<Element>;
            using ElementEntry = std::weak_ptr<Element>;
            using ElementMap = std::unordered_map<std::string, ElementEntry>>;

            void            push(const ElementKey &uniqueKey, ElementPtr Element)
            {
                if (!tryPush(uniqueKey, Element))
                    throw std::runtime_error(std::string(__func__) + " : cannot push in place of existing element '" + uniqueKey + "'");
            }

            bool            tryPush(const ElementKey &uniqueKey, ElementPtr Element)
            {
                return _Elements.try_emplace(uniqueKey, Element).second;
            }

            void            forcePush(const ElementKey &uniqueKey, ElementPtr Element)
            {
                _Elements[uniqueKey] = Element;
            }

            void            pop(const ElementKey &uniqueKey)
            {
                if (!tryPop(uniqueKey))
                    throw std::runtime_error(std::string(__func__) + " : cannot remove cache element '" + uniqueKey + "' because it is still referenced elsewhere");
            }

            bool            tryPop(const ElementKey &uniqueKey)
            {
                auto &Element = _Elements.at(uniqueKey);
                if (!Element.expired())
                    return false;
                _Elements.erase(uniqueKey);
                return true;
            }

            void            forcePop(const ElementKey &uniqueKey)
            {
                _Elements.erase(uniqueKey);
            }

            ElementPtr       get(const ElementKey &uniqueKey)
            {
                return _Elements.at(uniqueKey);
            }

            const ElementPtr get(const ElementKey &uniqueKey) const
            {
                return _Elements.at(uniqueKey);
            }

            template<typename... Args>
            ElementPtr       load(const ElementKey &uniqueKey, Args&&... args)
            {
                auto it = _Elements.find(uniqueKey);
                if (it != _Elements.end()) {
                    auto Element = it.second.lock();
                    if (Element)
                        return Element;
                }
                auto Element = std::make_shared<Element>(args);
                _Elements[uniqueKey] = Element;
                return Element;
            }

            ElementPtr       load(const ElementKey &uniqueKey)
            {
                auto it = _Elements.find(uniqueKey);
                if (it != _Elements.end()) {
                    auto Element = it.second.lock();
                    if (Element)
                        return Element;
                }
                auto Element = std::make_shared<Element>(uniqueKey);
                _Elements[uniqueKey] = Element;
                return Element;
            }

            template<typename... Args>
            ElementPtr       reload(const ElementKey &uniqueKey, Args&&... args)
            {
                forcePush(uniqueKey, std::make_shared<Element>(args));
            }

            ElementPtr       reload(const ElementKey &uniqueKey)
            {
                forcePush(uniqueKey, std::make_shared<Element>(uniqueKey));
            }

            void            shrink()
            {
                std::vector<std::string>    expired;
                for (auto &[key, entry] : _Elements) {
                    if (entry.expired())
                        expired.push_back(key);
                }
                for (auto &key : expired)
                    _Elements.erase(key);
            }

            bool            contains(const ElementKey &uniqueKey) const
            {
                _Elements.find()
            }

            decltype(auto)  begin()
            {
                return _Elements.begin();
            }

            decltype(auto)  begin() const
            {
                return _Elements.begin();
            }

            decltype(auto)  end()
            {
                return _Elements.end();
            }

            decltype(auto)  end() const
            {
                return _Elements.end();
            }

            const ElementPtr    operator[](const ElementKey &uniqueKey) const
            {
                return get(uniqueKey);
            }

            ElementPtr          operator[](const ElementKey &uniqueKey)
            {
                return load(uniqueKey);
            }

        protected:
        private:

        ElementMap   _Elements;

    };

}


#endif /* !CACHE_HPP_ */
