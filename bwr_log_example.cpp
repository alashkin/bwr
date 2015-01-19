// bwr_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "bwr_log.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_LOG_INIT(LR"(c:\temp\mylog.txt)", debug, true, true);
	
	_LOG(debug) << "Simple log record";

	_LOG_PREPARE_REC();
	//...
	_LOG_OPEN_REC(debug);
	_LOG_ADD_TO_REC() << "Record 1 ";
	_LOG_ADD_TO_REC() << "Record 2 ";
	_LOG_ADD_TO_REC() << "Record 3 ";
	_LOG_CLOSE_REC();

	return 0;
}

