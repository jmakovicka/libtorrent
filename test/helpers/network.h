#ifndef LIBTORRENT_HELPER_NETWORK_H
#define LIBTORRENT_HELPER_NETWORK_H

#include <functional>
#include <string>
#include <cppunit/extensions/HelperMacros.h>

#include "torrent/net/address_info.h"

inline bool
compare_sin6_addr(in6_addr lhs, in6_addr rhs) {
  return std::equal(lhs.s6_addr, lhs.s6_addr + 16, rhs.s6_addr);
}

inline torrent::sa_unique_ptr
wrap_ai_get_first_sa(const char* nodename, const char* servname = nullptr, const addrinfo* hints = nullptr) {
  auto sa = torrent::ai_get_first_sa(nodename, servname, hints);

  CPPUNIT_ASSERT_MESSAGE(("wrap_ai_get_first_sa: nodename:'" + std::string(nodename) + "'").c_str(),
                        sa != nullptr);
  return sa;
}

inline torrent::c_sa_unique_ptr
wrap_ai_get_first_c_sa(const char* nodename, const char* servname = nullptr, const addrinfo* hints = nullptr) {
  auto sa = torrent::ai_get_first_sa(nodename, servname, hints);

  CPPUNIT_ASSERT_MESSAGE(("wrap_ai_get_first_sa: nodename:'" + std::string(nodename) + "'").c_str(),
                        sa != nullptr);
  return torrent::c_sa_unique_ptr(sa.release());
}

//
// Address info tests:
//

typedef std::function<int (torrent::ai_unique_ptr&)> test_ai_ref;

enum ai_flags_enum : int {
      aif_inet = 0x1,
      aif_inet6 = 0x2,
      aif_any = 0x4,
};

constexpr ai_flags_enum operator | (ai_flags_enum a, ai_flags_enum b) {
  return static_cast<ai_flags_enum>(static_cast<int>(a) | static_cast<int>(b));
}

template <ai_flags_enum ai_flags>
inline bool
test_valid_ai_ref(test_ai_ref ftor, uint16_t port = 0) {
  torrent::ai_unique_ptr ai;

  if (int err = ftor(ai)) {
    std::cerr << std::endl << "valid_ai_ref got error '" << gai_strerror(err) << "'" << std::endl;
    return false;
  }

  if ((ai_flags & aif_inet) && !torrent::sa_is_inet(ai->ai_addr))
    return false;

  if ((ai_flags & aif_inet6) && !torrent::sa_is_inet6(ai->ai_addr))
    return false;

  if (!!(ai_flags & aif_any) == !torrent::sa_is_any(ai->ai_addr))
    return false;

  if (torrent::sa_port(ai->ai_addr) != port)
    return false;

  return true;
}

inline bool
test_valid_ai_ref_err(test_ai_ref ftor, int expect_err) {
  torrent::ai_unique_ptr ai;
  int err = ftor(ai);

  if (err != expect_err) {
    std::cerr << std::endl << "ai_ref_err got wrong error, expected '" << gai_strerror(expect_err) << "', got '" << gai_strerror(err) << "'" << std::endl;
    return false;
  }

  return true;
}

#endif