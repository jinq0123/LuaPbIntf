#ifndef LUAPBINTFIMPL_H
#define LUAPBINTFIMPL_H

#include <string>
#include <memory>  // for unique_ptr<>

namespace google {
namespace protobuf {
namespace compiler {
class DiskSourceTree;
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

    // e.g. AddProtoPath("proto")
    // e.g. AddProtoPath("d:/proto")
    // AddProtoPath() and MapPath() must before CompileProtoFile().
    void AddProtoPath(const string& sProtoPath);
    void MapPath(const string& sVirtualPath, const string& sDiskPath);

    // e.g. CompileProtoFile("bar/foo.proto")
    bool CompileProtoFile(const string& sProtoFile);

private:
    using DiskSourceTree = google::protobuf::compiler::DiskSourceTree;
    std::unique_ptr<DiskSourceTree> m_pDiskSourceTree;
};  // class LuaPbIntfImpl

#endif  // LUAPBINTFIMPL_H
