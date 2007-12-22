/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "motion.h"
#include "utility/stringhelper.h"

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

    InitSpeedFactorLimits(animName);
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

/**
 * \brief Inits limits for speed factor
 **/
void Motion::InitSpeedFactorLimits(const std::string animName)
{
    string limitKeyStr = "speed_limits_" + animName;
    bool limitsDefined = false;    
	if ( Config::getInstance()->IsKey(limitKeyStr.c_str()) )
    { 
        float limits[2];
		
        std::string limitsStr = Config::getInstance()->GetStrVal(limitKeyStr);
        if (StringHelper::ReadFloatArrayFromString(limitsStr, limits,2))
        {
            setMinSpeedfactor(limits[0]);
            setMaxSpeedfactor(limits[1]);
            limitsDefined = true;
        }
    }

    if (!limitsDefined)
    {
        //sets dafault values
        float minSpeed = 0.5f;
        float maxSpeed = 1.5f;
        if (Config::getInstance()->IsKey("default_min_speed_factor"))
        {
            minSpeed = Config::getInstance()->GetFloatVal("default_min_speed_factor");
        }
        setMinSpeedfactor(minSpeed);

        if (Config::getInstance()->IsKey("default_max_speed_factor"))
        {
            maxSpeed = Config::getInstance()->GetFloatVal("default_max_speed_factor");
        }
        setMaxSpeedfactor(maxSpeed);
    }
}