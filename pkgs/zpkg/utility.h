/** 
 * @copyright Copyright © 2020-2024 code by zhaoj
 * 
 * LICENSE
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

 /**
 * @author zhaoj 286897655@qq.com
 * @brief 
 */

#ifndef ZPKG_UTILITY_H_
#define ZPKG_UTILITY_H_

#include <zpkg/portable.h>
#include <zpkg/memory.h>
#include <zpkg/error.h>
#include <zpkg/strings.h>

// Explicit declaration some variable
#define Z_UNUSED(x) (void)x;

// min max
#define Z_MIN(a, b) (((a) < (b))? (a) : (b))
#define Z_MAX(a, b) (((a) < (b))? (b) : (a))

// delete wrap
#define Z_DELETE_P(p) \
    delete p;         \
    p = NULL;         \
    (void)0

#define Z_DELETE_PA(pa) \
    delete[] pa;        \
    pa = NULL;          \
    (void)0

#define Z_IF_DELETE_P(p) \
    if (p) {             \
        delete p;        \
        p = NULL;        \
    }                    \
    (void)0

#define Z_IF_DELETE_PA(pa) \
    if (pa) {              \
        delete[] pa;       \
        pa = NULL;         \
    }                      \
    (void)0

// disable copy move macro
#define Z_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define Z_DISABLE_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;

#define Z_DISABLE_COPY_MOVE(Class) \
    Z_DISABLE_COPY(Class) \
    Z_DISABLE_MOVE(Class)

// virtual pure func mark
#ifndef PURE_FUNC
#define PURE_FUNC(Func) virtual Func = 0
#endif

namespace zpkg{

/// @brief wrap for unique_ptr
/// @tparam T 
template<typename T>
struct apply_unique{
    using unique = std::unique_ptr<T>;
};

/// @brief wrap for shared_ptr
/// @tparam T 
template<typename T>
struct apply_shared{
    using shared = std::shared_ptr<T>;
    using weak = std::weak_ptr<T>;
};

template<typename Fn>
class defer{
public:
    defer(Fn&& fn):fn_{std::forward<Fn>(fn)}{

    };
    ~defer(){
        fn_();
    }
private:
    const Fn fn_;
};
template<typename Fn>
inline std::unique_ptr<defer<Fn>> at_defer(Fn&& fn){
    return std::make_unique<defer<Fn>>(std::forward<Fn>(fn));
}

};//!namespace zpkg

#endif //!ZPKG_UTILITY_H_