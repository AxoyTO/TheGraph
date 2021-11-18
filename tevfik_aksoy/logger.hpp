#pragma once

#include <fstream>
#include <optional>
#include <string>

class Logger{
public:
    static Logger &get_instance(){
        static Logger instance;
        return instance;
    }

    void set_file(const std::optional<std::string> &filename);

    void log(const std::string &string);

    ~Logger(){
        if(file_stream_->is_open())
            file_stream_->close();
    }

private:
    std::optional<std::ofstream> file_stream_;

    Logger() = default;

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    Logger(Logger &&) = delete;

    Logger &operator=(Logger &&) = delete;
};
