/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


namespace ponder
{
    
namespace detail
{
    template <typename T>
    void destroy(const UserObject& object, bool destruct)
    {
        if (destruct)
            object.get<T*>() -> ~T();
        else
            delete object.get<T*>();
    }
    
    template <typename T>
    UserObject userObjectCreator(void* ptr)
    {
        return UserObject(static_cast<T*>(ptr));
    }
}

template <typename T>
inline ClassBuilder<T> Class::declare(IdRef id)
{
    Class& newClass =
        detail::ClassManager::instance().addClass(
            id.empty() ? detail::StaticTypeId<T>::get(false) : id);
    newClass.m_sizeof = sizeof(T);
    newClass.m_destructor = &detail::destroy<T>;
    newClass.m_userObjectCreator = &detail::userObjectCreator<T>;
    return ClassBuilder<T>(newClass);
}

template <typename T>
inline void Class::undeclare(IdRef id)
{
    detail::ClassManager::instance().removeClass(
        id.empty() ? detail::StaticTypeId<T>::get(false) : id);
}

template <typename ...A>
inline UserObject Class::create(A... args) const
{
    Args a(args...);
    return construct(a);
}

inline Class::FunctionTable::Iterator Class::functionIterator() const
{
    return m_functions.getIterator();
}

inline Class::PropertyTable::Iterator Class::propertyIterator() const
{
    return m_properties.getIterator();
}

inline UserObject Class::getUserObjectFromPointer(void* ptr) const
{
    return m_userObjectCreator(ptr);
}

} // namespace ponder
