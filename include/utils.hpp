#pragma once

#include <iostream>

/*
    1) Unary right fold (E op ...)
    becomes (E1 op (... op (EN-1 op EN)))

    2) Unary left fold (... op E) 
    becomes (((E1 op E2) op ...) op EN)

    3) Binary right fold (E op ... op I)
    becomes (E1 op (... op (EN−1 op (EN op I))))

    4) Binary left fold (I op ... op E)
    becomes ((((I op E1) op E2) op ...) op EN)
*/

namespace vke
{   
    using std::cout;
    using std::endl;

    template<class T>
    void printContainer(T&& container)
    {
        for (auto&& c : container) {
            std::cout << c << " ";
        }

        std::cout << std::endl;
    }

    template <class... T>
    void printCollapsed(const T&... args)
    {
        (cout << ... << args) << endl;
    }

    template<class U, class... T>
    void print(const U& u, const T&... args)
    {   
        ((cout << u) << ... << (cout << ' ', args)) << endl;
    }

    int getOne();
}