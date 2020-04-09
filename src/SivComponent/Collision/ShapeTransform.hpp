#pragma once

#define NO_S3D_USING
#include <Siv3D.hpp>

namespace ComponentEngine::Collision
{
    s3d::Quad transformed(const s3d::RectF& rect, const s3d::Mat3x2 mat);

    s3d::Quad transformed(const s3d::Quad& quad, const s3d::Mat3x2 mat);

    s3d::Circle transformed(const s3d::Circle& circle, const s3d::Mat3x2 mat);

    s3d::Line transformed(const s3d::Line& line, const s3d::Mat3x2 mat);

    s3d::Triangle transformed(const s3d::Triangle& line, const s3d::Mat3x2 mat);
}  // namespace ComponentEngine::Collision
