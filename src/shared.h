#ifndef MY_SHARED_H_
#define MY_SHARED_H_

#include <memory>
#include <vector>

extern std::vector<std::shared_ptr<void>> retained;

// Creates explicit refs in 'retained' in order to avoid scenario
// where shared ptrs seem to be unexpectedly garbage collected.
template <typename T>
inline std::shared_ptr<T> retain(std::shared_ptr<T> whatever) {
    retained.push_back(whatever);
    return whatever;
}

#endif // MY_SHARED_H_