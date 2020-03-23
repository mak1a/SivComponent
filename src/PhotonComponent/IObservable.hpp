#pragma once

namespace ComponentEngine::Photon
{
    class AttachableComponentPhotonCallbacks;
    class IPhotonObservable
    {
    public:
        virtual void Subscribe(AttachableComponentPhotonCallbacks* component) = 0;
        virtual void Dispose(AttachableComponentPhotonCallbacks* component) = 0;
        virtual ~IPhotonObservable() = default;
    };
}  // namespace PhotonComponent
