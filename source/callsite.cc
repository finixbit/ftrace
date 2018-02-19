#include "callsite.h"

std::unordered_map<std::intptr_t, Callsite> Callsite::m_callsites_map;


bool Callsite::get_callsite(std::intptr_t cs_address, Callsite &cs) {
  if(!Callsite::m_callsites_map.count(cs_address))
    return false;

  cs = Callsite::m_callsites_map[cs_address];
  return true;
}

intptr_t Callsite::string_to_intptr(std::string str) {
  return (intptr_t)std::stoi(str);
}