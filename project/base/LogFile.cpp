#include<string>
#include<assert.h>
#include<time.h>
#include"LogFile.h"
#include"FileUtil.h"

using namespace std;

LogFile::LogFile(const string& basename,int flushEveryN)
:basename_(basename),flushEveryN_(flushEveryN),count_(0),mutex_(new MutexLock)
{
	file_.reset(new AppendFile(basename));       /*智能指针的  reset接口*/
}

LogFile::~LogFile(){}

void LogFile::append(const char* logline,int len){
	MutexLockGuard lock(*mutex_);
	append_unlocked(logline,len);
}


void LogFile::flush(){
	MutexLockGuard lock(*mutex_);
	file_->flush();
}

void LogFile::append_unlocked(const char* logline,int len){
	file_->append(logline,len);
	++count_;
	if(count_>=flushEveryN_){
		count_=0;
		file_->flush();
	}
}
