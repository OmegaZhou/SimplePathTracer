#pragma once
#include "../ZMath/Zmath.h"
ZLib::Vec3f toWorld(const ZLib::Vec3f& a, const ZLib::Vec3f& N)
{
    ZLib::Vec3f B, C;
    if (std::fabs(N.x()) > std::fabs(N.y())) {
        float invLen = 1.0f / std::sqrt(N.x() * N.x() + N.z() * N.z());
        C = ZLib::Vec3f(N.z() * invLen, 0.0f, -N.x() * invLen);
    } else {
        float invLen = 1.0f / std::sqrt(N.y() * N.y() + N.z() * N.z());
        C = ZLib::Vec3f(0.0f, N.z() * invLen, -N.y() * invLen);
    }
    C.normalize();
    B = C.cross(N);
    B.normalize();
    return   B * a.x() + C * a.y() + N * a.z();
}