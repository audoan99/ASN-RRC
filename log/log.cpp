#include "log.hpp"
#include <thread>
#include <ctime>
#include <cstdarg>
#include <mutex>
#include <vector>
#include <sstream>
#include <fstream>
#include <dirent.h>


#define MAX_THREAD_NAME_LENGTH  15


typedef struct
{
    string sThreadName;
    thread::id thisID;
} thread_info_t;

typedef struct
{
    string sYear;
    string sMonth;
    string sDay;
    string sHour;
    string sMin;
    string sSec;
} date_time_t;


mutex m;
static string s_sFileName;
vector<thread_info_t> g_vThreadInfo;

// date       time         [ thread name/id ]   type                file:line
static char *convert(unsigned int num, int base);


string logInit(const string &file)
{
    time_t now = time(0);
    tm * pLocalTime = localtime(&now);

    int iYear = 1900 + pLocalTime->tm_year;
    int iMonth = 1 + pLocalTime->tm_mon;
    int iDay = pLocalTime->tm_mday;
    int iHour = pLocalTime->tm_hour;
    int iMinute = pLocalTime->tm_min;

    string sYear = to_string(iYear);
    string sMonth = (iMonth < 10)? ("0" + to_string(iMonth)) : (to_string(iMonth));
    string sDay = (iDay < 10)? ("0" + to_string(iDay)) : (to_string(iDay));
    string sHour = (iHour < 10)? ("0" + to_string(iHour)) : (to_string(iHour));
    string sMinute = (iMinute < 10)? ("0" + to_string(iMinute)) : (to_string(iMinute));

    s_sFileName = sYear + sMonth + sDay + "_" + sHour + "_" + sMinute + "_" + file;

    freopen( s_sFileName.c_str(), WRITE_MODE, stdout);

    cout << "DATA" << "       " << "TIME" << "      " << "[ THREAD NAME/ID ]" << " " << "TYPE" << "     " << "DESCRIPTION" << endl;
    return s_sFileName;
}

void logSetThreadName (const string &threadName)
{
    m.lock();
    thread_info_t thread;

    thread::id thisID = this_thread::get_id();
    thread.sThreadName = threadName;
    thread.thisID = thisID;
    g_vThreadInfo.push_back(thread);
    m.unlock();
}


void log(log_type_t type, const char * file, int line, const char * format, ...)
{
    m.lock();
    char * pDateTime;
    tm dateTime;
    date_time_t strDateTime;
    unsigned int unNumberThread = 0;
    int unLen;
    int unLenRemaining;
    string sSpace;
    const char * ch;
    int nNumber;
    char cChar;
    char * sStr;
    bool bIdFlag = true;
    auto thisID = this_thread::get_id();
    
    /* date time */
    time_t now = time(0);
    tm * pLocalTime = localtime(&now);

    dateTime.tm_year = pLocalTime->tm_year + 1900;
    dateTime.tm_mon = pLocalTime->tm_mon + 1;
    dateTime.tm_mday = pLocalTime->tm_mday;
    dateTime.tm_hour = pLocalTime->tm_hour;
    dateTime.tm_min = pLocalTime->tm_min;
    dateTime.tm_sec = pLocalTime->tm_sec;

    strDateTime.sYear = to_string(dateTime.tm_year);
    strDateTime.sMonth = (dateTime.tm_mon < 10)? ("0" + to_string(dateTime.tm_mon)) : to_string(dateTime.tm_mon);
    strDateTime.sDay = (dateTime.tm_mday < 10)? ("0" + to_string(dateTime.tm_mday)) : to_string(dateTime.tm_mday);
    strDateTime.sHour = (dateTime.tm_hour < 10)? ("0" + to_string(dateTime.tm_hour)) : to_string(dateTime.tm_hour);
    strDateTime.sMin = (dateTime.tm_min < 10)? ("0" + to_string(dateTime.tm_min)) : to_string(dateTime.tm_min);
    strDateTime.sSec = (dateTime.tm_sec < 10)? ("0" + to_string(dateTime.tm_sec)) : to_string(dateTime.tm_sec);
    cout << strDateTime.sDay << "-" << strDateTime.sMonth << "-" << strDateTime.sYear << " " << strDateTime.sHour << ":" << strDateTime.sMin << ":" << strDateTime.sSec << "  ";
    
    /* thread name/id */
    if(!g_vThreadInfo.empty())
    {
        unNumberThread = g_vThreadInfo.size();
        for (unsigned int i = 0; i < unNumberThread; i++)
        {
            if (g_vThreadInfo[i].thisID == thisID)
            {
                if (!g_vThreadInfo[i].sThreadName.empty())
                {
                    cout << "[ " <<  g_vThreadInfo[i].sThreadName;
                    unLen = g_vThreadInfo[i].sThreadName.length();
                    bIdFlag = false;
                    break;
                }
            }
        }
        if(bIdFlag)
        {
            stringstream ss;
            ss << thisID;
            string sThreadID = ss.str();
            cout << "[ " <<  thisID;
            unLen = sThreadID.length();
        }

        unLenRemaining = MAX_THREAD_NAME_LENGTH - unLen;
        if (unLenRemaining > 0)
        {
            for (unsigned int i = 0; i < unLenRemaining; i++)
            {
                sSpace += " ";
            }
            cout << sSpace << "]";
        }
        else
        {
            cout << " ]";
        }
    }

    /* type log */
    cout << " ";
    switch (type)
    {
    case INFO:
        cout << "INFO   ";
        break;
    case WARNING:
        cout << "WARNING";
        break;
    case ERROR:
        cout << "ERROR  ";
        break;
    default:
        cout << "       ";
        break;
    }
    cout << "  ";

    /* file:line */
    // cout << file << ":" << line << "     ";

    /* description */
    va_list arg; 
    va_start(arg, format);
    for (ch = format; *ch != '\0'; ch++)
    {
        if (*ch != '%')
        {
            cout << *ch;
        }
        else
        {
            ++ch;
            switch(*ch) 
            { 
                case 'c' : cChar = va_arg(arg, int);        //Fetch char argument
                        cout << cChar;
                        break;
                case 'd' : nNumber = va_arg(arg,int);         //Fetch Decimal/Integer argument
                        if(nNumber<0) 
                        { 
                            nNumber = -nNumber;
                            cout << '-'; 
                        }
                        cout << convert(nNumber,10);
                        break; 
                case 's': sStr = va_arg(arg,char *);         //Fetch string
                        cout << sStr; 
                        break;
                case 'x': nNumber = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                        cout << convert(nNumber,10);
                        break; 
            }
        }
    }
    va_end(arg);
    cout << endl;

    m.unlock();
}

static char *convert(unsigned int num, int base) 
{ 
    static char s_cRepresentation[]= "0123456789ABCDEF";
    static char s_cbuffer[50]; 
    char * pcPtr; 
    
    pcPtr = &s_cbuffer[49]; 
    *pcPtr = '\0'; 
    
    do 
    { 
        *--pcPtr = s_cRepresentation[num%base]; 
        num /= base; 
    } while(num != 0); 
    
    return(pcPtr); 
}
