#ifndef LUAPBINTFIMPL_H
#define LUAPBINTFIMPL_H

#include <memory>  // for unique_ptr<>
#include <string>
#include <tuple>

class ErrorCollector;

namespace google {
namespace protobuf {
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

    // e.g. AddProtoPath("proto")
    // e.g. AddProtoPath("d:/proto")
    // AddProtoPath() and MapPath() must before CompileProtoFile().
    void AddProtoPath(const string& sProtoPath);
    void MapPath(const string& sVirtualPath, const string& sDiskPath);

    // e.g. CompileProtoFile("bar/foo.proto")
    std::tuple<bool, string>
    CompileProtoFile(const string& sProtoFile);

private:
    using DiskSourceTree = google::protobuf::compiler::DiskSourceTree;
    std::unique_ptr<DiskSourceTree> m_pDiskSourceTree;
    std::unique_ptr<ErrorCollector> m_pErrorCollector;
    using Importer = google::protobuf::compiler::Importer;
    std::unique_ptr<Importer> m_pImporter;
};  // class LuaPbIntfImpl

#endif  // LUAPBINTFIMPL_H
