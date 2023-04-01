//
// Created by aalisher on 4/1/23.
//

#pragma once

#define DEFINE_HAS_MEMBER(trait, symbol)                           \
    template <typename T>                                          \
    class trait {                                                  \
       private:                                                    \
        typedef char true_type[1];                                 \
        typedef char false_type[2];                                \
                                                                   \
        template <typename U>                                      \
        static false_type& check(...);                             \
                                                                   \
        template <typename U>                                      \
        static true_type& check(decltype(&U::symbol));             \
                                                                   \
       public:                                                     \
        enum { value = sizeof(check<T>(0)) == sizeof(true_type) }; \
    };                                                             \
                                                                   \
    template <typename T>                                          \
    constexpr bool trait##_v = trait<T>::value
