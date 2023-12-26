#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;

// 字符串分割
vector<string> split(const string& str, const string& delimiter) {
	string::size_type lastPos = str.find_first_not_of(delimiter, 0);
	string::size_type pos = str.find_first_of(delimiter, lastPos);
	vector<string> tokens;
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiter, pos);
		pos = str.find_first_of(delimiter, lastPos);
	}
	return tokens;
}

template <class T>
int indexOf(const vector<T>& vec, const T& item) {
	const T* p = vec.data();
	for (int i = 0; i < vec.size(); i++) {
		if (*(p + i) == item) {
			return i;
		}
	}
	return -1;
}

#endif
