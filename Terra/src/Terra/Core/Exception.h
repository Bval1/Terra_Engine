#pragma once
#include <exception>
#include <string>
namespace Terra
{
	class SBException : public std::exception
	{
	public:
		SBException(int line, const char* file) noexcept
			: m_Line(line), m_File(file) {}

		const char* what() const noexcept override;
		std::string GetOriginString() const noexcept;

		virtual const char* GetType() const noexcept { return "Terra Exception"; }
		int GetLine() const noexcept { return m_Line; }
		const std::string& GetFile() const noexcept { return m_File; }
	private:
		int m_Line;
		std::string m_File;
	protected:
		mutable std::string m_WhatBuffer;
	};
}