#include<tuple>
namespace magma::_detail {
template<class T, class U>
using tuple_cat_t = std::invoke_result_t<
        decltype(std::tuple_cat<T, U>),
        T, U
    >;
}
