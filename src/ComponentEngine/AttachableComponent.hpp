
#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include "IComponent.hpp"
#include "GameObject.hpp"

namespace ComponentEngine
{
    class AttachableComponent : public IComponent
    {
    private:
        bool initialized = false;

        std::weak_ptr<GameObject> gameobject;

            public : protected :
            // virtual funcs
            virtual void
            Start() override
        {
        }
        virtual void Update() override {}

        //これ自体の生成を禁止するためprotected
        AttachableComponent(){};

    public:
        virtual ~AttachableComponent() = default;

        void start_component()
        {
            if (!initialized)
            {
                Start();
                initialized = true;
            }
        }

        void update_component()
        {
            Update();
        }
    };
}  // namespace ComponentEngine
