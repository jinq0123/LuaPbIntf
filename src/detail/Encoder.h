#ifndef DETAIL_ENCODER_H
#define DETAIL_ENCODER_H

#include <string>

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

namespace Encoder {
std::string Encode(const std::string& sMsgTypeName,
    const LuaIntf::LuaRef& tbl);
}  // namespace Encoder
#endif  // DETAIL_ENCODER_H

