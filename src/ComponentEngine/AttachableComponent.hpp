
#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include "GameObject.hpp"
#include "IComponent.hpp"

namespace ComponentEngine
{
    class AttachableComponent : public IComponent
    {
    private:
        bool initialized = false;

    public:
        bool IsInitialized()
        {
            return initialized;
        }

        std::weak_ptr<GameObject> gameobject;

    protected:
        // virtual funcs
        virtual void Start() override {}
        virtual void Update() override {}

        //これ自体の生成を禁止するためprotected
        AttachableComponent(){};

    protected:
        void call_start() override
        {
            if (!initialized)
            {
                Start();
                initialized = true;
            }
        }

    public:
        virtual ~AttachableComponent() = default;
    };
}  // namespace ComponentEngine
