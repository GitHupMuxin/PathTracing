#pragma once
#include "Vector.h"
#include <math.h>
namespace MATH
{
    template<typename T>
    T Vector3<T>::error = static_cast<T>(0.0);
    template<typename T>
    T Vector3<T>::lenght() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::normalize()
    {
        T div = static_cast<T>(this->lenght());
        (*this) = *this / div;
        return *this;
    }

    template<typename T>
    Vector3<T> Vector3<T>::getNormalize()
    {
        T div = static_cast<T>(this->lenght());
        return (*this) / div;
    }


    template<typename T>
    Vector3<T> Vector3<T>::getInv() const
    {
        // if (this->x == 0 || this->y == 0 || this->z == 0)
        // {
        //     std::cout << "error use function vector3 is " << *this << "class Vector3<T> function Vector3<T> getInv() const\n"; 
        // }
        return Vector3<T>(1.0 / this->x, 1.0 / this->y, 1.0 / this->z);
    }

    template<typename T>
    Vector3<T>::Vector3() : x(static_cast<T>(0.0f)), y(static_cast<T>(0.0f)), z(static_cast<T>(0.0f)) { }

    template<typename T>
    Vector3<T>::Vector3(const T& num) : x(num), y(num), z(num) { }

    template<typename T>
    template<typename U>
    Vector3<T>::Vector3(const std::vector<U>& vec)
    {
        if (vec.size() != 3)
        {
            std::cout << "wrong vector input Class Vector3<T> Function Vector3<T>(const std::vector<U>& vec)\n";
            return ;
        }
        for (int i = 0; i < 3; i++)
        {
            (*this)[i] = vec[i];
        }

    }

    template<typename T>
    template<typename U>
    Vector3<T>::Vector3(const Vector3<U>& vec)
    {
        for (int i = 0; i < 3; i++)
            (*this)[i] = static_cast<T>(vec[i]);
    }
    template<typename T>
    template<typename X, typename Y, typename Z>
    Vector3<T>::Vector3(const X& xx, const Y& yy, const Z& zz) : x(xx), y(yy), z(zz) { }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator = (const Vector3<U>& vec)
    {
        this->x = static_cast<T>(vec.x);
        this->y = static_cast<T>(vec.y);
        this->z = static_cast<T>(vec.z);
        return *(this);

    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator + (const Vector3<U>& vec) const
    {   
        return Vector3<T>(this->x + static_cast<T>(vec.x), this->y + static_cast<T>(vec.y), this->z + static_cast<T>(vec.z));
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator - (const Vector3<U>& vec) const
    {
        return Vector3<T>(this->x - static_cast<T>(vec.x), this->y - static_cast<T>(vec.y), this->z - static_cast<T>(vec.z));
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator - () const 
    {
        return Vector3<T>(-this->x, -this->y, -this->z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator * (const T& scale)  const
    {
        T t = static_cast<T>(scale);
        return Vector3<T>(this->x * scale, this->y * scale, this->z * scale);    
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator * (const Vector3<U>& scale) const
    {
        Vector3<T> t;
        for (int i = 0; i < 3; i++)
            t.data[i] = static_cast<T>(scale.data[i]);   
        
        for (int i = 0; i < 3; i++)
            t.data[i] *= this->data[i];
        
        return t;
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator / (const T& scale) const
    {
        // if (scale == static_cast<T>(0.0))
        // {
        //     std::cout << "wrong div by zero Class Vector3<T> function Vector3<T> operator / (const U scale) \n";
        //     return Vector3<T>(0.0);
        // }
        T t = static_cast<T>(1.0 / scale);
        return (*this) * t;
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator / (const Vector3<U>& scale) const
    {
        MATH::Vector3<T> t;
        for (int i = 0; i < 3; i++)
            t.data[i] = static_cast<T>(1.0) / static_cast<T>(scale.data[i]);   

        for (int i = 0; i < 3; i++)
            t.data[i] = this->data[i] * t.data[i];
        
        return t;
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator += (const Vector3<U>& vec)
    {
        *(this) = *(this) + vec;
        return *(this);
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator -= (const Vector3<U>& vec)
    {
        *(this) = *(this) - vec;
        return (*this);
    }
    
    template<typename T>
    Vector3<T> Vector3<T>::operator *= (const T& scale)
    {
        (*this) = (*this) * scale;
        return (*this);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator /= (const T& scale)
    {
        (*this) = (*this) / scale;
        return (*this);
    }

    template<typename T>
    T& Vector3<T>::operator [] (const int& i)
    {
        if (i < 0 || i > 3)
        {   
            std::cout << "wrong index i = " << i << "Class Vector3<T> function T& operator[](const int i)\n";
            return Vector3<T>::error;
        }
        return this->data[i];
    }

    template<typename T>
    const T& Vector3<T>::operator [] (const int& i) const
    {
        if (i < 0 || i > 3)
        {
            std::cout << "wrong index i = " << i << "Class Vector3<T> function T& operator[](const int i)\n";
            return Vector3<T>::error;
        }
        return this->data[i];
    }



    template<typename T>
    T Vector2<T>::error = static_cast<T>(0.0);

    template<typename T>
    T Vector2<T>::lenght() const
    {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::normalize()
    {
        T div = static_cast<T>(this->lenght());
        return *this / div;
    }

    template<typename T>
    Vector2<T>  Vector2<T>::getNormalize()
    {
        T div = static_cast<T>(this->lenght());
        return (*this) / div;
    }

    template<typename T>
    Vector2<T> Vector2<T>::getInv() const
    {
        if (this->x == 0 || this->y == 0)
        {
            std::cout << "error use function vector2 is " << *this << "class Vector2<T> function Vector2<T> getInv() const\n"; 
        }
        return Vector2<T>(1.0 / this->x, 1.0 / this->y);
    }

    template<typename T>
    Vector2<T>::Vector2() { }

    template<typename T>
    Vector2<T>::Vector2(const T& num) : x(static_cast<T>(num)), y(static_cast<T>(num)){ }

    template<typename T>
    template<typename U>
    Vector2<T>::Vector2(const std::vector<U>& vec)
    {
        if (vec.size() != 2)
        {
            std::cout << "wrong vector input Class Vector2<T> Function Vector2<T>(const std::vector<U>& vec)\n";
            return ;
        }
        for (int i = 0; i < 2; i++)
        {
            (*this)[i] = vec[i];
        }
    }

    template<typename T>
    template<typename U>
    Vector2<T>::Vector2(const Vector2<U>& vec)
    {
        for (int i = 0; i < 2; i++)
            (*this)[i] = static_cast<T>(vec[i]);
    }
    
    template<typename T>
    template<typename X, typename Y>
    Vector2<T>::Vector2(const X& xx, const Y& yy) : x(xx), y(yy) { }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator = (const Vector2<U>& vec) 
    {
        this->x = static_cast<T>(vec.x);
        this->y = static_cast<T>(vec.y);
        return *(this);
    }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator + (const Vector2<U>& vec) const
    {   
        return Vector2<T>(this->x + static_cast<T>(vec.x), this->y + static_cast<T>(vec.y));
    }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator - (const Vector2<U>& vec) const
    {
        return Vector2<T>(this->x - static_cast<T>(vec.x), this->y - static_cast<T>(vec.y));
    }
    template<typename T>
    Vector2<T> Vector2<T>::operator - () const
    {
        return Vector2<T>(-this->x, -this->y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator * (const T& scale)  const
    {
        return Vector2<T>(this->x * scale, this->y * scale);    
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator / (const T& scale) const
    {
        if (scale == static_cast<T>(0.0))
        {
            std::cout << "wrong div by zero Class Vector2<T> function Vector2<T> operator / (const U scale) \n";
            return Vector2<T>(0.0);
        }
        T t = static_cast<T>(1.0 / scale);
        return (*this) * t;
    }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator += (const Vector2<U>& vec)
    {
        *(this) = *(this) + vec;
        return *(this);
    }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator -= (const Vector2<U>& vec)
    {
        *(this) = *(this) - vec;
        return (*this);
    }
    
    template<typename T>
    Vector2<T> Vector2<T>::operator *= (const T& scale)
    {
        (*this) = (*this) * scale;
        return (*this);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator /= (const T& scale)
    {
        (*this) = (*this) / scale;
        return (*this);
    }

    template<typename T>
    T& Vector2<T>::operator [] (const int& i)
    {
        if (i < 0 || i > 2)
        {   
            std::cout << "wrong index i = " << i << "Class Vector2<T> function T& operator[](const int i)\n";
            return Vector2<T>::error;
        }
        return this->data[i];
    }

    template<typename T>
    const T& Vector2<T>::operator [] (const int& i) const
    {
        if (i < 0 || i > 2)
        {
            std::cout << "wrong index i = " << i << "Class Vector2<T> function T& operator[](const int i)\n";
            return Vector2<T>::error;
        }
        return this->data[i];
    }
}