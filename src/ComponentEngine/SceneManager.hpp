#pragma once

#include "IScene.hpp"

namespace ComponentEngine
{
    class SceneManager
    {
    public:
        std::string CommonParentObjectName() const;

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

    private:
        using KeyType = IScene::KeyType;

        using ScenePtr = std::shared_ptr<IScene>;

        ScenePtr currentScene, nextScene;

        // SceneManager* manager, std::shared_ptr<GameObject>& parent

        using FuncType = std::function<ScenePtr()>;

        std::unordered_map<KeyType, FuncType> sceneMaker;

    public:
        SceneManager()
        {
            // commonMap = std::make_shared<ObjectMap>();
            commonParent = std::make_shared<GameObject>();
            commonParent->SetName(CommonParentObjectName());
        }

        //シーンを登録します
        template <typename SceneName>
        void RegisterScene(const KeyType& key)
        {
#ifdef DEBUG
            if (sceneMaker.count(key) != 0)
            {
                std::cout << "登録シーンが重複しています" << std::endl;
            }
#endif

            // auto _this = this;

            FuncType func = [&]() -> ScenePtr {
                ScenePtr scene = std::make_shared<SceneName>();
                //ポインタの注入
                // scene->manager = _this;
                //                scene->masterObject->children.push_back(commonParent);
                //                commonParent->parent = scene->masterObject;
                return scene;
            };

            sceneMaker[key] = func;

            if (currentScene == nullptr && nextScene == nullptr)
            {
                nextScene = sceneMaker[key]();
            }
        }

        void ChangeScene(const KeyType& key)
        {
#ifdef DEBUG
            if (sceneMaker.count(key) == 0)
            {
                std::cout << "シーンが存在しません" << std::endl;
                return;
            }
#endif
            nextScene = sceneMaker[key]();
            nextScene->manager = this;
        }

        void UpdateCurrentScene()
        {
            //次のシーンが用意されていれば
            if (nextScene)
            {
                //差し替える
                if (currentScene)
                {
                    currentScene->DestoryAllObjects();
                }
                currentScene.swap(nextScene);
                nextScene.reset();
                currentScene->manager = this;

                //共通オブジェクトの設定
                currentScene->masterObject->AddChild(commonParent);
            }

            //シーンの更新
            currentScene->Update();
        }

        ~SceneManager() {}

        void End()
        {
            currentScene->DestoryAllObjects();
            currentScene.reset();
        }
    };
}  // namespace ComponentEngine
