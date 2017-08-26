#ifndef STRINGHELP_H_INCLUDED
#define STRINGHELP_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;

//namespace magic
namespace {

    void remove_right_left_spaces(string &s) {
        // remove_right_spaces
        int i = s.length() - 1;
        while(i != -1 && s[i] == ' ') --i;
        s = s.substr(0, i + 1);

        // remove_left_spaces
        i = 0;
        int len = s.length();
        while(i != len && s[i] == ' ') ++i;
        s = s.substr(i);
    }

    vector<string> split(const string &s, char delimiter, bool escape_if_empty) {
        vector<string> splitted_strings;
        stringstream ss(s);
        string token;
        while (getline(ss, token, delimiter)) {
            if(!escape_if_empty || token.length() != 0) {
                splitted_strings.push_back(token);
            }
        }
        return splitted_strings;
    }

    void replace_all(string &s, string from, char to) {
        for(char c : from) {
            replace(s.begin(), s.end(), c, to);
        }
    }

    void replace_string(std::string &subject, const std::string& search,
                              const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        subject = subject;
    }

    void remove_duplicate_consecutive(string &s, char c) {
        string res;
        res.push_back(s[0]);

        for(int i = 1, len = s.length(); i < len; ++i) {
            if(s[i] == s[i - 1] && s[i] == c) continue;
            res.push_back(s[i]);
        }

        s = res;
    }

}

#endif // STRINGHELP_H_INCLUDED
