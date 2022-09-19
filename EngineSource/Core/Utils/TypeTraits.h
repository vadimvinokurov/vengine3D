//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_TYPETRAITS_H
#define VENGINE3D_TYPETRAITS_H

template <typename R>
struct get_class_type_by_method
{
};

template <typename R, typename T, typename... Args>
struct get_class_type_by_method<R (T::*)(Args...) const>
{
	using type = T;
};

template <typename R, typename T, typename... Args>
struct get_class_type_by_method<R (T::*)(Args...)>
{
	using type = T;
};

template <typename T>
using get_class_type_by_method_t = typename get_class_type_by_method<T>::type;

#endif // VENGINE3D_TYPETRAITS_H
