#pragma once

namespace ComponentEngine
{
    class IComponent
    {
    public:
        virtual void Start() = 0;
        virtual void Update() = 0;
    };
}  // namespace ComponentEngine