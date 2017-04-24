#ifndef DETAIL_LUAPBINTFIMPL_H
#define DETAIL_LUAPBINTFIMPL_H

#include "MessageSptr.h"  // for MessageSptr

#include <memory>  // for unique_ptr<>
#include <string>

class ErrorCollector;
struct lua_State;

namespace LuaIntf {
class LuaRef;
}  // namespace LuaIntf

namespace google {
namespace protobuf {
class Message;
class DynamicMessageFactory;
namespace compiler {
class DiskSourceTree;
class Importer;
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

class LuaPbIntfImpl
{
public:
    LuaPbIntfImpl();
    virtual ~LuaPbIntfImpl();

public:
    using string = std::string;
    using LuaRef = LuaIntf::LuaRef;

    // e.g. AddProtoPath("proto")
    // e.g. AddProtoPath("d:/proto")
    void AddProtoPath(const string& sProtoPath);
    void MapPath(const string& sVirtualPath, const string& sDiskPath);

    // e.g. ImportProtoFile("bar/foo.proto")
    void ImportProtoFile(const string& sProtoFile);

    MessageSptr MakeSharedMessage(const string& sTypeName) const;

    string Encode(const string& sMsgTypeName, const LuaRef& luaTable) const;

    // Return lua table.
    LuaRef Decode(lua_State* L, const string& sMsgTypeName,
        const string& sData) const;

private:
    using DiskSourceTree = google::protobuf::compiler::DiskSourceTree;
    using Importer = google::protobuf::compiler::Importer;
    using MsgFactory = google::protobuf::DynamicMessageFactory;

    std::unique_ptr<DiskSourceTree> m_pDiskSourceTree;
    std::unique_ptr<ErrorCollector> m_pErrorCollector;
    std::unique_ptr<Importer> m_pImporter;
    std::unique_ptr<MsgFactory> m_pMsgFactory;
};  // class LuaPbIntfImpl

#endif  // DETAIL_LUAPBINTFIMPL_H
