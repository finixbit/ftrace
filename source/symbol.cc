#include "symbol.h"

std::unordered_map<std::intptr_t, Symbol> Symbol::m_symbols_map;


bool Symbol::get_symbol(std::intptr_t sym_address, Symbol &sym) {
  if(!Symbol::m_symbols_map.count(sym_address))
    return false;

  sym = Symbol::m_symbols_map[sym_address];
  return true;
}