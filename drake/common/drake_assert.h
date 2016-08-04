#pragma once

#include <type_traits>

#include "drake/drakeCommon_export.h"

/// @file
/// Provides Drake's assertion implementation.  This is intended to be used
/// both within Drake and by other software.  Drake's asserts can be armed
/// and disarmed independently from the system-wide asserts.

#ifdef DRAKE_DOXYGEN_CXX
/// @p DRAKE_ASSERT(condition) is similar to the built-in @p assert(condition)
/// from the C++ system header @p <cassert>.  Unless Drake's assertions are
/// disarmed by the pre-processor definitions listed below, @p DRAKE_ASSERT
/// will evaluate @p condition and iff the value is false will ::abort() the
/// program with a message showing at least the condition text, function name,
/// file, and line.
///
/// Assertions are enabled or disabled using the following pre-processor macros:
/// - If @p DRAKE_ENABLE_ASSERTS is defined, then @p DRAKE_ASSERT is armed.
/// - If @p DRAKE_DISABLE_ASSERTS is defined, then @p DRAKE_ASSERT is disarmed.
/// - If both macros are defined, then it is a compile-time error.
/// - If neither are defined, then NDEBUG governs assertions as usual.
///
/// This header will define exactly one of either @p DRAKE_ASSERT_IS_ARMED or
/// @p DRAKE_ASSERT_IS_DISARMED to indicate whether @p DRAKE_ASSERT is armed.
///
/// One difference versus the standard @p assert(condition) is that the
/// @p condition within @p DRAKE_ASSERT is always syntax-checked, even if
/// Drake's assertions are disarmed.
///
/// Treat @p DRAKE_ASSERT like a statement -- it must always be used
/// in block scope, and must always be followed by a semicolon.
#define DRAKE_ASSERT(condition)
/// Evaluates @p condition and iff the value is false will ::abort() the
/// program with a message showing at least the condition text, function name,
/// file, and line.
#define DRAKE_ABORT_UNLESS(condition)
/// Like @p DRAKE_ABORT_UNLESS(false), except that the meaningless "false"
/// condition test is not included in the message.
#define DRAKE_ABORT()
#else  //  DRAKE_DOXYGEN_CXX

// Users should NOT set these; only this header should set them.
#ifdef DRAKE_ASSERT_IS_ARMED
# error Unexpected DRAKE_ASSERT_IS_ARMED defined.
#endif
#ifdef DRAKE_ASSERT_IS_DISARMED
# error Unexpected DRAKE_ASSERT_IS_DISARMED defined.
#endif

// Decide whether Drake assertions are enabled.
#if defined(DRAKE_ENABLE_ASSERTS) && defined(DRAKE_DISABLE_ASSERTS)
# error Conflicting assertion toggles.
#elif defined(DRAKE_ENABLE_ASSERTS)
# define DRAKE_ASSERT_IS_ARMED
#elif defined(DRAKE_DISABLE_ASSERTS) || defined(NDEBUG)
# define DRAKE_ASSERT_IS_DISARMED
#else
# define DRAKE_ASSERT_IS_ARMED
#endif

namespace drake {
namespace detail {
// Abort the program with an error message.
DRAKECOMMON_EXPORT
void Abort(const char* condition, const char* func, const char* file, int line);
}
}

#define DRAKE_ABORT()                                           \
  ::drake::detail::Abort(nullptr, __func__, __FILE__, __LINE__)

#define DRAKE_ABORT_UNLESS(condition)                                   \
  do {                                                                  \
    if (!(condition)) {                                                 \
      ::drake::detail::Abort(#condition, __func__, __FILE__, __LINE__); \
    }                                                                   \
  } while (0)

#define DRAKE_ABORT_MSG(msg)                                        \
  ::drake::detail::Abort(msg, __func__, __FILE__, __LINE__)

#ifdef DRAKE_ASSERT_IS_ARMED
// Assertions are enabled.
# define DRAKE_ASSERT(condition) DRAKE_ABORT_UNLESS(condition)
#else
// Assertions are disabled, so just typecheck the expression.
# define DRAKE_ASSERT(condition) static_assert(                 \
      std::is_convertible<decltype(condition), bool>::value,    \
      "Assertion condition should be bool-convertible.")
#endif

#endif  // DRAKE_DOXYGEN_CXX
