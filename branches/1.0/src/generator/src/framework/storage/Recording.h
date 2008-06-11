/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_STORAGE_RECORDING_H
#define _GEN_STORAGE_RECORDING_H

#include <string>
#include "../avatar/avatar.h"

namespace ft
{
    //TYPES of Recording
    static const int REC_TYPE_CAL3D = 0;
    static const int REC_TYPE_FT = 1;

	//STATES of Recording
	static const int REC_STATE_STARTED= 0;
	static const int REC_STATE_STOPPED= 0;

	/**
	 * Class PhysicsAvatar: this class is to handle physical aspects of avatar
     * 
     **/
    class Recording
    {
    public:
		Recording(const std::string& name, int type);
        Recording(const std::string& name, int type, const std::string& fileName);
        virtual ~Recording(void);
        virtual void Destroy(void);

		int getState() { return m_state; }

		void Start();
		void Stop();
		void Flush();
	                    
		void Clear();

		void AddAvatar(Avatar* av);
		void RemoveAvatar(Avatar* av);
		void RemoveAllAvatars();

		float getDuration() { return m_duration; }
		int getAvatarNumber() { return m_avatarNumber; }

	private:
		std::string m_name;
		std::string m_fileName;

		int m_type;
		int m_state;
		
		float m_duration;
		int m_avatarNumber;
		

    };
}

#endif //_GEN_STORAGE_RECORDING_H

