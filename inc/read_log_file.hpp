#ifndef READ_LOG_FILE_H
#define READ_LOG_FILE_H

#include <std.hpp>

#include <boost.hpp>

namespace input {

using namespace std;

inline auto read(filesystem::path const& path) noexcept
{
  using Lines = vector<string>;

  if (filesystem::exists(path))
  {
    try
    {
      boost::iostreams::mapped_file const mmap(path, boost::iostreams::mapped_file::readonly);
      auto                                data_pointer = mmap.const_data();
      const auto                          data_end     = data_pointer + mmap.size();

      Lines lines;


      while (data_pointer && data_pointer != data_end)
      {
        auto const new_line_location = memchr(data_pointer, '\n', data_end - data_pointer);
        auto const old_data_pointer  = data_pointer;
        if ((data_pointer = static_cast<char const*>(new_line_location)))
        {
          auto const str_len = distance(old_data_pointer, data_pointer);
          lines.emplace_back(old_data_pointer, str_len);

          data_pointer++;
        }
      }
      lines.shrink_to_fit();

      return make_optional(lines);
    } catch (std::ios_base::failure const& e)
    {}
  }

  return optional<Lines>();
}

using LogFileLinesOpt = invoke_result<decltype(read), filesystem::path>::type;
}

#endif /* READ_LOG_FILE_H */
