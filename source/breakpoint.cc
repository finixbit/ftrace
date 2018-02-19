#include <iostream>
#include <sys/ptrace.h> /* ptrace */
#include <sys/wait.h> /* wait */
#include <inttypes.h> /* PRIx64 */
#include "breakpoint.h"
#include "callsite.h"
#include "symbol.h"


void Breakpoint::set_symbol_breakpoint(
    pid_t &child_pid, std::intptr_t &sym_address) {

    if(m_breakpoints_map.count(sym_address)) {
        auto &bp = m_breakpoints_map[sym_address];
        bp.m_is_symbol_address = true;

    } else {
        breakpoint_t bp;
        bp.m_bp_address = sym_address;
        bp.m_is_symbol_address = true;
      
        enable_breakpoint(child_pid, bp);
        m_breakpoints_map[sym_address] = bp;
    } 
}

void Breakpoint::set_callsite_breakpoint(
    pid_t &child_pid, std::intptr_t &cs_address, std::intptr_t &cs_return_address) {

    if(m_breakpoints_map.count(cs_address)) {
        auto &bp = m_breakpoints_map[cs_address];
        bp.m_is_cs_caller_address = true;
        bp.m_cs_caller_address = cs_address;

    } else {
        breakpoint_t bp;
        bp.m_bp_address = cs_address;
        bp.m_is_cs_caller_address = true;
        bp.m_cs_caller_address = cs_address;
      
        enable_breakpoint(child_pid, bp);
        m_breakpoints_map[cs_address] = bp;
    } 

    if(cs_return_address == 0)
        return;

    if(m_breakpoints_map.count(cs_return_address)) {
        auto &bp = m_breakpoints_map[cs_return_address];
        bp.m_is_cs_return_address = true;
        bp.m_cs_return_caller_address = cs_address;

    } else {
        breakpoint_t bp;
        bp.m_bp_address = cs_return_address;
        bp.m_is_cs_return_address = true;
        bp.m_cs_return_caller_address = cs_address;
      
        enable_breakpoint(child_pid, bp);
        m_breakpoints_map[cs_return_address] = bp;
    }  
}

void Breakpoint::continue_execution(pid_t &child_pid) {
    while(1) { 
        ptrace(PTRACE_CONT, child_pid, nullptr, nullptr);
        wait_for_signal(child_pid);
        step_over_breakpoint(child_pid);
    }
}

void Breakpoint::wait_for_signal(pid_t &child_pid) {
    int wait_status = 0;
    auto options = 0;
    auto i = waitpid(child_pid, &wait_status, options);
    
    if(WIFEXITED(wait_status) || (WIFSIGNALED(wait_status) && WTERMSIG(wait_status) == SIGKILL)) {
      std::cout << "[+] process " << child_pid << " terminated" << std::endl;
      return;
    }
}

void Breakpoint::step_over_breakpoint(pid_t &child_pid) {
    // - 1 because execution will go past the breakpoint
    auto possible_breakpoint_location = get_program_counter(child_pid) - 1;

    if (m_breakpoints_map.count(possible_breakpoint_location)) {
        auto& bp = m_breakpoints_map[possible_breakpoint_location];
        
        if (bp.m_enabled) {
            printf("breakpoint[0x%" PRIx64 "] ", bp.m_bp_address);

            if(bp.m_is_cs_return_address) {
                auto &cs = Callsite::m_callsites_map[bp.m_cs_return_caller_address];
                std::cout << "return: " << cs.m_cs_name << " ";
            }
            if(bp.m_is_cs_caller_address) {
                auto &cs = Callsite::m_callsites_map[bp.m_cs_caller_address];
                std::cout << "  call: " << cs.m_cs_name << " ";
            }
            if(bp.m_is_symbol_address) {
                auto &sym = Symbol::m_symbols_map[bp.m_bp_address];
                std::cout << "  call: " << sym.m_sym_name << " ";
            }
            printf("\n");

            auto previous_instruction_address = possible_breakpoint_location;
            set_program_counter(child_pid, previous_instruction_address);

            disable_breakpoint(child_pid, bp);
            ptrace(PTRACE_SINGLESTEP, child_pid, nullptr, nullptr);
            wait_for_signal(child_pid);

            enable_breakpoint(child_pid, bp);
        }
  } 
}

uint64_t Breakpoint::get_program_counter(pid_t &child_pid) {
    user_regs_struct regs = get_registers(child_pid);
    return (uint64_t)regs.rip;
}

void Breakpoint::set_program_counter(pid_t &child_pid, uint64_t &pc) {
    user_regs_struct regs = get_registers(child_pid);

    #ifdef __x86_64__
        regs.rip = pc;
    #else
        regs.eip = pc;
    #endif
    ptrace(PTRACE_SETREGS, child_pid, nullptr, &regs);
}

user_regs_struct Breakpoint::get_registers(pid_t &child_pid) {
    struct user_regs_struct regs;
    long esp, eax, ebx, edx, ecx, esi, edi, eip;
    #ifdef __x86_64__
      esp = regs.rsp;
      eip = regs.rip;
      eax = regs.rax;
      ebx = regs.rbx;
      ecx = regs.rcx;
      edx = regs.rdx;
      esi = regs.rsi;
      edi = regs.rdi;
    #else
      esp = regs.esp;
      eip = regs.eip;
      eax = regs.eax;
      ebx = regs.ebx;
      ecx = regs.ecx;
      edx = regs.edx;
      esi = regs.esi;
      edi = regs.edi;
    #endif
    if(ptrace(PTRACE_GETREGS, child_pid, nullptr, &regs) == -1) {
        std::cout << "Error: PTRACE_GETREGS" << std::endl;
        exit(1);
    };
    return regs;
}

void Breakpoint::enable_breakpoint(pid_t &child_pid, breakpoint_t &bp) {
    auto data = ptrace(PTRACE_PEEKDATA, child_pid, bp.m_bp_address, nullptr);
    bp.m_original_data = static_cast<uint8_t>(data & 0xff); //save bottom byte
    
    //set bottom byte to 0xcc
    uint64_t int3 = 0xcc;
    uint64_t data_with_int3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, child_pid, bp.m_bp_address, data_with_int3);

    bp.m_enabled = true;
}

void Breakpoint::disable_breakpoint(pid_t &child_pid, breakpoint_t &bp) {
    auto data = ptrace(PTRACE_PEEKDATA, child_pid, bp.m_bp_address, nullptr);

    //overwrite the low byte with the original data and write it back to memory.
    auto restored_data = ((data & ~0xff) | bp.m_original_data);
    ptrace(PTRACE_POKEDATA, child_pid, bp.m_bp_address, restored_data);

    bp.m_enabled = false;
}


