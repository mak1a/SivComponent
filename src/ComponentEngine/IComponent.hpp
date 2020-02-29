#pragma once

namespace ComponentEngine
{
    class IComponent
    {
    private:
        virtual void call_start() = 0;

    protected:
        virtual void Start() = 0;
        virtual void Update() = 0;

        friend class AttachableComponent;
        friend class GameObject;

    public:
        virtual ~IComponent() = default;
    };
}  // namespace ComponentEngine
