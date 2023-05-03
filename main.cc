#include <regex>
#include <unordered_map>
#include <iostream>
#include "replxx.hxx"
#include "thrid_party/clipp/include/clipp.h"

void setup_replxx(replxx::Replxx &rx);

const std::string kPrompt = "\x1b[1;32mmykv\x1b[0m> ";
// words to be completed
const std::vector<std::string> kCommands = {
    "help",
    "quit",
    "clear",
    "get ",
    "set ",
    "rm ",
};

const std::vector<std::string> kCommandsHint = {
    "help",
    "quit",
    "clear",
    "get <key>",
    "set <key> <value>",
    "rm <key>",
};

enum class CommandType : char
{
    UNKNOW,
    SET,
    GET,
    RM,
    QUIT,
    CLEAR,
    HELP,
};

using syntax_highlight_t =
    std::vector<std::pair<std::string, replxx::Replxx::Color>>;

const syntax_highlight_t kRegexColor{
    {"\".*?\"", replxx::Replxx::Color::BROWN},  // double quotes
    {"\'.*?\'", replxx::Replxx::Color::BROWN},  // single quotes
};


const std::unordered_map<std::string, CommandType> kCommandsMap = {
    {"set", CommandType::SET},
    {"get", CommandType::GET},
    {"rm", CommandType::RM},
    {"quit", CommandType::QUIT},
    {"clear", CommandType::CLEAR},
    {"help", CommandType::HELP},
};

int utf8str_codepoint_len(char const* s, int utf8len) {
  int codepointLen = 0;
  unsigned char m4 = 128 + 64 + 32 + 16;
  unsigned char m3 = 128 + 64 + 32;
  unsigned char m2 = 128 + 64;
  for (int i = 0; i < utf8len; ++i, ++codepointLen) {
    char c = s[i];
    if ((c & m4) == m4) {
      i += 3;
    } else if ((c & m3) == m3) {
      i += 2;
    } else if ((c & m2) == m2) {
      i += 1;
    }
  }
  return (codepointLen);
}


int main(int argc, char **argv)
{
    // parse command line
    std::string dbpath;
    bool check_crc = false;
    size_t dead_bytes_threshold = 128 * 1024 * 1024;

    auto cli = (clipp::value("db path", dbpath),
                clipp::option("-c", "--checksum")
                    .set(check_crc)
                    .doc("Enable CRC verification"),
                clipp::option("-d", "--dead_bytes_threshold")
                    .set(dead_bytes_threshold)
                    .doc("maximum single log file size"));
    if (!clipp::parse(argc, argv, cli))
    {
        std::cerr << clipp::make_man_page(cli, argv[0]);
        return 0;
    };

    // init the repl
    replxx::Replxx rx;
    setup_replxx(rx);

    std::cout << "Welcome to Mykv" << std::endl
              << "Press 'tab' to view autocompletions" << std::endl
              << "Type 'help' for help" << std::endl
              << "Type 'quit' to exit" << std::endl;

    return 0;
}

void setup_replxx(replxx::Replxx &rx)
{
    rx.set_completion_callback([&](std::string const &context, int &contextLen)
                               {
    replxx::Replxx::completions_t completions;
    if (contextLen == 0) {
      return completions;
    }
    for (auto const& c : kCommands) {
      if (c.rfind(context, 0) == 0) {
        completions.emplace_back(c.c_str());
      }
    }
    return completions; });

    rx.set_hint_callback([&](std::string const &context, int &contextLen,
                             replxx::Replxx::Color &color)
                         {
    replxx::Replxx::hints_t hints;
    if (contextLen == 0) {
      return hints;
    }
    for (auto const& c : kCommandsHint) {
      if (c.rfind(context, 0) == 0) {
        hints.emplace_back(c.c_str());
      }
    }
    return hints; });

    rx.set_highlighter_callback(
        [&](std::string const &context, replxx::Replxx::colors_t &colors)
        {
            for (auto const &e : kRegexColor)
            {
                size_t pos = 0;
                std::string str = context;
                std::smatch match;

                while (std::regex_search(str, match, std::regex(e.first)))
                {
                    std::string c = match[0];
                    std::string prefix = match.prefix().str();
                    pos += utf8str_codepoint_len(prefix.c_str(),
                                                 static_cast<int>(prefix.length()));
                    int len =
                        utf8str_codepoint_len(c.c_str(), static_cast<int>(c.length()));

                    for (int i = 0; i < len; ++i)
                    {
                        colors.at(pos + i) = replxx::Replxx::Color::YELLOW;
                    }

                    pos += len;
                    str = match.suffix();
                }
            }

            std::string command = context.substr(0, context.find(" "));
            auto it = kCommandsMap.find(command);
            if (it != kCommandsMap.end())
            {
                for (int i = 0; i < command.size(); ++i)
                {
                    colors.at(i) = replxx::Replxx::Color::YELLOW;
                }
            }
        });
}