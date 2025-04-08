#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>

namespace core
{

    template<typename T>
    class Vector3
    {
        public:
            static constexpr T MAX_NUMBER = std::numeric_limits<T>::max();
            union 
            {
                struct { T x, y, z; };
                struct { T r, g, b; };
                struct { T data[3]; };
            };
    
        T           Lenght() const;

        Vector3<T>  Normalize();
        Vector3<T>  GetNormalize();

        Vector3<T>  GetInv() const;

        Vector3();
        Vector3(const T& num);
        template<typename U>
        explicit Vector3(const std::vector<U>& vec);
        template<typename U>
        explicit Vector3(const Vector3<U>& vec);

        template<typename X, typename Y, typename Z>
        Vector3(const X& xx, const Y& yy, const Z& zz);

        ~Vector3() = default;

        template<typename U>
        Vector3<T> operator = (const Vector3<U>& vec);
        template<typename U>
        Vector3<T> operator + (const Vector3<U>& vec) const;
        template<typename U>
        Vector3<T> operator - (const Vector3<U>& vec) const;
        
        Vector3<T> operator - () const;

        Vector3<T> operator * (const T& scale) const;
        template <typename U>
        Vector3<T> operator * (const Vector3<U>& scale) const;


        Vector3<T> operator / (const T& scale) const;        
        template<typename U>
        Vector3<T> operator / (const Vector3<U>& scale) const;

        template<typename U>
        Vector3<T> operator += (const Vector3<U>& vec);
        template<typename U>
        Vector3<T> operator -= (const Vector3<U>& vec);
    

        Vector3<T> operator *= (const T& scale);
        Vector3<T> operator /= (const T& scale);

        T&          operator [] (const int& i);
        const T&    operator [] (const int& i) const;

        template<typename V>
        friend Vector3<T> operator* (const T& num, const Vector3<V>& vec)
        {
            T n = static_cast<T>(num);
            return Vector3<T>(vec * n);
        }

        friend std::ostream& operator << (std::ostream& os, const  Vector3<T>& vec)
        {
            for (int i = 0; i < 3; i++)
                os << vec[i] << " ";
            return os;
        }
    };

    template<typename T>
    class Vector2
    {
        public:
            static constexpr T MAX_NUMBER = std::numeric_limits<T>::max();
            union 
            {
                struct { T x, y; };
                struct { T r, g; };
                struct { T data[2]; };
            };
    
        T           Lenght() const;

        Vector2<T>  Normalize();
        Vector2<T>  GetNormalize();

        Vector2<T>  GetInv() const;

        Vector2();
        Vector2(const T& num);
        template<typename U>
        explicit Vector2(const std::vector<U>& vec);
        template<typename U>
        explicit Vector2(const Vector2<U>& vec);

        template<typename X, typename Y>
        Vector2(const X& xx, const Y& yy);


        template<typename U>
        Vector2<T> operator = (const Vector2<U>& vec);
        
        template<typename U>
        Vector2<T> operator + (const Vector2<U>& vec) const;
        template<typename U>
        Vector2<T> operator - (const Vector2<U>& vec) const;

        Vector2<T> operator - () const;

        Vector2<T> operator * (const T& scale) const;
        template<typename U>
        Vector2<T> operator * (const Vector2<U>& scale) const;
        Vector2<T> operator / (const T& scale) const;
        template<typename U>
        Vector2<T> operator / (const Vector2<U>& scale) const;

        template<typename U>
        Vector2<T> operator += (const Vector2<U>& vec);
        template<typename U>
        Vector2<T> operator -= (const Vector2<U>& vec);
    
        Vector2<T> operator *= (const T& scale);
        Vector2<T> operator /= (const T& scale);

        T&          operator [] (const int& i);
        const T&    operator [] (const int& i) const;

        template<typename V>
        friend Vector2<T> operator* (const T& num, const Vector2<V>& vec)
        {
            T n = static_cast<T>(num);
            return Vector2<T>(vec * n);
        }

        friend std::ostream& operator << (std::ostream& os, const  Vector2<T>& vec)
        {
            for (int i = 0; i < 2; i++)
                os << vec[i] << " ";
            return os;
        }
    };
}

#include "vector_inl.hpp"

