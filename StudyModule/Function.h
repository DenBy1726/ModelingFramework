#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

namespace Function
{
	

	template <class T>
	std::wstring ToWString(T num){
		std::wstring a;
		std::wstringstream bufer;
		bufer.clear();
		bufer.sync();
		bufer << num;
		a = bufer.str();
		return a;
	}

	template <class T>
	std::string ToString(T num){
		std::string a;
		std::stringstream bufer;
		bufer.clear();
		bufer.sync();
		bufer << num;
		a = bufer.str();
		return a;
	}


	template <class T>
	T FromString(std::string data){
		T a;
		std::stringstream bufer(data);
		bufer.clear();
		bufer.sync();
		bufer >> a;
		return a;
	}



	template<class T = char>
	std::vector<std::string> Split(const std::string &text, T sep)
	{
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = text.find(sep, start)) != std::string::npos)
		{
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(text.substr(start));
		return tokens;
	}

	template<class T = char>
	std::vector<std::string> GetTokens(const std::string& text, T sep = ' ')
	{
		auto tokens = Split(text, sep);
		auto it = std::remove_if(tokens.begin(),
			tokens.end(), std::mem_fun_ref(&std::string::empty));
		// erase the removed elements
		tokens.erase(it, tokens.end());
		return tokens;
	}
}

#endif