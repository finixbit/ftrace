#include <iostream>
#include <fcntl.h>       // O_RDONLY 
#include <sys/stat.h> /* For the size of the file. , fstat */
#include <sys/mman.h> /* mmap, MAP_PRIVATE */
#include "disassembler.h"
#include "callsite.h"


void Disassembler::disassemble_callsites(
    uint8_t* code, int32_t code_size, std::intptr_t code_entry, bool print_ins = false) {
    
    csh capstone_handle;
    cs_insn *disassembled_ins;
    size_t count;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &capstone_handle) != CS_ERR_OK) {
        std::cout << "Initializing Capstone failed ..." << std::endl;
        exit(1);
    }

    count = cs_disasm(
        capstone_handle, code, code_size, code_entry, 0, &disassembled_ins);

    if (count > 0) {
        size_t j;
        for (j = 0; j < count; j++) {
            if(print_ins)
                print_disassembled_ins(disassembled_ins[j]);

            if(std::string(disassembled_ins[j].mnemonic) != "call")
                continue;

            auto addr = (std::intptr_t)disassembled_ins[j].address;
            if(!Callsite::m_callsites_map.count(addr)) {

                generate_callsite(
                    disassembled_ins[j], disassembled_ins[j+1], false);
            }
        }
        cs_free(disassembled_ins, count);
    } else {
        std::cout << "ERROR: Failed to disassemble program ..." << std::endl;
        exit(1);
    }
    cs_close(&capstone_handle);
}

void Disassembler::disassemble_ins(
    uint8_t* code, int32_t code_size, std::intptr_t code_entry, bool print_ins = false) {
    
    csh capstone_handle;
    cs_insn *disassembled_ins;
    size_t count;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &capstone_handle) != CS_ERR_OK) {
        std::cout << "Initializing Capstone failed ..." << std::endl;
        exit(1);
    }

    count = cs_disasm(
        capstone_handle, code, code_size, code_entry, 0, &disassembled_ins);

    if (count > 0) {
        size_t j;
        for (j = 0; j < count; j++) {
            m_disassembled_ins[
                disassembled_ins[j].address] = disassembled_ins[j];

            if(print_ins)
                print_disassembled_ins(disassembled_ins[j]);
        }
        cs_free(disassembled_ins, count);
    } else {
        std::cout << "ERROR: Failed to disassemble program ..." << std::endl;
    }
    cs_close(&capstone_handle);
}

void Disassembler::print_disassembled_ins(cs_insn &disassembled_ins) {
    printf("0x%" PRIx64 ":\t%s\t\t%s\n", disassembled_ins.address, 
                    disassembled_ins.mnemonic, disassembled_ins.op_str);
}

void Disassembler::generate_callsite(
    cs_insn &insn, cs_insn &next_insn, bool print)  {

    Callsite cs;
    cs.m_cs_address         = insn.address;
    cs.m_cs_name            = std::string(insn.op_str);
    cs.m_cs_target_resolved = false;
    
    try {
        std::intptr_t ptr = std::stol(std::string(insn.op_str), nullptr, 0);
        cs.m_cs_target_address = ptr;
        cs.m_cs_target_resolved = true;
    } catch(...) {}

    try {
        cs.m_cs_return_address = next_insn.address;
    } catch(...) {}

    Callsite::m_callsites_map[cs.m_cs_address] = cs;  

    if(print)
         print_disassembled_ins(insn); 
}