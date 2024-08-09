#include <MathExtension.h>


pd::Mat3 pd::matrixMultiply(const Mat3& left, const Mat3& right)
{
    Mat3 result;

    result.m0 = left.m0 * right.m0 + left.m3 * right.m1 + left.m6 * right.m2;
    result.m1 = left.m1 * right.m0 + left.m4 * right.m1 + left.m7 * right.m2;
    result.m2 = left.m2 * right.m0 + left.m5 * right.m1 + left.m8 * right.m2;

    result.m3 = left.m0 * right.m3 + left.m3 * right.m4 + left.m6 * right.m5;
    result.m4 = left.m1 * right.m3 + left.m4 * right.m4 + left.m7 * right.m5;
    result.m5 = left.m2 * right.m3 + left.m5 * right.m4 + left.m8 * right.m5;

    result.m6 = left.m0 * right.m6 + left.m3 * right.m7 + left.m6 * right.m8;
    result.m7 = left.m1 * right.m6 + left.m4 * right.m7 + left.m7 * right.m8;
    result.m8 = left.m2 * right.m6 + left.m5 * right.m7 + left.m8 * right.m8;

    return result;
}

pd::Mat3 pd::translation(const Vector2& translation)
{
    Mat3 result = { 1.0f, 0.0f, translation.x,
                    0.0f, 1.0f, translation.y,
                    0.0f, 0.0f, 1.0f };

    return result;
}

pd::Mat3 pd::rotation(f32 angle)
{
    f32 c = cos(angle);
    f32 s = sin(angle);

    pd::Mat3 result;

    result.m0 = c;
    result.m1 = s;
    result.m2 = 0.0f;

    result.m3 = -s;
    result.m4 = c;
    result.m5 = 0.0f;

    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 1.0f;

    return result;
}

pd::Mat3 pd::scaling(const Vector2& scaling)
{
    Mat3 result = { scaling.x, 0.0f, 0.0f,
                      0.0f, scaling.y, 0.0f,
                      0.0f, 0.0f, 1.0f };

    return result;
}

pd::Mat3 pd::inverse(const Mat3& mat)
{
    pd::Mat3 invMat{ 1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 1.0f };

    // Cache matrix values (speed optimization)
    f32 a00 = mat.m0, a01 = mat.m1, a02 = mat.m2;
    f32 a10 = mat.m3, a11 = mat.m4, a12 = mat.m5;
    f32 a20 = mat.m6, a21 = mat.m7, a22 = mat.m8;

    f32 det = a00 * (a11 * a22 - a12 * a21) -
              a01 * (a10 * a22 - a12 * a20) +
              a02 * (a10 * a21 - a11 * a20);

    if (det == 0.0f) {
        // Matrix is not invertible
        // For simplicity, we'll just return identity matrix.
        return invMat;
    }

    f32 invDet = 1.0f / det;

    invMat.m0 = (a11 * a22 - a12 * a21) * invDet;
    invMat.m1 = (a02 * a21 - a01 * a22) * invDet;
    invMat.m2 = (a01 * a12 - a02 * a11) * invDet;
    invMat.m3 = (a12 * a20 - a10 * a22) * invDet;
    invMat.m4 = (a00 * a22 - a02 * a20) * invDet;
    invMat.m5 = (a02 * a10 - a00 * a12) * invDet;
    invMat.m6 = (a10 * a21 - a11 * a20) * invDet;
    invMat.m7 = (a01 * a20 - a00 * a21) * invDet;
    invMat.m8 = (a00 * a11 - a01 * a10) * invDet;

    return invMat;
}

Vector3 pd::vectorTransform(Vector3 v, Mat3 mat)
{
    Vector3 result = { 0 };

    f32 x = v.x;
    f32 y = v.y;
    f32 z = v.z;

    result.x = mat.m0 * x + mat.m3 * y + mat.m6 * z;
    result.y = mat.m1 * x + mat.m4 * y + mat.m7 * z;
    result.z = mat.m2 * x + mat.m5 * y + mat.m8 * z;

    return result;
}

f32 pd::lerp(f32 a, f32 b, f32 t)
{
    return a + t * (b - a);
}

Vector2 pd::vector2Lerp(Vector2& a, Vector2& b, f32 t)
{
    return Vector2{
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
    };
}
