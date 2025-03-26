#pragma once
#include "vector.h"
#include <functional>

namespace core
{
    typedef Vector2<int>        Vector2i;
    typedef Vector2<float>      Vector2f;
    typedef Vector2<double>     Vector2d;

    typedef Vector3<int>        Vector3i;
    typedef Vector3<float>      Vector3f;
    typedef Vector3<double>     Vector3d;

    template<typename T>
    class Vector3_Tool
    {
        public:
            static std::function<Vector3<T>(Vector3<T>& vv1, Vector3<T>& vv2)> MAX()
            {
                return [](Vector3<T>& vv1, Vector3<T>& vv2) ->Vector3<T>{
                    Vector3<T> t;
                    for (int i = 0; i < 3; i++)
                        t.data[i] = std::max(vv1.data[i], vv2.data[i]);
                    return t;
                };
            }

            static std::function<Vector3<T>(Vector3<T>& vv1, Vector3<T>& vv2)> MIN()
            {
                return [](Vector3<T>& vv1, Vector3<T>& vv2) ->Vector3<T>{
                    Vector3<T> t;
                    for (int i = 0; i < 3; i++)
                        t.data[i] = std::min(vv1.data[i], vv2.data[i]);
                    return t;
                };
            }
    };
    

    template<typename T>
    class Vector2_Tool
    {
        public:
            static std::function<Vector2<T>(Vector2<T>& vv1, Vector2<T>& vv2)> MAX()
            {
                return [](Vector2<T>& vv1, Vector2<T>& vv2) ->Vector2<T>{
                    Vector2<T> t;
                    for (int i = 0; i < 2; i++)
                        t.data[i] = std::max(vv1.data[i], vv2.data[i]);
                    return t;
                };
            }

            static std::function<Vector2<T>(Vector2<T>& vv1, Vector2<T>& vv2)> MIN()
            {
                return [](Vector2<T>& vv1, Vector2<T>& vv2) ->Vector2<T>{
                    Vector2<T> t;
                    for (int i = 0; i < 3; i++)
                        t.data[i] = std::min(vv1.data[i], vv2.data[i]);
                    return t;
                };
            }
    };


    template<typename T>
    T DotProduction(Vector2<T> v1, Vector2<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T>
    T DotProduction(Vector3<T> v1, Vector3<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename T>
    Vector3<T> CrossProduction(Vector3<T> v1, Vector3<T> v2)
    {
        return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template<typename T>
    Vector3<T> Process2Vector(Vector3<T>& v1, Vector3<T>& v2, std::function<Vector3<T> (Vector3<T>& vv1, Vector3<T>& vv2)> func)
    {
        return func(v1, v2);
    }
    
}
