#ifndef DETAIL_MESSAGESPTR_H
#define DETAIL_MESSAGESPTR_H

#include <memory>  // for shared_ptr<>

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

using MessageSptr = std::shared_ptr<google::protobuf::Message>;

#endif  // DETAIL_MESSAGESPTR_H

