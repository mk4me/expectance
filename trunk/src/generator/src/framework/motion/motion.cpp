/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "motion.h"

using namespace ft;


/// \brief Constructor
Motion::Motion()
{
    setAnimName("");
    setAnimID(-1); 
}
/// \brief Constructor
/// \param const std::string animName - name of animation which is referenced by this object
/// \param int anim_id - id of animation which is referenced by this object
Motion::Motion(const std::string animName, int anim_id)
{
    setAnimName(animName);
    setAnimID(anim_id); 
    if (!initFootLimits(animName))
    {
	footLimits[0] = footLimits[2] = footLimits[1] = footLimits[3] = 1000.0f; 
    }
}

bool Motion::initFootLimits(const std::string animName)
{
	vector<string> _tokens;
	std::string _limitsStr;

	if ( Config::getInstance()->IsKey(animName.c_str()) )
    { // parse limits data for foot detection in that motion
		//read foot limits and dispatch each parameter
		_limitsStr = Config::getInstance()->GetStrVal(animName);
		_limitsStr = StringHelper::ClearDelimiters(_limitsStr, '(',')');
		_limitsStr = StringHelper::RemoveChar(_limitsStr,' ');
		_tokens = StringHelper::Split(_limitsStr, ",");
		if ((_tokens[0] == "<KEY_NOT_FOUND>")||(_tokens.size()!=4))
			return false;
		for (unsigned int i = 0; i <_tokens.size(); i++)
		{
			std::istringstream is(_tokens[i]);
			is >> footLimits[i];
			if( !is )
				return false;
		}
		return true;
	}
	return true;
}