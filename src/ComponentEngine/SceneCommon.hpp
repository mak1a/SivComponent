#pragma once

#include <unordered_map>

#include "GameObject.hpp"

namespace ComponentEngine
{
    class SceneManager;

    class SceneCommon
    {
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

    public:
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
    };
}  // namespace ComponentEngine