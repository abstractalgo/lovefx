#pragma once

#include <cmath>

#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

// =============================================================================
// === vec2
// =============================================================================
namespace aamath
{
    template<typename T>
    struct vec2
    {
        float x, y;
        // constructors
        vec2(T _x = 0, T _y = 0) : x(_x), y(_y) {}
        vec2(const vec2<T> &v) : x(v.x), y(v.y) {}
        vec2<T>& operator=(const vec2<T>& v)
        {
            x = v.x;
            y = v.y;
            return *this;
        }
        // add
        vec2<T>& operator+(const vec2<T>& v) const
        {
            return vec2(x + v.x, y + v.y);
        }
        vec2<T>& operator+=(const vec2<T>& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }
        // sub
        vec2<T>& operator-(const vec2<T>& v) const
        {
            return vec2(x - v.x, y - v.y, z - v.z);
        }
        vec2<T>& operator-=(const vec2<T>& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        // mul
        template <typename U> vec2<T> operator*(U s) const
        {
            return vec2<T>(s * x, s * y);
        }
        template <typename U> vec2<T>& operator*=(U s)
        {
            x *= s;
            y *= s;
            return *this;
        }
        // div
        template <typename U> vec2<T> operator/(U f) const
        {
            float inv = (float)1 / f;
            return vec2<T>(x * inv, y * inv);
        }

        template <typename U> vec2<T>& operator/=(U f)
        {
            float inv = (float)1 / f;
            x *= inv;
            y *= inv;
            return *this;
        }
        // other
        vec2<T> operator-() const { return vec2<T>(-x, -y); }
        bool operator==(const vec2<T>& v) const { return x == v.x && y == v.y; }
        bool operator!=(const vec2<T>& v) const { return x != v.x || y != v.y; }
        // methods
        float len() const
        {
            return ::sqrt(x*x + y*y);
        }
        vec2& normalize()
        {
            float l = len();
            return ((*this) / l);
        }
        vec2& abs()
        {
            x = abs(x);
            y = abs(y);
            return *this;
        }
    };

    typedef vec2<float> vec2f;
    typedef vec2<int> vec2i;

    template<typename T>
    float dot(const vec2<T>& a, const vec2<T>& b)
    {
        return (a.x*b.x + a.y*b.y);
    }
}

// =============================================================================
// === vec3
// =============================================================================
namespace aamath
{
    template<typename T>
    struct vec3
    {
        float x, y, z;
        // constructors
        vec3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}
        vec3(const vec3<T> &v) : x(v.x), y(v.y), z(v.z) {}
        vec3<T>& operator=(const vec3<T>& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }
        // add
        vec3<T>& operator+(const vec3<T>& v) const
        {
            return vec3(x + v.x, y + v.y, z + v.z);
        }
        vec3<T>& operator+=(const vec3<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        // sub
        vec3<T>& operator-(const vec3<T>& v) const
        {
            return vec3(x - v.x, y - v.y, z - v.z);
        }
        vec3<T>& operator-=(const vec3<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }
        // mul
        template <typename U> vec3<T> operator*(U s) const
        {
            return vec3<T>(s * x, s * y, s * z);
        }
        template <typename U> vec3<T>& operator*=(U s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        // div
        template <typename U> vec3<T> operator/(U f) const
        {
            float inv = (float)1 / f;
            return vec3<T>(x * inv, y * inv, z * inv);
        }

        template <typename U> vec3<T>& operator/=(U f)
        {
            float inv = (float)1 / f;
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }
        // other
        vec3<T> operator-() const { return vec3<T>(-x, -y, -z); }
        bool operator==(const vec3<T>& v) const { return x == v.x && y == v.y && z == v.z; }
        bool operator!=(const vec3<T>& v) const { return x != v.x || y != v.y || z != v.z; }
        // methods
        float len() const
        {
            return ::sqrt(x*x + y*y + z*z);
        }
        vec3& normalize()
        {
            float inv = 1.0f / len();
            x *= inv;
            y *= inv;
            z *= inv;
            return *this;
        }
        vec3& abs()
        {
            x = abs(x);
            y = abs(y);
            z = abs(z);
            return *this;
        }
    };

    typedef vec3<float> vec3f;
    typedef vec3<int> vec3i;

    template<typename T>
    float dot(const vec3<T>& a, const vec3<T>& b)
    {
        return (a.x*b.x + a.y*b.y + a.z*b.z);
    }

    template<typename T>
    vec3<T> cross(const vec3<T>& a, const vec3<T>& b)
    {
        return vec3<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
}

// =============================================================================
// === vec4
// =============================================================================
namespace aamath
{
    template<typename T>
    struct vec4
    {
        float x, y, z, w;
        // constructors
        vec4(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w) {}
        vec4(const vec4<T> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
        vec4<T>& operator=(const vec4<T>& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }
        // add
        vec4<T>& operator+(const vec4<T>& v) const
        {
            return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        vec4<T>& operator+=(const vec4<T>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }
        // sub
        vec4<T>& operator-(const vec4<T>& v) const
        {
            return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }
        vec4<T>& operator-=(const vec4<T>& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }
        // mul
        template <typename U> vec4<T> operator*(U s) const
        {
            return vec4<T>(s * x, s * y, s * z, s*w);
        }
        template <typename U> vec4<T>& operator*=(U s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }
        // div
        template <typename U> vec4<T> operator/(U f) const
        {
            float inv = (float)1 / f;
            return vec4<T>(x * inv, y * inv, z * inv, w*inv);
        }

        template <typename U> vec4<T>& operator/=(U f)
        {
            float inv = (float)1 / f;
            x *= inv;
            y *= inv;
            z *= inv;
            w *= inv;
            return *this;
        }
        // other
        vec4<T> operator-() const { return vec4<T>(-x, -y, -z, -w); }
        bool operator==(const vec4<T>& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        bool operator!=(const vec4<T>& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
        // methods
        float len() const
        {
            return ::sqrt(x*x + y*y + z*z + w*w);
        }
        vec4& normalize()
        {
            float l = len();
            return ((*this) / l);
        }
        vec4& abs()
        {
            x = abs(x);
            y = abs(y);
            z = abs(z);
            w = abs(w);
            return *this;
        }
    };

    typedef vec4<float> vec4f;
    typedef vec4<int> vec4i;

    template<typename T>
    float dot(const vec4<T>& a, const vec4<T>& b)
    {
        return (a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w);
    }
}

// =============================================================================
// === euler order
// =============================================================================

namespace aamath
{
    enum euler { XYZ, XZY, YXZ, YZX, ZXY, ZYX };
}

// =============================================================================
// === quaternion
// =============================================================================

namespace aamath
{
    struct quaternion
    {
        float x, y, z, w;

        quaternion(float x = 0, float y = 0, float z = 0, float w = 1)
            : x(x), y(y), z(z), w(w)
        {}

        quaternion& set(float _x = 0, float _y = 0, float _z = 0, float _w = 1)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
            return *this;
        }

        quaternion& setFromEuler(euler order, vec3f v)
        {
            float c1 = cos(v.x / 2);
            float c2 = cos(v.y / 2);
            float c3 = cos(v.z / 2);
            float s1 = sin(v.x / 2);
            float s2 = sin(v.y / 2);
            float s3 = sin(v.z / 2);

            if (order == XYZ)
            {
                x = s1 * c2 * c3 + c1 * s2 * s3;
                y = c1 * s2 * c3 - s1 * c2 * s3;
                z = c1 * c2 * s3 + s1 * s2 * c3;
                w = c1 * c2 * c3 - s1 * s2 * s3;
            }
            else if (order == YXZ)
            {
                x = s1 * c2 * c3 + c1 * s2 * s3;
                y = c1 * s2 * c3 - s1 * c2 * s3;
                z = c1 * c2 * s3 - s1 * s2 * c3;
                w = c1 * c2 * c3 + s1 * s2 * s3;
            }
            else if (order == ZXY)
            {
                x = s1 * c2 * c3 - c1 * s2 * s3;
                y = c1 * s2 * c3 + s1 * c2 * s3;
                z = c1 * c2 * s3 + s1 * s2 * c3;
                w = c1 * c2 * c3 - s1 * s2 * s3;
            }
            else if (order == ZYX)
            {
                x = s1 * c2 * c3 - c1 * s2 * s3;
                y = c1 * s2 * c3 + s1 * c2 * s3;
                z = c1 * c2 * s3 - s1 * s2 * c3;
                w = c1 * c2 * c3 + s1 * s2 * s3;
            }
            else if (order == YZX)
            {
                x = s1 * c2 * c3 + c1 * s2 * s3;
                y = c1 * s2 * c3 + s1 * c2 * s3;
                z = c1 * c2 * s3 - s1 * s2 * c3;
                w = c1 * c2 * c3 - s1 * s2 * s3;
            }
            else if (order == XZY)
            {
                x = s1 * c2 * c3 - c1 * s2 * s3;
                y = c1 * s2 * c3 - s1 * c2 * s3;
                z = c1 * c2 * s3 + s1 * s2 * c3;
                w = c1 * c2 * c3 + s1 * s2 * s3;
            }

            return *this;
        }
    };
}

// =============================================================================
// === mat3
// =============================================================================

namespace aamath
{
    template<typename T>
    struct mat3
    {
        T e[9];

        T& operator[](int idx)
        {
            return e[idx];
        }
    };

    typedef mat3<float> mat3f;
    typedef mat3<int> mat3i;
}

// =============================================================================
// === mat4
// =============================================================================

namespace aamath
{
    struct mat4
    {
        float e[16];

        float& operator[](int idx)
        {
            return e[idx];
        }

        void set(float jj, float jd, float jt, float jc, float dj, float dd, float dt, float dc, float tj, float td, float tt, float tc, float cj, float cd, float ct, float cc)
        {
            e[0] = jj; e[1] = jd; e[2] = jt; e[3] = jc;
            e[4] = dj; e[5] = dd; e[6] = dt; e[7] = dc;
            e[8] = tj; e[9] = td; e[10] = tt; e[11] = tc;
            e[12] = cj; e[13] = cd; e[14] = ct; e[15] = cc;
        }

        void zero()
        {
            set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        }

        void identity()
        {
            set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        }

        mat4()
        {
            e[0] = 1; e[1] = 0; e[2] = 0; e[3] = 0;
            e[4] = 0; e[5] = 1; e[6] = 0; e[7] = 0;
            e[8] = 0; e[9] = 0; e[10] = 1; e[11] = 0;
            e[12] = 0; e[13] = 0; e[14] = 0; e[15] = 1;
        }

        mat4(const mat4& m)
        {
            for (uint8_t i = 0; i < 16; i++) e[i] = m.e[i];
        }

        mat4& transpose(void)
        {
            float tmp;
            tmp = e[1]; e[1] = e[4]; e[4] = tmp;
            tmp = e[2]; e[2] = e[8]; e[8] = tmp;
            tmp = e[6]; e[6] = e[9]; e[9] = tmp;

            tmp = e[3]; e[3] = e[12]; e[12] = tmp;
            tmp = e[7]; e[7] = e[13]; e[13] = tmp;
            tmp = e[11]; e[11] = e[14]; e[14] = tmp;

            return *this;
        }

        mat4& orthographic(float left, float right, float bottom, float top, float _near, float _far)
        {
            float w = right - left;
            float h = top - bottom;
            float p = _far - _near;
            float x = (right + left) / w;
            float y = (top + bottom) / h;
            float z = (_far + _near) / p;
            e[0] = 2 / w;   e[4] = 0;       e[8] = 0;       e[12] = 0;
            e[1] = 0;       e[5] = 2 / h;   e[9] = 0;       e[13] = 0;
            e[2] = 0;       e[6] = 0;       e[10] = -2 / p; e[14] = 0;
            e[3] = -x;      e[7] = -y;      e[11] = -z;     e[15] = 1;

            return *this;
        }

        mat4& frustum(float left, float right, float bottom, float top, float _near, float _far)
        {
            float x = 2 * _near / (right - left);
            float y = 2 * _near / (top - bottom);
            float a = (right + left) / (right - left);
            float b = (top + bottom) / (top - bottom);
            float c = -(_far + _near) / (_far - _near);
            float d = -2 * _far * _near / (_far - _near);
            e[0] = x; e[4] = 0; e[8] = a;   e[12] = 0;
            e[1] = 0; e[5] = y; e[9] = b;   e[13] = 0;
            e[2] = 0; e[6] = 0; e[10] = c;  e[14] = d;
            e[3] = 0; e[7] = 0; e[11] = -1; e[15] = 0;
            
            return *this;
        }

        mat4& lookAt(const vec3f& eye, const vec3f& target, const vec3f& up)
        {
            /*vec3f x, y, z;
            z = (eye - target).normalize();
            if (z.len() == 0) { z.z = 1; }
            x = (cross(up, z)).normalize();
            if (x.len() == 0)
            {
                z.x += 0.0001;
                x = (cross(up, z)).normalize();
            }
            y = cross(z, x);

            e[0] = x.x; e[4] = y.x; e[8] = z.x;
            e[1] = x.y; e[5] = y.y; e[9] = z.y;
            e[2] = x.z; e[6] = y.z; e[10] = z.z;

            */

            /*vec3f f = (target - eye).normalize();
            vec3f u = vec3f(up).normalize();
            vec3f s = cross(f, u);
            u = cross(s, f);
            e[0] = s.x; e[1] = s.y; e[2] = s.z; e[3] = -eye.x;
            e[4] = u.x; e[5] = u.y; e[6] = u.z; e[7] = -eye.y;
            e[8] = -f.x; e[9] = -f.y; e[10] = -f.z; e[11] = -eye.z;
            e[12] = 0; e[13] = 0; e[14] = 0; e[15] = 1;
            */

            vec3f zaxis = (target - eye).normalize();
            vec3f xaxis = (cross(up, zaxis)).normalize();
            vec3f yaxis = cross(zaxis, xaxis);

            e[0] = xaxis.x;             e[1] = yaxis.x;             e[2] = zaxis.x;             e[3] = 0;
            e[4] = xaxis.y;             e[5] = yaxis.y;             e[6] = zaxis.y;             e[7] = 0;
            e[8] = xaxis.z;             e[9] = yaxis.z;             e[10] = zaxis.z;            e[11] = 0;
            e[12] = -dot(xaxis, eye);   e[13] = -dot(yaxis, eye);   e[14] = -dot(zaxis, eye);   e[15] = 1;

            //transpose();
            return *this;
            
        }

        vec4f operator*(vec4f& v)
        {
            float x = v.x;
            float y = v.y;
            float z = v.z;
            float w = v.w;

            vec4f res;

            res.x = e[0] * x + e[4] * y + e[8] * z + e[12] * w;
            res.y = e[1] * x + e[5] * y + e[9] * z + e[13] * w;
            res.z = e[2] * x + e[6] * y + e[10] * z + e[14] * w;
            res.w = e[3] * x + e[7] * y + e[11] * z + e[15] * w;

            return res;
        }

        vec4f operator*(vec3f& v)
        {
            return (*this)*vec4f({ v.x, v.y, v.z, 1 });
        }

        mat4& perspectiveCamera(float fov, float aspect, float _near, float _far)
        {
            float ymax = _near * tan(((double)fov * 0.5)*M_PI/180.0);
            float ymin = -ymax;
            float xmin = ymin * aspect;
            float xmax = ymax * aspect;
            return frustum(xmin, xmax, ymin, ymax, _near, _far);
        }

        mat4& orthographicCamera(float left, float right, float bottom, float top, float _near = 0.1f, float _far = 1000.0f)
        {
            float dx = (right - left) / 2;
            float dy = (top - bottom) / 2;
            float cx = (right + left) / 2;
            float cy = (top + bottom) / 2;
            return orthographic(cx - dx, cx + dx, cy + dy, cy - dy, _near, _far);
        }

        mat3f get3x3() const
        {
            mat3f m;
            m[0] = e[0];
            m[1] = e[1];
            m[2] = e[2];
            m[3] = e[4];
            m[4] = e[5];
            m[5] = e[6];
            m[6] = e[8];
            m[7] = e[9];
            m[8] = e[10];
            return m;
        }

        mat4& translate(vec3f& v)
        {
            e[12] += v.x;
            e[13] += v.y;
            e[14] += v.z;
            return *this;
        }

        mat4& setTranslate(vec3f& v)
        {
            e[12] = v.x;
            e[13] = v.y;
            e[14] = v.z;
            return *this;
        }

        mat4& setRotationX(float theta)
        {
            float c = cos(theta), s = sin(theta);

            set(
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1
                );

            return *this;
        }

        mat4& setRotationY(float theta)
        {
            float c = cos(theta), s = sin(theta);

            set(
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
                );

            return *this;
        }

        mat4& setRotationZ(float theta)
        {
            float c = cos(theta), s = sin(theta);

            set(
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
                );

            return *this;
        }

        mat4& setRotationAxis(vec3f& axis, float angle)
        {
            float c = cos(angle);
            float s = sin(angle);
            float t = 1.0f - c;
            float x = axis.x, y = axis.y, z = axis.z;
            float tx = t * x, ty = t * y;

            set(

                tx * x + c, tx * y - s * z, tx * z + s * y, 0,
                tx * y + s * z, ty * y + c, ty * z - s * x, 0,
                tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
                0, 0, 0, 1

                );

            return *this;
        }

        mat4& setRotationFromQuaternion(quaternion& q)
        {

            float x = q.x, y = q.y, z = q.z, w = q.w;
            float x2 = x + x, y2 = y + y, z2 = z + z;
            float xx = x * x2, xy = x * y2, xz = x * z2;
            float yy = y * y2, yz = y * z2, zz = z * z2;
            float wx = w * x2, wy = w * y2, wz = w * z2;

            e[0] = 1 - (yy + zz);
            e[4] = xy - wz;
            e[8] = xz + wy;

            e[1] = xy + wz;
            e[5] = 1 - (xx + zz);
            e[9] = yz - wx;

            e[2] = xz - wy;
            e[6] = yz + wx;
            e[10] = 1 - (xx + yy);

            // last column
            e[3] = 0;
            e[7] = 0;
            e[11] = 0;

            // bottom row
            e[12] = 0;
            e[13] = 0;
            e[14] = 0;
            e[15] = 1;

            return *this;
        }

        mat4& setScale(float x, float y, float z)
        {
            e[0] *= x; e[4] *= y; e[8] *= z;
            e[1] *= x; e[5] *= y; e[9] *= z;
            e[2] *= x; e[6] *= y; e[10] *= z;
            e[3] *= x; e[7] *= y; e[11] *= z;

            return *this;
        }

        mat4& setScale(vec3f v)
        {
            return setScale(v.x, v.y, v.z);
        }

        mat4& compose(vec3f t, quaternion r, vec3f s)
        {
            setRotationFromQuaternion(r);
            setScale(s);
            setTranslate(t);

            return *this;
        }
    };
}

// =============================================================================
// === transform
// =============================================================================

namespace aamath
{
    struct transform
    {
        vec3f R;
        vec3f T;
        vec3f S;
        euler order;
        mat4 mat;

        transform() : S(1, 1, 1), order(XYZ) {};

        void updateMat()
        {
            quaternion q;
            q.setFromEuler(order, R);
            mat.compose(T, q, S);
        }
    };
}