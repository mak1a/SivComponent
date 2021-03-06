﻿#pragma once

#include "IScene.hpp"
#include "SceneCommon.hpp"
#include "Global/Global.hpp"

namespace ComponentEngine
{
    class SceneManager
    {
        static SceneManager* manager;

    public:
        static SceneManager& GetSceneManager()
        {
            return *manager;
        }

    private:
        using KeyType = IScene::KeyType;

        using ScenePtr = std::shared_ptr<IScene>;

        ScenePtr currentScene, nextScene;

        SceneCommon common;

        using FuncType = std::function<ScenePtr()>;

        std::unordered_map<KeyType, FuncType> sceneMaker;

    public:
        SceneManager()
        {
            assert(manager == nullptr);

            manager = this;
            // commonMap = std::make_shared<ObjectMap>();
        }

        IScene& GetCurrentScene()
        {
            return *currentScene;
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

            //シーンによるエンジンへのデータ登録
            //            nextScene
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
            nextScene->_set_manager(this);
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
                    currentScene.reset();
                }
                currentScene.swap(nextScene);
                currentScene->_set_manager(this);

                //共通オブジェクトの設定
                currentScene->GetMasterObject()->AddChild(common.commonParent);
            }

            // フレームカウントの更新
            Global::Update();

            //シーンの更新
            currentScene->Update();
        }

        SceneCommon& GetCommon()
        {
            return common;
        }

        ~SceneManager()
        {
            manager = nullptr;
        }

        void End()
        {
            currentScene->DestoryAllObjects();
            currentScene.reset();
        }
    };
}  // namespace ComponentEngine
