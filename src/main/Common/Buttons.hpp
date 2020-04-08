
#pragma once

#include "../../ComponentEngine/ComponentEngine.hpp"
// #include "../../PhotonComponent/NetworkSystem.hpp"

class ChangeSceneButton : public ComponentEngine::AttachableComponent
{
    // std::shared_ptr<NameGenerator> generator;

    const std::string sceneName;

public:
    ChangeSceneButton(const std::string& scenename)
        : sceneName(scenename){};

    void OnClick()
    {
        auto manager = GetGameObject().lock()->GetScene().lock()->GetSceneManager();

        manager->ChangeScene(sceneName);
    }
};