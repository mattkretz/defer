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

#ifdef __COUNTER__
#define DEFER(lambda__) const auto &defer_object_##__COUNTER__ = ext::defer([&]() lambda__)
#else
#define DEFER(lambda__) const auto &defer_object_##__LINE__ = ext::defer([&]() lambda__)
#endif
}  // namespace ext
