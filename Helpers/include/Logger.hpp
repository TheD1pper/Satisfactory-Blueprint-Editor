#include <string>
#include <filesystem>
#include <fstream>

#pragma comment(lib, "Logger.lib")

namespace Log
{
	enum class LogLevels
	{
		Debug,
		Info,
		Warning,
		Error,
		CriticalError
	};



	class Logger
	{
	private:
		std::ofstream m_LogFile;
		Log::LogLevels m_MinConsoleLevel = LogLevels::Debug;
		Log::LogLevels m_MinFileLevel = LogLevels::Debug;
		bool m_IsReadyToFileLog = false;

		char m_TimeOpenBracket = '|';
		char m_TimeCloseBracket = '|';

		char m_LevelOpenBracket = '[';
		char m_LevelCloseBracket = ']';

	public:
		void Console(Log::LogLevels _Level, const std::string& _Message);
		void File(Log::LogLevels _Level, const std::string& _Message);
		void Both(Log::LogLevels _Level, const std::string& _Message);

	public:
		void SetLogFile(const std::filesystem::path& _Path);
		void CloseLogFile();

	public:
		void SetTimeBrackets(const char& _OpenBracket, const char& _CloseBracket);
		void SetLevelBrackets(const char& _OpenBracket, const char& _CloseBracket);

	public:
		std::string LevelToString(Log::LogLevels _Level);
		std::string LevelToColor(Log::LogLevels _Level);
		void SetMinConsoleLevel(Log::LogLevels _Level);
		void SetMinFileLevel(Log::LogLevels _Level);
		void SetBothLevel(Log::LogLevels _Level);

	public:
		~Logger()
		{
			CloseLogFile();
		}
	};
}