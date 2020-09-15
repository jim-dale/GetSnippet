#pragma once

class Helpers
{
public:
	static std::string GetFileNameFromUrl(const std::string& url)
	{
		std::string result;

		auto pos = url.find_last_of('/');
		if (pos != std::string::npos)
		{
			result = url.substr(pos + 1);
		}

		return result;
	}

	static std::string ReplaceAll(const std::string& str, const std::string& old_value, const std::string& new_value)
	{
		std::string result = str;

		size_t offset = old_value.length();
		size_t pos = 0;
		do
		{
			pos = str.find(old_value, pos);
			if (pos != std::string::npos)
			{
				result = result.replace(pos, old_value.length(), new_value);

				pos += offset;
			}
		} while (pos != std::string::npos);

		return result;
	}

	static int WriteAllBytes(const std::string& filename, const std::string& contents)
	{
		int result = 0;

		auto file = fopen(filename.c_str(), "wb");
		if (file == nullptr)
		{
			result = errno;
		}
		else
		{
			fwrite(contents.data(), 1, contents.length(), file);
			fclose(file);
		}

		return result;
	}

private:
	Helpers();
};
