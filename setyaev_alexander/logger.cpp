
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "config.hpp"
#include "logger.hpp"

namespace{

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
}

namespace uni_course_cpp{
	Logger::Logger() : file_(uni_course_cpp::config::kLogFilePath){
		if (!file_.is_open()){
			throw std::runtime_error("File is not open");
		}
	}
	Logger::~Logger(){
		file_.close();
	}
	void Logger::log(const std::string& string){
		file_ << get_current_date_time() << " " << string << "\n";
	}
}
