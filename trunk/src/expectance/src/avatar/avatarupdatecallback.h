/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _AVATAR_UPDATECALLBACK_H
#define _AVATAR_UPDATECALLBACK_H

#include <osg/NodeCallback>
#include <osg/Timer>
#include "osgCal/Model"
#include "evolution/avatar.h"

namespace ft { 
	class AvatarUpdateCallback: public osg::NodeCallback 
	{
		public:

			AvatarUpdateCallback(Avatar* avatar)
				: previous(0)
				, prevTime(0)
			{
				m_avatar = avatar;
			}

			virtual void operator()( osg::Node*        node,
									 osg::NodeVisitor* nv )
			{
				osgCal::Model* model = dynamic_cast< osgCal::Model* >( node );

				if ( previous == 0 )
				{
					previous = timer.tick();
				}
	            
				double deltaTime = 0;

				if (!nv->getFrameStamp())
				{
					osg::Timer_t current = timer.tick();
					deltaTime = timer.delta_s(previous, current);
					previous = current;
				}
				else
				{
					double time = nv->getFrameStamp()->getSimulationTime();
					deltaTime = time - prevTime;
					prevTime = time;
				}

				//std::cout << "CalUpdateCallback: " << deltaTime << std::endl;
				if ( deltaTime > 0.0 )
				{
					osgCal::ModelData* modelData = model->getModelDataForEvolution();

					OsgAvatar* avatarImpl = static_cast<OsgAvatar*>(m_avatar->getImplementation());
					CalMixer* calMixer = avatarImpl->getOsgModel()->getCalModel()->getMixer();
					// -- Update calMixer & skeleton --
//					if (calMixer->getAnimationActionList().size() > 0 
//						 || calMixer->getAnimationCycle().size() > 0 )
					{
						float elapsedTimeForAnims = deltaTime *  avatarImpl->getCurrSpeedFactor();

						calMixer->updateAnimation( elapsedTimeForAnims ); 
						calMixer->updateSkeleton();

						m_avatar->ManualUpdateAI(deltaTime); // abak: NOTE that this is deltaTime not elapsedTimeForAnims !!!
															 // it is because AI is not depentent on speed factor


						m_avatar->ManualUpdateMotions(elapsedTimeForAnims);

						m_avatar->ManualUpdateControllers(elapsedTimeForAnims);

						if ( modelData->update())
							model->updateMeshes();
					}
					//model->update( deltaTime );
				}
				//std::cout << "CalUpdateCallback: ok" << std::endl;

				traverse(node, nv); // may be needed for user nodes
	//            node->dirtyBound(); <- is it necessary?
			}

		private:

			osg::Timer timer;
			osg::Timer_t previous;
			double prevTime;
			Avatar* m_avatar;

	};
};


#endif //_AVATAR_UPDATECALLBACK_H
