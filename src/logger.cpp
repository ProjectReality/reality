/*************************************************************
*************Projet reality : fichier de log******************
*************Cree par Najeeth balasingam, login : balasi_a****
*************************************************************/

#include <string>
#include <fstream>
#include <iostream>
#include "logger.h"

using namespace std;

Logger Logger::instance;

Logger::Logger() : active(false) {}

void Logger::Start(Priority minPriority, const string& logFile)
{
    instance.active = true;
    instance.minPriority = minPriority;
    if (logFile != "")
    {
        instance.fileStream.open(logFile.c_str());
    }
}

void Logger::Stop()
{
    instance.active = false;
    if (instance.fileStream.is_open())
    {
        instance.fileStream.close();
    }
}

void Logger::Write(Priority priority, const string& message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        ostream& stream
            = instance.fileStream.is_open() ? instance.fileStream : std::cout;

        stream  << PRIORITY_NAMES[priority]
                << ": "
                << message
                << endl;
    }
}
