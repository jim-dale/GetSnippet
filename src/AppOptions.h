#pragma once

class AppOptions
{
public:
	std::string url;
	std::string ns;
	std::string filename;
	bool force;

	AppOptions()
	{
		force = false;
	}

	void Parse(int argc, char* argv[])
	{
		AppOptionsState state = AppOptionsState::ExpectOption;

		for (size_t i = 1; i < argc; i++)
		{
			auto arg = std::string(argv[i]);

			switch (state)
			{
			case AppOptionsState::ExpectOption:
				state = ParseOption(arg);
				break;
			case AppOptionsState::ExpectUrl:
				url = arg;
				filename = Helpers::GetFileNameFromUrl(url);
				state = AppOptionsState::ExpectOption;
				break;
			case AppOptionsState::ExpectNamespace:
				ns = arg;
				state = AppOptionsState::ExpectOption;
				break;
			default:
				break;
			}
		}
	}

	bool IsValid()
	{
		return (url.empty() == false && ns.empty() == false && filename.empty() == false);
	}

private:
	enum class AppOptionsState
	{
		ExpectOption,
		ExpectUrl,
		ExpectNamespace
	};

	AppOptionsState ParseOption(const std::string& arg)
	{
		auto result = AppOptionsState::ExpectOption;

		if (arg.length() == 2)
		{
			if (arg[0] == '-' || arg[0] == '/')
			{
				if (arg[1] == 'n' || arg[1] == 'N')
				{
					result = AppOptionsState::ExpectNamespace;
				}
				else if (arg[1] == 'u' || arg[1] == 'U')
				{
					result = AppOptionsState::ExpectUrl;
				}
				else if (arg[1] == 'f' || arg[1] == 'F')
				{
					force = true;
				}
			}
		}

		return result;
	}
};
