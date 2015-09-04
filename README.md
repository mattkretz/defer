Defer a function/lambda until the end of the scope
==================================================

Inspired by the ``defer`` keyword in Go, this simple ``defer`` function in C++
provides similar functionality. In contrast to Go, the deferred function is
called at the end of the current scope. In Go the deferred function is only
called at the end of the current function scope.

The main use for this C++ ``defer`` function is to simplify resource cleanup
safety in simple situations where the definition of a complete new RAII class
would be overkill.

Example:
```C++
void f(const std::string &filename) {
  auto fp = std::fopen(filename.c_str(), "r");
  if (!fp) {
    std::perror("File opening failed");
    return;
  }
  DEFER({ std::fclose(fp); });
  // do stuff that can throw or return early. The file stream will be closed in
  // any case
}
```

There are two possible interfaces to using the ``defer`` function in ``defer.h``:

1. Either use the preprocessor macro (yuck) as shown above (this avoids some
boilerplate code/syntax). The macro constructs a lambda and expects the lambda
body as its argument.
2. Or declare a local object, which you capture from the return value of
``ext::defer``. Note that ``ext::defer`` works with any callable, so you could
also use a ``std::function`` or standard function pointer here.
```C++
const auto &defer_fclose = ext::defer([&fp] { std::fclose(fp); });
```
