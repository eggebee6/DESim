#ifndef __DES_COMMON_H__
#define __DES_COMMON_H__

#include <cstdint>
#include <stdexcept>

namespace des
{
/** @addtogroup Common
* @{
*/

typedef uint64_t SimTime;     ///< Simulation time typedef

typedef uint32_t EventType;   ///< Event type typedef
typedef uint32_t EventTag;    ///< Event tag typedef

/**
* @brief  Causality exception class
*
*  To be thrown in case of a causality violation
*/
class CausalityException : public std::runtime_error
{
public:
  CausalityException(const char* what_arg = "") : std::runtime_error(what_arg)
  {}
};

/** @} */
} // End namespace

#endif
