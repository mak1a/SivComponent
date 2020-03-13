#pragma once

#include <memory>

namespace ComponentEngine
{
    class GameObject;
    class IComponent
    {
    protected:
        std::weak_ptr<GameObject> gameobject;

    protected:
        virtual void call_awake() = 0;
        // virtual void Awake() = 0;
        virtual void call_start() = 0;
        // virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void LateUpdate() = 0;
        virtual void Draw() const = 0;
        virtual void OnDestroy() = 0;

        // friend class AttachableComponent;
        friend class GameObject;

    public:
        // active flag
        virtual void SetActive(bool) = 0;
        virtual bool GetActive() const = 0;

    public:
        virtual ~IComponent() = default;
    };
}  // namespace ComponentEngine