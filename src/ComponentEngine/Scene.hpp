
#pragma once

#include <list>
#include "AttachableComponent.hpp"
#include "GameObject.hpp"
namespace ComponentEngine
{
using Key = std::string;
//    template <class Key>
    class SceneManager;

//    template <class Key>
    class Scene
    {
    private:
        // std::list<GameObject*> objects;
        GameObject* masterObject;

    protected:
//        SceneManager<Key>* sceneManager;

    public:
        Scene()
        {
            masterObject = new GameObject();
        }

    public:
        virtual void Update() final;

        void AddObject(GameObject* obj)
        {
            //無効なポインタなら終了
            if (obj == nullptr)
            {
                return;
            }
            masterObject->AddChild(obj);
        }

        virtual ~Scene()
        {
            delete masterObject;
        }
    };

//    template <class Key>
    class SceneManager
    {
        using ScenePtr = std::shared_ptr<Scene>;

        ScenePtr currentScene, nextScene;

        std::unordered_map<Key, std::function<ScenePtr()>> sceneMaker;

    public:
        //シーンを登録します
        template <typename SceneName>
        void RegisterScene(const Key& key)
        {
#ifdef DEBUG
            if (sceneMaker.count(key) != 0)
            {
                std::cout << "登録シーンが重複しています" << std::endl;
            }
#endif
            sceneMaker[key] = [&]() { return std::make_shared<SceneName>(this); };
        }

        void ChangeScene(const Key& key)
        {
#ifdef DEBUG
            if (sceneMaker.count(key) == 0)
            {
                std::cout << "シーンが存在しません" << std::endl;
            }
#endif
            nextScene = sceneMaker[key]();
        }

        void UpdateCurrentScene()
        {
            //次のシーンが用意されていれば
            if (nextScene)
            {
                //差し替える
                currentScene = nextScene;
                nextScene.reset();
            }

            //シーンの更新
            currentScene->Update();
        }
    };
}  // namespace ComponentEngine
