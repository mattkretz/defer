namespace ext
{
namespace detail
{
template <typename F> class defer_raii
{
public:
  defer_raii(const defer_raii &) = default;
  defer_raii(defer_raii &&) = default;
  template <typename FF> defer_raii(FF &&f) : cleanup_function(std::forward<FF>(f)) {}
  ~defer_raii() { cleanup_function(); }

private:
  F cleanup_function;
};
}  // namespace detail

template <typename F> detail::defer_raii<F> defer(F &&f)
{
  return detail::defer_raii<typename std::remove_reference<F>::type>{std::forward<F>(f)};
}

#ifdef __COUNTER__
#define DEFER(lambda__) const auto &defer_object_##__COUNTER__ = ext::defer([&]() lambda__)
#else
#define DEFER(lambda__) const auto &defer_object_##__LINE__ = ext::defer([&]() lambda__)
#endif
}  // namespace ext
