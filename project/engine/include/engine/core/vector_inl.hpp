#pragma once
#include "vector.h"
#include <math.h>
#include <assert.h>
namespace core
{
    template<typename T>
    T Vector3<T>::Lenght() const
    {
        return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::Normalize()
    {
        T div = static_cast<T>(this->Lenght());
        (*this) = *this / div;
        return *this;
    }

    template<typename T>
    Vector3<T> Vector3<T>::GetNormalize()
    {
        T div = static_cast<T>(this->Lenght());
        return (*this) / div;
    }


    template<typename T>
    Vector3<T> Vector3<T>::GetInv() const
    {
        Vector3<T> result;
        result.data[0] = this->data[0] == 0 ? Vector3<T>::MAX_NUMBER : 1.0f / this->data[0];
        result.data[1] = this->data[1] == 0 ? Vector3<T>::MAX_NUMBER : 1.0f / this->data[1];
        result.data[2] = this->data[2] == 0 ? Vector3<T>::MAX_NUMBER : 1.0f / this->data[2];
        return result;
    }

    template<typename T>
    Vector3<T>::Vector3() : x(static_cast<T>(0.0f)), y(static_cast<T>(0.0f)), z(static_cast<T>(0.0f)) { }

    template<typename T>
    Vector3<T>::Vector3(const T& num) : x(num), y(num), z(num) { }

    template<typename T>
    template<typename U>
    Vector3<T>::Vector3(const std::vector<U>& vec)
    {
        assert(vec.size() == 3); 
        (*this)[0] = static_cast<T>(vec[0]);
        (*this)[1] = static_cast<T>(vec[1]);
        (*this)[2] = static_cast<T>(vec[2]);
    }

    template<typename T>
    template<typename U>
    Vector3<T>::Vector3(const Vector3<U>& vec)
    {
        assert(vec.size() == 3);
        (*this)[0] = static_cast<T>(vec[0]);
        (*this)[1] = static_cast<T>(vec[1]);
        (*this)[2] = static_cast<T>(vec[2]);

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
        t.data[0] = scale.data[0] * static_cast<T>(this->data[0]);
        t.data[1] = scale.data[1] * static_cast<T>(this->data[1]);
        t.data[2] = scale.data[2] * static_cast<T>(this->data[2]);
        return t;
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator / (const T& scale) const
    {
        if (scale == static_cast<T>(0.0))
        {
            return Vector3<T>(Vector3<T>::MAX_NUMBER);
        }
        T t = static_cast<T>(1.0 / scale);
        return (*this) * t;
    }

    template<typename T>
    template<typename U>
    Vector3<T> Vector3<T>::operator / (const Vector3<U>& scale) const
    {
        Vector3<T> t;
        t.data[0] = scale.data[0] == 0 ? Vector3<T>::MAX_NUMBER : static_cast<T>(this->data[0]) / scale.data[0];
        t.data[1] = scale.data[1] == 0 ? Vector3<T>::MAX_NUMBER : static_cast<T>(this->data[1]) / scale.data[1];
        t.data[2] = scale.data[2] == 0 ? Vector3<T>::MAX_NUMBER : static_cast<T>(this->data[2]) / scale.data[2];
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
        assert((i >= 0 && i < 3));
        return this->data[i];
    }

    template<typename T>
    const T& Vector3<T>::operator [] (const int& i) const
    {
        assert((i >= 0 && i < 3));
        return this->data[i];
    }



    template<typename T>
    T Vector2<T>::Lenght() const
    {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::Normalize()
    {
        T div = static_cast<T>(this->Lenght());
        return *this / div;
    }

    template<typename T>
    Vector2<T>  Vector2<T>::GetNormalize()
    {
        T div = static_cast<T>(this->Lenght());
        return (*this) / div;
    }

    template<typename T>
    Vector2<T> Vector2<T>::GetInv() const
    {
        Vector2<T> t;
        t.data[0] = this->data[0] == 0 ? Vector2<T>::MAX_NUMBER : 1.0f / this->data[0];
        t.data[1] = this->data[1] == 0 ? Vector2<T>::MAX_NUMBER : 1.0f / this->data[1];
        return t;
    }

    template<typename T>
    Vector2<T>::Vector2() { }

    template<typename T>
    Vector2<T>::Vector2(const T& num) : x(static_cast<T>(num)), y(static_cast<T>(num)){ }

    template<typename T>
    template<typename U>
    Vector2<T>::Vector2(const std::vector<U>& vec)
    {
        assert(vec.size() == 2);
        (*this)[0] = vec[0];
        (*this)[1] = vec[1];
    }

    template<typename T>
    template<typename U>
    Vector2<T>::Vector2(const Vector2<U>& vec)
    {
        (*this)[0] = static_cast<T>(vec[0]);
        (*this)[1] = static_cast<T>(vec[1]);
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
    template<typename U>
    Vector2<T> Vector2<T>::operator * (const Vector2<U>& scale)  const
    {
        return Vector2<T>(this->x * scale.x, this->y * scale.y);    
    }   

    template<typename T>
    Vector2<T> Vector2<T>::operator / (const T& scale) const
    {
        if (scale == static_cast<T>(0.0))
            return Vector2<T>(Vector2<T>::MAX_NUMBER);
        T t = static_cast<T>(1.0 / scale);
        return (*this) * t;
    }

    template<typename T>
    template<typename U>
    Vector2<T> Vector2<T>::operator / (const Vector2<U>& scale) const
    {
        Vector2<T> t;
        t.data[0] = scale.data[0] == 0 ? Vector2<T>::MAX_NUMBER : this->data[0] / scale.data[0];
        t.data[1] = scale.data[1] == 0 ? Vector2<T>::MAX_NUMBER : this->data[1] / scale.data[1];
        return t;
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
        assert((i >= 0 && i < 2));
        return this->data[i];
    }

    template<typename T>
    const T& Vector2<T>::operator [] (const int& i) const
    {
        assert((i >= 0 && i < 2)); 
        return this->data[i];
    }
}