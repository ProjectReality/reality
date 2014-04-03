/*************************************************************
*************Projet reality : fichier de log******************
*************Cree par Najeeth balasingam, login : balasi_a****
*************************************************************/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_


#define LOGGER_START(MIN_PRIORITY, FILE) Logger::Start(MIN_PRIORITY, FILE);
#define LOGGER_STOP() Logger::Stop();
#define LOGGER_WRITE(PRIORITY, MESSAGE) Logger::Write(PRIORITY, MESSAGE);


#include <string>
#include <fstream>

using namespace std;


class Logger
{
public:
	enum Priority
	{
		DEBUG,
		CONFIG,
		INFO,
		WARNING,
		ERROR
	};

	static void Start(Priority minPriority, const string& logFile);
	static void Stop();
	static void Write(Priority priority, const string& message);

private:
	Logger();
	Logger(const Logger& logger) {}
	Logger& operator = (const Logger& logger) {}

	bool        active;
	ofstream    fileStream;
	Priority    minPriority;

	static const string PRIORITY_NAMES[];
	static Logger instance;
};

const string Logger::PRIORITY_NAMES[] =
{
	"DEBUG",
	"CONFIG",
	"INFO",
	"WARNING",
	"ERROR"
};

#endif /*LOGGER_HPP_*/
