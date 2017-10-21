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
class DynamicMessageFactory;
class Message;
class MethodDescriptor;
class ServiceDescriptor;
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

    // Input file must be relative to proto paths.
    // e.g. ImportProtoFile("bar/foo.proto")
    void ImportProtoFile(const string& sProtoFile);

    // Make a dynamic Message.
    MessageSptr MakeSharedMessage(const string& sTypeName) const;

    // Encode lua table to string.
    string Encode(const string& sMsgTypeName, const LuaRef& luaTable) const;

    // Decode string to lua message table. Return a lua table.
    // Return nil if parse failed.
    LuaRef Decode(lua_State* L, const string& sMsgTypeName,
        const string& sData) const;

    LuaRef GetServiceDescriptorTbl(lua_State* L, const string& sServiceName) const;

    // Get RPC request or response message type.
    string GetRpcInputName(const string& sServiceName,
        const string& sMethodName) const;
    string GetRpcOutputName(const string& sServiceName,
        const string& sMethodName) const;

    bool IsRpcClientStreaming(const string& sServiceName,
        const string& sMethodName) const;
    bool IsRpcServerStreaming(const string& sServiceName,
        const string& sMethodName) const;

private:
    const google::protobuf::ServiceDescriptor*
        GetServiceDescriptor(const string& sServiceName) const;
    const google::protobuf::MethodDescriptor& FindRpcMethod(
        const string& sServiceName, const string& sMethodName) const;

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
