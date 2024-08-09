#pragma once


#include <raylib.h>
#include <raymath.h>

#include <Types.h>


namespace pd
{
    // Matrix, 3x3 components, column major, OpenGL style, right-handed
    struct Mat3 {
        f32 m0, m3, m6;  // Matrix first row (3 components)
        f32 m1, m4, m7;  // Matrix second row (3 components)
        f32 m2, m5, m8;  // Matrix third row (3 components)
    };

    // Get two matrix multiplication
    // NOTE: When multiplying matrices... the order matters!
    Mat3 matrixMultiply(const Mat3& left, const Mat3& right);

    // Get translation matrix
    Mat3 translation(const Vector2& translation);

    // Create rotation matrix from axis and angle
    // NOTE: Angle should be provided in radians
    Mat3 rotation(f32 angle);

    // Get scaling matrix
    Mat3 scaling(const Vector2& scaling);

    Mat3 inverse(const Mat3& mat);

    // Transforms a Vector3 by a given 3x3 Matrix
    Vector3 vectorTransform(Vector3 v, Mat3 mat);

    f32 lerp(f32 a, f32 b, f32 t);
    Vector2 vector2Lerp(Vector2& a, Vector2& b, f32 t);
}
