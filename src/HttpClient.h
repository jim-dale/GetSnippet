#pragma once

class HttpClient
{
public:
	HttpClient()
	{
		curl_result = CURLE_OK;
		curl = curl_easy_init();
	}
	~HttpClient()
	{
		curl_easy_cleanup(curl);
	}

	/*
	 * Download a file using HTTP GET and store in in a std::string
	 * @param url The URL to download.
	 * @return The string containing the downloaded data.
	*/
	std::string Download(const std::string& url)
	{
		std::string result;
		curl_result = CURLE_OK;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

		curl_result = curl_easy_perform(curl);

		return result;
	}

	bool IsSuccess()
	{
		return (curl_result == CURLE_OK);
	}

	std::string GetErrorString()
	{
		std::string result;
		if (IsSuccess() == false)
		{
			result = curl_easy_strerror(curl_result);
		}

		return result;
	}

private:
	CURL* curl;
	CURLcode curl_result;

	static size_t write_data(char* data, size_t size, size_t nmemb, std::string* buffer)
	{
		size_t result = 0;
		if (buffer != NULL)
		{
			buffer->append(data, size * nmemb);
			result = size * nmemb;
		}

		return result;
	}
};
