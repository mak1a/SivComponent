
#pragma once

#include <boost/noncopyable.hpp>
namespace ComponentEngine
{
    class AttachableComponent : private boost::noncopyable
    {
        bool initialized = false;
        
    public:
    protected:
        // virtual funcs
        virtual void Start() {}
        virtual void Update() {}
        
        //これ自体の生成を禁止するためprotected
        AttachableComponent(){};

    public:
        virtual ~AttachableComponent() = default;
        
        void update_component()
        {
            if(!initialized)
            {
                Start();
                initialized = true;
            }
            Update();
        }
    };
}  // namespace ComponentEngine
