#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

class FormatString {
public:
    FormatString(const std::string& format) : format_(format) {}

    template <typename... Args>
    std::string operator()(Args... args) const {
        std::vector<std::string> arg_strings = {to_string(args)...};
        size_t num_args = arg_strings.size();
        size_t pos = 0;
        std::stringstream result;
        while (pos < format_.length()) {
            size_t percent_pos = format_.find('%', pos);
            if (percent_pos == std::string::npos) {
                result << format_.substr(pos);
                break;
            }
            result << format_.substr(pos, percent_pos - pos);
            if (percent_pos + 1 < format_.length() && isdigit(format_[percent_pos + 1])) {
                size_t arg_index = format_[percent_pos + 1] - '0';
                if (arg_index >= 1 && arg_index <= num_args) {
                    result << arg_strings[arg_index - 1];
                } else {
                    throw std::runtime_error("Invalid argument index or insufficient number of arguments");
                }
                pos = percent_pos + 2;
            } else {
                throw std::runtime_error("Invalid format");
            }
        }
        return result.str();
    }

    size_t arg_number() const {
        size_t count = 0;
        size_t pos = 0;
        while ((pos = format_.find('%', pos)) != std::string::npos) {
            if (pos + 1 < format_.length() && isdigit(format_[pos + 1])) {
                count++;
            }
            pos++;
        }
        return count;
    }

private:
    std::string format_;

    template <typename T>
    std::string to_string(const T& arg) const {
        std::stringstream ss;
        ss << arg;
        return ss.str();
    }
};



int main() {
    FormatString format_str = FormatString("Hello, %2. I am %1! Buy, %2, %3");
    std::string str = format_str(3.14, 'F', 4);
    std::string another_str = format_str('Q', 333, 5);
    std::cout << str << std::endl;
    std::cout << another_str << std::endl;

    size_t num_args = format_str.arg_number();
    std::cout << "Number of arguments: " << num_args << std::endl;

    return 0;
}