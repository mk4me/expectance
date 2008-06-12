/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "Recording.h"

using namespace ft;
using namespace std;



Recording::Recording(const std::string& name, int type)
{
	m_name = name;
	m_type = type;
}

Recording::Recording(const std::string& name, int type, const std::string& fileName)
{
	m_name = name;
	m_type = type;
	m_fileName = fileName;
}

Recording::~Recording(void)
{
}

void Recording::Destroy(void)
{
}

void Recording::Start()
{
}

void Recording::Stop()
{
}

void Recording::Flush()
{
}
	                    
void Recording::Clear()
{
}

void Recording::AddAvatar(CalAvatar* av)
{
}

void Recording::RemoveAvatar(CalAvatar* av)
{
}

void Recording::RemoveAllAvatars()
{
}

