#pragma once

#include <iostream>

#ifdef _LOG
#define debug(...) Logger::getLogger().log(__FILE__ , __LINE__, __VA_ARGS__)
#else
#define debug(...)
#endif


class Logger {
    bool m_printLineNo = false;
    bool m_printFileName = false;
    std::string m_prompt = ">> ";
public:


    template<typename T>
    void log(char const* filename, int lineNo, T x) {
        if(m_printFileName || m_printLineNo) std::cerr<<"[";
        if(m_printFileName) std::cerr<<filename;
        if(m_printFileName && m_printLineNo) std::cerr<<":";
        if(m_printLineNo) std::cerr<<lineNo;
        if(m_printFileName || m_printLineNo) std::cerr<<"] ";
        std::cerr<<m_prompt;

        std::cerr<<x<<std::endl;
    }


    static Logger getLogger() {
        static Logger logger;
        return logger;
    }
};