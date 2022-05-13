#include "Terrapch.h"
#include "Exception.h"

const char* Terra::SBException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl << GetOriginString();
    m_WhatBuffer = oss.str();
    return m_WhatBuffer.c_str();
}

std::string Terra::SBException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File]" << m_File << std::endl
        << "[Line]" << m_Line;
    return oss.str();
}
