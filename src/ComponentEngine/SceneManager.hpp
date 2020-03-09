#pragma once

#include "IScene.hpp"

namespace ComponentEngine
{
    class SceneManager
    {
    private:
        using KeyType = IScene::KeyType;

        using ScenePtr = std::shared_ptr<IScene>;

        ScenePtr currentScene, nextScene;

        std::unordered_map<KeyType, std::function<ScenePtr()>> sceneMaker;

    public:
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
            SceneManager* _this = this;

            sceneMaker[key] = [&]() {
                ScenePtr scene = std::make_shared<SceneName>();
                scene->manager = _this;
                return scene;
            };

            if (currentScene == nullptr)
            {
                currentScene = sceneMaker[key]();
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
