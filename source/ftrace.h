#ifndef _H_LTRACE_
#define _H_LTRACE_

#include <unordered_map>
#include <linux/types.h> // pid_t
#include "elf-parser/elf_parser.h"
#include "disassembler.h"
#include "breakpoint.h"
using namespace elf_parser;

typedef std::unordered_map<std::intptr_t, relocation_t> RELOC_MAP;
typedef std::unordered_map<std::intptr_t, symbol_t> SYMBOL_MAP;

class Debugger: Elf_parser, Disassembler, Breakpoint  {
    public:
        Debugger (std::string &program_path): 
            m_program_path{program_path}, Elf_parser{program_path} {   
        }
        void run_debuggee(char* cmd[]);   
        void run_debugger(pid_t child_pid);

        RELOC_MAP m_relocs;
        SYMBOL_MAP m_symbols;
        
    private:
        std::string m_program_path;
        std::string m_program_argv;
        std::string m_proc_program_path; 
        pid_t m_child_pid;
        
        RELOC_MAP relocations_to_unordered_map(std::vector<relocation_t> &rels);
};

#endif