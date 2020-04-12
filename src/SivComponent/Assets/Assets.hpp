#define NO_S3D_USING
#include <Siv3D.hpp>

#include "../../ComponentEngine/AttachableComponent.hpp"

namespace ComponentEngine::Siv
{
    class BGM : public ComponentEngine::AttachableComponent
    {
        s3d::String name;

        void Awake() override;
        void OnDestroy() override;

    public:
        BGM(const s3d::String& BGM);

        s3d::AudioAsset GetAsset() const
        {
            return s3d::AudioAsset(name);
        }
    };
}  // namespace ComponentEngine::Siv