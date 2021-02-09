#ifndef SANDPILE__UDCS_H_
#define SANDPILE__UDCS_H_

#include <algorithm>
#include <numeric>

namespace detail{
template <typename T> struct next_t {};
template <> struct next_t<uint64_t>      { typedef uint32_t type; };
template <> struct next_t<uint32_t>       { typedef uint16_t type; };
template <> struct next_t<uint16_t>     { typedef uint8_t type; };

template <typename Max_t, Max_t upto_n, typename Best_t=Max_t, typename Try_t=uint64_t, bool try_is_better = (sizeof(Try_t) <= sizeof(Best_t) && upto_n == Max_t(Try_t(upto_n)))>
struct tight_int {
  typedef typename tight_int<Max_t, upto_n, Best_t, typename next_t<Try_t>::type>::type type;
};

template <typename Max_t, Max_t upto_n, typename Best_t, typename Try_t>
struct tight_int<Max_t, upto_n, Best_t, Try_t, true>  {
  typedef typename tight_int<Max_t, upto_n, Try_t, typename next_t<Try_t>::type>::type type;
};

template <typename Max_t, Max_t upto_n, typename Best_t>
struct tight_int<Max_t, upto_n, Best_t, uint8_t, true>  {
  typedef uint8_t type;
};

template <typename Max_t, Max_t upto_n, typename Best_t>
struct tight_int<Max_t, upto_n, Best_t, uint8_t, false> {
  typedef Best_t type;
};
}

template<size_t N, typename T = typename detail::tight_int<size_t,N>::type  >
class unsigned_dat_compact_set{
public:
  unsigned_dat_compact_set(T size=0) : ssize(size) {
    std::iota(content.begin(),content.end(),0);
    std::iota(content_map.begin(),content_map.end(),0);
  }
  void clear() {ssize = 0;}
  void fill() {ssize = N;}
  bool insert(T x) {
    if(size_t x_idx = content_map.at(x); x_idx < ssize) {
      return false; //already in
    } else {
      T y = content.at(ssize);
      std::swap(content[ssize],content[x_idx]);
      std::swap(content_map[x],content_map[y]);
      ++ssize;
      return true;
    }
  }
  size_t size() const {return ssize;}
  auto begin() const { return content.cbegin(); }
  auto end() const { return content.cbegin()+ssize; }
  bool erase(size_t x) {
    if(size_t x_idx = content_map.at(x); x_idx < ssize) {
      --ssize;
      T y = content.at(ssize);
      std::swap(content[ssize],content[x_idx]);
      std::swap(content_map[x],content_map[y]);
      return true;
    } else {
      return false; //already out
    }
  }
  bool contains(size_t x) const {
    return content_map.at(x) < ssize;
  }
  bool empty() const {return ssize==0;}
private:
  T ssize;
  std::array<T,N> content, content_map;
};

#endif //SANDPILE__UDCS_H_
