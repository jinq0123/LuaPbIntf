# Reference

## Functions

* `luapbintf.add_proto_path(proto_path)`
	+ Add proto path.
		- Like protoc -IPATH or --proto_path=PATH option,
		  to specify the directory in which to search for imports.
		- May add many times to set multiple proto paths,
		  and directories will be searched in order.
		- The current working directory is default added before any other path.
		- Same as `map_path("", proto_path)`
	+ `proto_path` {string}: a proto path
	+ returns nil
	+ e.g.
		- `add_proto_path("d:/proto_root")`
		- `add_proto_path("foo_proto")`

* `luapbintf.map_path(virtual_path, disk_path)`
	+ Map a path on disk to a location in the SourceTree.
	  Details see Protobuf `DiskSourceTree::MapPath()`.
	+ `virtual_path` {string}
	+ `disk_path` {string}
	+ returns nil
	+ e.g. `map_path("bar", "foo/bar")`

* `luapbintf.import_proto_file(proto_file)`
	+ Import the given file to a FileDescriptor.
		- Dependencies are imported automatically.
	+ `proto_file` {string}: a proto file name
		- Must be relative to proto paths
	+ returns nil
	+ e.g. `import_proto_file("foo.proto")`

* `luapbintf.encode(message_type_name, tbl)`
	+ Encode message table to string.
	+ `message_type_name` {string} 
	+ `tbl` {table} message table
		- Non-exist field will error
			```
			> pb.encode("test.TestMsg", {abcd = 123})
			stdin:1: Message test.TestMsg has no field: abcd
			```
		- Non-string index is allowed and is ingored.
			* `pb.encode("test.TestMsg", {[print]=123})` is same as
			  `pb.encode("test.TestMsg", {})`
	+ returns {string}
	+ e.g. `encode("test.TestMsg", {})`

* `luapbintf.decode(message_type_name, data)`
	+ Decode string to message table.
	+ `message_type_name` {string}
	+ `data` {string} encoded message string
	+ returns nil | {table}
		- returns nil if failed to parse message
		- otherwise returns a message table
	+ e.g. `decode("test.TestMsg", "")`

* `luapbintf.get_rpc_input_name(service_name, method_name)`
	+ Get rpc service method input message type full name.
	+ service_name {string}
	+ method_name {string}
	+ returns {string}
	+ e.g. `assert(pb.get_rpc_input_name("test.Test", "Foo") == "test.TestMsg")`
* `luapbintf.get_rpc_output_name(service_name, method_name)`
	+ Get rpc service method output message type full name.
	+ service_name {string}
	+ method_name {string}
	+ returns {string}
	+ e.g. `assert(pb.get_rpc_output_name("test.Test", "Foo") == "test.CommonMsg")`
