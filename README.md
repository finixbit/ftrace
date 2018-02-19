# ftrace
Simple Function calls tracer 

Target source must be compiled with symbols information (see example 1).

And if not not compile with symbols breakpoints are set on call locations (see example 2)

# Required libraries
capstone, finixbit/elf-parser

# Installation

```
$ sudo apt-get install libcapstone-dev
$ git clone --recursive https://github.com/finixbit/ftrace.git
$ cd ftrace
$ make
```

# Usage 

```
./ftrace <cmd>
```

# Example 1
```
./ftrace /src/openssl dsaparam 1024

breakpoint[0x41cc6d]   call: _start 
breakpoint[0x467380]   call: __libc_csu_init 
breakpoint[0x416080]   call: _init 
breakpoint[0x41cd40]   call: frame_dummy 
breakpoint[0x41cce0]   call: register_tm_clones 
breakpoint[0x41c850]   call: main 
breakpoint[0x41f3e0]   call: app_malloc 
breakpoint[0x421440]   call: dup_bio_in 
breakpoint[0x421460]   call: dup_bio_out 
breakpoint[0x421480]   call: dup_bio_err 
breakpoint[0x41e340]   call: setup_ui_method 
breakpoint[0x434c90]   call: SortFnByName 
breakpoint[0x434c90]   call: SortFnByName 
breakpoint[0x434c90]   call: SortFnByName 
...<redacted>
breakpoint[0x434a90]   call: function_hash 
breakpoint[0x434a90]   call: function_hash 
breakpoint[0x434a90]   call: function_hash 
breakpoint[0x435700]   call: opt_progname 
breakpoint[0x434a90]   call: function_hash 
breakpoint[0x434aa0]   call: do_cmd 
breakpoint[0x434a90]   call: function_hash 
breakpoint[0x434a70]   call: function_cmp 
breakpoint[0x42cc10]   call: dsaparam_main 
breakpoint[0x435760]   call: opt_init 
breakpoint[0x436bf0]   call: opt_next 
breakpoint[0x4370d0]   call: opt_num_rest 
breakpoint[0x4370b0]   call: opt_rest 
breakpoint[0x436260]   call: opt_int 
breakpoint[0x4216f0]   call: bio_open_default 
breakpoint[0x4214b0]   call: bio_open_owner 
...<redacted>
breakpoint[0x42cb90]   call: dsa_cb 
breakpoint[0x42cb90]   call: dsa_cb 
breakpoint[0x42cb90]   call: dsa_cb 
breakpoint[0x41fea0]   call: release_engine 
breakpoint[0x41ce00]   call: app_RAND_write 
breakpoint[0x41e3c0]   call: destroy_ui_method 
breakpoint[0x41cd20]   call: __do_global_dtors_aux 
breakpoint[0x41cca0]   call: deregister_tm_clones 
breakpoint[0x4673f4]   call: _fini 
```

# Example 2
```
./ftrace /bin/ps

breakpoint[0x40b83e] call: fn_4201144
breakpoint[0x4035bb] call: fn_4206656
breakpoint[0x408f9d] call: fn_4227072
breakpoint[0x4090b0] call: fn_4223520
breakpoint[0x4090cf] call: fn_4237168
```

