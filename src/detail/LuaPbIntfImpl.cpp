#include "LuaPbIntfImpl.h"

#include "MessageSetter.h"
#include "MsgToTbl.h"

// for LuaException
#include <LuaIntf/LuaIntf.h>
#include <LuaIntf/LuaState.h>

#include <google/protobuf/compiler/importer.h>  // for DiskSourceTree
#include <google/protobuf/descriptor.h>  // for Descriptor
#include <google/protobuf/dynamic_message.h>  // for GetPrototype()
#include <google/protobuf/message.h>  // for Message

#include <sstream>  // for ostringstream

using namespace LuaIntf;
using namespace google::protobuf;

// See protobuf CommandLineInterface::Run().

class ErrorCollector : public compiler::MultiFileErrorCollector
{
public:
    void Clear() { m_sError.clear(); }
    const std::string& GetError() const { return m_sError; }

    // Only record the last error.
    void AddError(const std::string & filename,
        int line, int column, const std::string & message) override
    {
        std::ostringstream oss;
        oss << filename << ":" << line << ": " << message;
        m_sError = oss.str();
    }
private:
    std::string m_sError;
};

LuaPbIntfImpl::LuaPbIntfImpl()
    : m_pDiskSourceTree(new DiskSourceTree),  // unique_ptr
    m_pErrorCollector(new ErrorCollector),  // unique_ptr
    m_pImporter(new Importer(m_pDiskSourceTree.get(),  // unique_ptr
        m_pErrorCollector.get())),
    m_pMsgFactory(new MsgFactory)  // unique_ptr
{
    // The current dir is the default proto path.
    AddProtoPath("");
}

LuaPbIntfImpl::~LuaPbIntfImpl()
{
}

// e.g. AddProtoPath("proto")
// e.g. AddProtoPath("d:/proto")
void LuaPbIntfImpl::AddProtoPath(const string& sProtoPath)
{
    MapPath("", sProtoPath);
}

void LuaPbIntfImpl::MapPath(
    const string& sVirtualPath,
    const string& sDiskPath)
{
    m_pDiskSourceTree->MapPath(sVirtualPath, sDiskPath);
}

// e.g. ImportProtoFile("bar/foo.proto")
void LuaPbIntfImpl::ImportProtoFile(const string& sProtoFile)
{
    m_pErrorCollector->Clear();
    const FileDescriptor* pDesc = m_pImporter->Import(sProtoFile);
    if (pDesc) return;
    throw LuaException("Failed to import: " + m_pErrorCollector->GetError());
}

MessageSptr LuaPbIntfImpl::MakeSharedMessage(const string& sTypeName) const
{
    const Descriptor* pDesc = m_pImporter->pool()->
        FindMessageTypeByName(sTypeName);
    if (!pDesc) throw LuaException("No message type: " + sTypeName);
    const Message* pProtoType = m_pMsgFactory->GetPrototype(pDesc);
    if (!pProtoType) throw LuaException("No prototype for " + sTypeName);
    return MessageSptr(pProtoType->New());
}

std::string LuaPbIntfImpl::Encode(const string& sMsgTypeName,
    const LuaRef& luaTable) const
{
    luaTable.checkTable();  // Bad argument #-1 to 'encode' (table expected, got number)
    MessageSptr pMsg = MakeSharedMessage(sMsgTypeName);
    assert(pMsg);
    MessageSetter(*pMsg).SetMsg(luaTable);
    return pMsg->SerializeAsString();
}  // Encode()

LuaRef LuaPbIntfImpl::Decode(lua_State* L, const string& sMsgTypeName,
    const string& sData) const
{
    assert(L);
    MessageSptr pMsg = MakeSharedMessage(sMsgTypeName);
    assert(pMsg);
    if (pMsg->ParseFromString(sData))
        return MsgToTbl(*L, *pMsg).ToTbl();
    return LuaRef(L, nullptr);
}

LuaRef LuaPbIntfImpl::GetServiceDescriptorTbl(lua_State* L,
    const string& sServiceName) const
{
    assert(L);
    const google::protobuf::ServiceDescriptor* pDesc
        = GetServiceDescriptor(sServiceName);
    if (!pDesc)
        throw LuaIntf::LuaException("No such service: " + sServiceName);
    google::protobuf::ServiceDescriptorProto msg;
    pDesc->CopyTo(&msg);
    return MsgToTbl(*L, msg).ToTbl();
}

std::string LuaPbIntfImpl::GetRpcInputName(const string& sServiceName,
    const string& sMethodName) const
{
    return FindRpcMethod(sServiceName, sMethodName).input_type()->full_name();
}

std::string LuaPbIntfImpl::GetRpcOutputName(const string& sServiceName,
    const string& sMethodName) const
{
    return FindRpcMethod(sServiceName, sMethodName).output_type()->full_name();
}

bool LuaPbIntfImpl::IsRpcClientStreaming(const string& sServiceName,
    const string& sMethodName) const
{
    return FindRpcMethod(sServiceName, sMethodName).client_streaming();
}

bool LuaPbIntfImpl::IsRpcServerStreaming(const string& sServiceName,
    const string& sMethodName) const
{
    return FindRpcMethod(sServiceName, sMethodName).server_streaming();
}

const google::protobuf::ServiceDescriptor*
LuaPbIntfImpl::GetServiceDescriptor(const string& sServiceName) const
{
    return m_pImporter->pool()->FindServiceByName(sServiceName);
}

const MethodDescriptor& LuaPbIntfImpl::FindRpcMethod(
    const string& sServiceName, const string& sMethodName) const
{
    const ServiceDescriptor* pDesc = GetServiceDescriptor(sServiceName);
    if (!pDesc) throw LuaException("No such service: " + sServiceName);
    const MethodDescriptor* pMethod = pDesc->FindMethodByName(sMethodName);
    if (pMethod) return *pMethod;
    throw LuaException("No such method: " + sServiceName + "." + sMethodName);
}


