#pragma once

#include <unordered_map>

#include "GameObject.hpp"

namespace ComponentEngine
{
    class SceneManager;

    class SceneCommon
    {
    public:  //共通オブジェクト機構
        std::string CommonParentObjectName() const
        {
            return std::string("CommonParent");
        }

    public:
        using MapKey = std::string;
        using MapValue = std::shared_ptr<GameObject>;
        using ObjectMap = std::unordered_map<MapKey, MapValue>;
        //シーン間共通オブジェクト

    private:
        //
        ObjectMap commonMap;
        //共通オブジェクトはここにくっつけていく
        std::shared_ptr<GameObject> commonParent;
        friend SceneManager;

    public:
        SceneCommon()
        {
            commonParent = std::make_shared<GameObject>();
            commonParent->SetName(CommonParentObjectName());
        }

        std::shared_ptr<GameObject> GetCommonObject(const MapKey& name)
        {
            return commonMap.at(name);
        }

        std::shared_ptr<GameObject> CreateAndGetCommonObject(const MapKey& keyname)
        {
            MapValue object = std::make_shared<GameObject>();
            commonParent->AddChild(object);
            commonMap[keyname] = object;
            return object;
        }

    private:  // Instantiate機構
        using instantiate_key = std::string;
        using Functype = std::function<std::shared_ptr<GameObject>()>;
        std::unordered_map<instantiate_key, Functype> prefabs;

    public:
        bool AddObjectCreator(const instantiate_key& str, const Functype& func)
        {
            if (prefabs.count(str) > 0)
            {
                return false;
            }

            prefabs.emplace(str, func);
            return true;
        }

        ///オブジェクトを生成する関数を取得
        [[nodiscard]] Functype& GetInstantiate(const instantiate_key& key)
        {
            return prefabs.at(key);
        }

        ///オブジェクトを生成
        [[nodiscard]] std::shared_ptr<GameObject> Instantiate(const instantiate_key& key) const
        {
            return prefabs.at(key)();
        }

        ///親を指定してオブジェクトを生成
        std::shared_ptr<GameObject> Instantiate(const instantiate_key& key, std::shared_ptr<GameObject>& parent)
        {
            auto obj = prefabs.at(key)();
            obj->SetParent(parent);
            return obj;
        }
    };
}  // namespace ComponentEngine