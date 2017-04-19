#include "LuaPbIntfImpl.h"

#include <google/protobuf/compiler/importer.h>  // for DiskSourceTree

LuaPbIntfImpl::LuaPbIntfImpl()
    : m_pDiskSourceTree(new DiskSourceTree)
{
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

// e.g. CompileProtoFile("bar/foo.proto")
bool LuaPbIntfImpl::CompileProtoFile(const string& sProtoFile)
{
    return false;
}

