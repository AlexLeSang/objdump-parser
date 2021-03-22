#include <std.hpp>

#include <fmt.hpp>

#include <read_log_file.hpp>

using namespace std;

int main(int, char**)
{
  // read the file
  auto const filename = "/home/halushko/Projects/objdump-parser/build/log_translator.objdump";
  auto       lines    = input::read(filename);
  if (lines)
  {

    auto const& obj_lines = lines.value();
    fmt::print(fmt::format(FMT_STRING("There are {:d} lines in the input file {:s}\n"), obj_lines.size(), filename));

    auto match_line_regex = [&](auto const& l) {
      // ^(0{5,}[[:xdigit:]]*) <(.*)>:$
      const static auto function_re = regex{ "^(0{5,}[[:xdigit:]]*) <(.*)>:$", regex_constants::optimize };
      if (smatch matches; regex_search(l, matches, function_re))
      {
        auto const fun_address = matches[1].str();
        auto const fun_name    = matches[2].str();

        try
        {
          fmt::print(fmt::format("Found function {:s}\n", fun_name));
        } catch (fmt::format_error const& format_error)
        {
          fmt::print(stderr, FMT_STRING("Parse format error with: {:s} for line \"{:s}\": Address {:s} Function \"{:s}\" \n"), format_error.what(), l, fun_address, fun_name);
        }

        return true;
      }
      return false;
    };

    auto function_lines = obj_lines | std::ranges::views::filter(match_line_regex);
    for (auto const& a : function_lines)
    {
      fmt::print("Function\n");
    }
  }
  // find all functions
  // print all functions

  return 0;
}
