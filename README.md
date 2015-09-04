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
```
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
