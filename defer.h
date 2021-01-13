/*
Copyright © 2015 Matthias Kretz <kretz@kde.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of contributing organizations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

namespace ext
{
namespace detail
{
template <typename F> class defer_raii
{
public:
  // copy/move construction and any kind of assignment would lead to the cleanup function getting
  // called twice. We can't have that.
  defer_raii(defer_raii &&) = delete;
  defer_raii(const defer_raii &) = delete;
  defer_raii &operator=(const defer_raii &) = delete;
  defer_raii &operator=(defer_raii &&) = delete;

  // construct the object from the given callable
  template <typename FF> defer_raii(FF &&f) : cleanup_function(std::forward<FF>(f)) {}

  // when the object goes out of scope call the cleanup function
  ~defer_raii() { cleanup_function(); }

private:
  F cleanup_function;
};
}  // namespace detail

template <typename F> detail::defer_raii<F> defer(F &&f)
{
  return {std::forward<F>(f)};
}

#define DEFER_ACTUALLY_JOIN(x, y) x##y
#define DEFER_JOIN(x, y) DEFER_ACTUALLY_JOIN(x, y)
#ifdef __COUNTER__
  #define DEFER_UNIQUE_VARNAME(x) DEFER_JOIN(x, __COUNTER__)
#else
  #define DEFER_UNIQUE_VARNAME(x) DEFER_JOIN(x, __LINE__)
#endif

#define DEFER(lambda__) [[maybe_unused]] const auto& DEFER_UNIQUE_VARNAME(defer_object) = ext::defer([&]() lambda__)
}  // namespace ext
