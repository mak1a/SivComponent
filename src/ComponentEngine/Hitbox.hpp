#pragma once
#include <vector>

#define NO_S3D_USING
#include <Siv3D.hpp>

namespace ComponentEngine
{
    using LayerType = unsigned char;

    class Hitbox
    {
        const LayerType layer;

    public:
        Hitbox(LayerType _layer)
            : layer(_layer)
        {
            s3d::Rect rect;
        }

        [[nodiscard]] LayerType GetLayer()
        {
            return layer;
        }
    };

    class HitboxManager
    {
    public:
        using SettingArray = std::vector<std::pair<LayerType, LayerType>>;

    private:
    public:
        void DisposeObject() {}
    };
}  // namespace ComponentEngine
