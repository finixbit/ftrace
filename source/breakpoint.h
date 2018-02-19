#ifndef _H_BREAKPOINT_
#define _H_BREAKPOINT_

#include <unordered_map> // unordered_map
#include <sys/user.h> /* user_regs_struct */
#include <capstone/capstone.h>


typedef struct {
    std::intptr_t m_bp_address = 0;
    uint8_t m_original_data = 0;
    bool m_enabled = false;

    bool m_is_symbol_address = false;

    bool m_is_cs_caller_address = false;
    std::intptr_t m_cs_caller_address = 0;

    bool m_is_cs_return_address = false;
    std::intptr_t m_cs_return_caller_address = 0; // m_cs_caller_address's address 
} breakpoint_t;

class Breakpoint {
    public:
        void set_symbol_breakpoint(
            pid_t &child_pid, std::intptr_t &sym_address);

        void set_callsite_breakpoint(
            pid_t &child_pid, 
            std::intptr_t &cs_address, std::intptr_t &cs_return_address);

        void continue_execution(pid_t &child_pid);
        void wait_for_signal(pid_t &child_pid);
        void step_over_breakpoint(pid_t &child_pid);

        uint64_t get_program_counter(pid_t &child_pid);
        void set_program_counter(pid_t &child_pid, uint64_t &pc);

        user_regs_struct get_registers(pid_t &child_pid);
        
        void enable_breakpoint(pid_t &child_pid, breakpoint_t &bp);
        void disable_breakpoint(pid_t &child_pid, breakpoint_t &bp);

        std::unordered_map<std::intptr_t, breakpoint_t> m_breakpoints_map;

    private:
        pid_t m_child_pid;
};


#endif