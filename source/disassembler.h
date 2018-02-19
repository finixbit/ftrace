#ifndef _H_DISASSEMBER_
#define _H_DISASSEMBER_

#include <vector> // vector
#include <map> // map
#include <unordered_map> // unordered_map
#include <link.h> /* ElfW */
#include <capstone/capstone.h> //cs_insn


class Disassembler {
    public:
        void disassemble_callsites(
            uint8_t* code, int32_t size, std::intptr_t code_entry, bool print_ins);

        void disassemble_ins(
            uint8_t* code, int32_t size, std::intptr_t code_entry, bool print_ins);

        void generate_callsite(
            cs_insn &insn, cs_insn &next_insn, bool print); 

    private:
        void print_disassembled_ins(cs_insn &disassembled_ins);
        std::map<std::intptr_t, cs_insn> m_disassembled_ins;

};

#endif