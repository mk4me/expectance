/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "animation.h"
#include "../utility/stringhelper.h"

using namespace ft;


/// \brief Constructor
Animation::Animation(CalCoreAnimation* anim)
{
	m_coreAnim = anim;
    setAnimName("");
    setAnimID(-1); 
}
/// \brief Constructor
/// \param const std::string animName - name of animation which is referenced by this object
/// \param int anim_id - id of animation which is referenced by this object
Animation::Animation(CalCoreAnimation* anim, const std::string animName, int anim_id)
{
	m_coreAnim = anim;
    setAnimName(animName);
    setAnimID(anim_id); 
	
	//TODO:  recover this
//    if (!initFootLimits(animName))
//    {
//	footLimits[0] = footLimits[2] = footLimits[1] = footLimits[3] = 1000.0f; 
//    }

	//TODO:  recover this
//    InitSpeedFactorLimits(animName);
}

//TODO:  recover this
/*bool Animation::initFootLimits(const std::string animName)
{
	vector<string> _tokens;
	std::string _limitsStr;

	if ( Config::getInstance()->IsKey(animName.c_str()) )
    { // parse limits data for foot detection in that motion
		//read foot limits and dispatch each parameter
		_limitsStr = Config::getInstance()->GetStrVal(animName);

        if (StringHelper::ReadFloatArrayFromString(_limitsStr, footLimits,4))
        {
    		return true;
        }
	}
	return false;
}
*/

/**
 * \brief Inits limits for speed factor
 **/
//TODO:  recover this
/*
void Animation::InitSpeedFactorLimits(const std::string animName)
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
*/

float Animation::getDuration() 
{
	float result = 0;
    if (m_coreAnim != NULL)
    {
        result = m_coreAnim->getDuration();
    }
	else
	{
		//TODO: add ERROR
	}
	return result;
}
