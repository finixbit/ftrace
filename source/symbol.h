#ifndef _H_SYM_
#define _H_SYM_

#include <unordered_map> // unordered_map


class Symbol {
    public:
        static bool get_symbol(std::intptr_t sym_address, Symbol &sym);

        std::intptr_t m_sym_address = 0; 

        std::string m_sym_name;

        static std::unordered_map<std::intptr_t, Symbol> m_symbols_map;
};




#endif