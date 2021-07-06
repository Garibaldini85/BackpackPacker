/*
 *  BackpackPacker Copyright (C) 2021  Kambarov I. G.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Subsequent modifications must be distributed under the same license.
 */

#ifndef RANDOM_HPP
#define RANDOM_HPP
 
#include <random>
 
namespace details
{
    /// True if type T is applicable by a std::uniform_int_distribution
    template<class T>
    struct is_uniform_int {
        static constexpr bool value =
                std::is_same<T,              short>::value ||
                std::is_same<T,                int>::value ||
                std::is_same<T,               long>::value ||
                std::is_same<T,          long long>::value ||
                std::is_same<T,     unsigned short>::value ||
                std::is_same<T,       unsigned int>::value ||
                std::is_same<T,      unsigned long>::value ||
                std::is_same<T, unsigned long long>::value;
    };
 
    /// True if type T is applicable by a std::uniform_real_distribution
    template<class T>
    struct is_uniform_real {
        static constexpr bool value =
                std::is_same<T,       float>::value ||
                std::is_same<T,      double>::value ||
                std::is_same<T, long double>::value;
    };
}
 
class Random
{
    template <class T> using IntDist = std::uniform_int_distribution<T>;
    template <class T> using RealDist = std::uniform_real_distribution<T>;
 
public:
    template <class T>
    static typename std::enable_if<details::is_uniform_int<T>::value, T>::type get(T from = std::numeric_limits<T>::min(), T to = std::numeric_limits<T>::max())
    {
        if (from > to) std::swap(from, to);
        IntDist<T> dist{from, to};
        return dist(instance().engine());
    }
 
    template <class T>
    static typename std::enable_if<details::is_uniform_real<T>::value, T>::type get(T from = std::numeric_limits<T>::min(), T to = std::numeric_limits<T>::max())
    {
        if (from > to) std::swap(from, to);
        RealDist<T> dist{from, to};
        return dist(instance().engine());
    }
 
    std::mt19937& engine() { return m_mt; }
 
protected:
    static Random& instance()
    {
        static Random inst;
        return inst;
    }
 
private:
    std::random_device m_rd; 
    std::mt19937 m_mt;       
 
    Random() : m_mt(m_rd()) {}
    ~Random() {}
    Random(const Random&) = delete;
    Random& operator = (const Random&) = delete;
};
 
#endif // RANDOM_HPP
