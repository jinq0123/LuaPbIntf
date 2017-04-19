# Reference

## Function

* luapbintf.add_proto_path(proto_path)
	+ Add proto path.
		- Like protoc -IPATH or --proto_path=PATH option,
		  to specify the directory in which to search for imports.
		- May add many times to set multiple proto paths,
		  and directories will be searched in order.
		- The current the current working directory is default added
		  before any other path.
		- Same as map_path("", proto_path)
	+ proto_path {string}: a proto path
	+ returns nil
	+ e.g.
		- add_proto_path("d:/proto_root")
		- add_proto_path("foo_proto")

* luapbintf.map_path(virtual_path, disk_path)
	+ Map a path on disk to a location in the SourceTree.
	  Details see Protobuf DiskSourceTree::MapPath().
	+ virtual_path {string}
	+ disk_path {string}
	+ returns nil
	+ e.g. map_path("bar", "foo/bar")

* luapbintf.compile_proto_file(proto_file)
	+ Compile the given file to a FileDescriptor.
	+ proto_file {string}: a proto file name
		- Must be relative to proto paths
	+ returns {true, ""} | {false, string}
	+ e.g. assert(pb.compile_proto_file("foo.proto"))


