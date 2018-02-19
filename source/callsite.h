#ifndef _H_CALLSITE_
#define _H_CALLSITE_

#include <vector> // vector
#include <unordered_map> // unordered_map


class Callsite {
    public:
        static bool get_callsite(std::intptr_t cs_address, Callsite &cs);
        static intptr_t string_to_intptr(std::string str);
        
        // cs_address: call target
        std::intptr_t m_cs_address = 0; 

        // cs_address: call target_address
        std::intptr_t m_cs_target_address = 0; 

        // resolved   (call symbol)
        // unresolved (call eax/%rbp+8)
        bool m_cs_target_resolved = false;

        // next instruction following cs_address
        std::intptr_t m_cs_return_address = 0; 

        // target string
        std::string m_cs_name;

        static std::unordered_map<std::intptr_t, Callsite> m_callsites_map;
};


#endif