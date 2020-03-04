#pragma once

namespace ComponentEngine
{
    class GameObject;
    class IComponent
    {
        GameObject* hoge;

    private:
        virtual void call_start() = 0;
        // virtual void SetGameObject(GameObject* object);

    protected:
        virtual void Start() = 0;
        virtual void Update() = 0;

        friend class AttachableComponent;
        friend class GameObject;

    public:
        virtual ~IComponent() = default;
    };
}  // namespace ComponentEngine

class Hoge
{
};