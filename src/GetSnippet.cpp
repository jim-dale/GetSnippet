#include <io.h>

#include <string>

#include <curl/curl.h>
#include <curl/easy.h>

#include "Helpers.h"
#include "AppOptions.h"
#include "HttpClient.h"

int main(int argc, char* argv[])
{
	AppOptions options;

	options.Parse(argc, argv);

	if (options.IsValid() == false)
	{
		if (options.url.empty())
		{
			fprintf(stderr, "The snippet URL must be specified.\n");
		}
		else if (options.filename.empty())
		{
			fprintf(stderr, "Could not determine the file name from the specified URL.\n");
		}
		if (options.ns.empty())
		{
			fprintf(stderr, "The namespace for the snippet must be specified.\n");
		}

		printf("\n  -n <namespace>    Specify namespace for source file.\n");
		printf("  -u <url>          The URL of the snippet source file to download.\n");
	}
	else
	{
		std::string message;

		bool downloadFile = options.force;
		int res = _access(options.filename.c_str(), 0x00);
		if (res != 0)
		{	// The file doesn't exist
			downloadFile = true;
		}
		if (downloadFile)
		{
			WSADATA wsaData;
			(void)WSAStartup(MAKEWORD(2, 2), &wsaData);

			HttpClient client;

			auto contents = client.Download(options.url);
			if (client.IsSuccess())
			{
				contents = Helpers::ReplaceAll(contents, "__Snippets__", options.ns);

				res = Helpers::WriteAllBytes(options.filename, contents);
				if (res != 0)
				{
					message = strerror(res);
				}
			}
			else
			{
				message = client.GetErrorString();
			}
		}
		if (message.empty() == false)
		{
			fprintf(stderr, "%s\n", message.c_str());
		}
	}
}
