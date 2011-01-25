/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak,mka
 */

#include "osgmain.h"

#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include <osg/MatrixTransform>
#include <osg/Light>
#include <osg/ShadeModel>
#include <osg/LightModel>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgCal/CoreModel>
#include <osgCal/Model>

#include "evolution/avatar.h"
#include "evolution/factory.h"
#include "../evolution_impl/cal3dimpl.h"
#include "../evolution_impl/cal3danimprovider.h"
#include "evolution/world.h"
#include "evolution/dataprovider.h"
#include "../avatar/osgavatar.h"
#include "evolution/evodbg.h"
#include "../timeline/lcsmodifier.h"
#include "../timeline/speedcontroller.h"
#include "../control/controlmanager.h"
#include "../ai/goals/randommovegoal.h"
#include "../ai/goals/changedirgoal.h"
#include "../ai/goals/limitedareagoal.h"
#include "../ai/constraints/limitedareaconstraint.h"
#include "config.h"
#include "../utility/stringhelper.h"
#include "global.h"
// to fix #include "../data/xmlsceneparser.h"
#include"../avatar/osgavatarfactory.h"

#include "../scene/view/follownodemanip.h"
#include "../scene/object/traceline.h"
#include "../scene/object/othershapes.h"
#include "../scene/worldmanager.h"

// #include "../data/datacollector.h"


using namespace ft;


bool ChangeCamera = false;
bool AllowRefreshCamera = false;
bool SelectActiveAvatar = false;
// DataCollector* DCOLL;

template < typename T >
T normalize( const T& v )
{
    T r( v );
    r.normalize();
    return r;
}

/**
 * Add window in multi-window setup.
 */
void  addWindow( osgViewer::Viewer& viewer, int x, int y, int width, int height,
           float xTranslate,  float yTranslate )
{
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = x;
    traits->y = y;
    traits->width = width;
    traits->height = height;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext(gc.get());
    camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
    GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
    camera->setDrawBuffer(buffer);
    camera->setReadBuffer(buffer);

    // add this slave camera to the viewer, with a shift left of the projection matrix
    viewer.addSlave(camera.get(), osg::Matrixd::translate( xTranslate, yTranslate, 0.0),
                    osg::Matrixd());
}


class ToggleHandler : public osgGA::GUIEventHandler 
{
    public: 

        ToggleHandler( bool& toggleVar,
                       char  key,
                       const std::string& help, Avatar* activeAvatar )
            : toggleVar( &toggleVar )
            , key( key )
            , help( help )
			, m_activeAvatar( ControlManager::getInstance()->getActiveAvatar() )
        {
        }
        
        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
        {
            osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
            if (!viewer) return false;
    
            switch(ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::KEYDOWN):
                {
                    if ( ea.getKey() == key )
                    {
                        *toggleVar = !(*toggleVar);

					} else if ( ea.getKey() == ea.KEY_Up ) {

						if (m_activeAvatar != NULL)
						{
							if (m_activeAvatar->getCurrTopMotion() != NULL)
							{
								if (m_activeAvatar->getCurrTopMotion()->getName().compare("walk") == 0)
									m_activeAvatar->ExecuteAction("run");
								else if (m_activeAvatar->getCurrTopMotion()->getName().compare("idle") == 0)
									m_activeAvatar->ExecuteAction("walk");
							}
							else
							{
								m_activeAvatar->ExecuteAction("idle");
							}
						}

					} else if ( ea.getKey() == ea.KEY_Down ) {
						if (m_activeAvatar != NULL)
						{
							if (m_activeAvatar->getCurrTopMotion() != NULL)
							{
								if (m_activeAvatar->getCurrTopMotion()->getName().compare("walk") == 0)
								{
									//mka 2008.08.30 send signal to get stop controller possibility to go on
									OsgAvatar* avImpl = static_cast<OsgAvatar*>(m_activeAvatar->getImplementation());					
									avImpl->getStopController()->setStop(true);
									//m_activeAvatar->ExecuteAction("idle");
								}
								else if (m_activeAvatar->getCurrTopMotion()->getName().compare("run") == 0)
									m_activeAvatar->ExecuteAction("walk");
							}
							else
							{
								m_activeAvatar->ExecuteAction("idle");
							}
						}

					} else if ( ea.getKey() == ea.KEY_Left ) {
						if (m_activeAvatar != NULL)
						{
							OsgAvatar* avImpl = (OsgAvatar*)m_activeAvatar->getImplementation();
							osg::Quat currRot = avImpl->getGlobalRotation();
							currRot *= osg::Quat(0.1f,osg::Vec3d(0,0,1));
							avImpl->setGlobalRotation(currRot);
						}

					}  else if ( ea.getKey() == ea.KEY_Right ) {
						if (m_activeAvatar != NULL)
						{
							OsgAvatar* avImpl = (OsgAvatar*)m_activeAvatar->getImplementation();
							osg::Quat currRot = avImpl->getGlobalRotation();
							currRot *= osg::Quat(-0.1f,osg::Vec3d(0,0,1));
							avImpl->setGlobalRotation(currRot);
						}
					} else if ( ea.getKey() == ea.KEY_F1 ) {
						int _activeAvatarIndex = ControlManager::getInstance()->getActiveAvatarInd();
						if ( _activeAvatarIndex >= 0)
						{
							_activeAvatarIndex = (_activeAvatarIndex+1) % ControlManager::getInstance()->getAvatarsCount();
							ControlManager::getInstance()->setActiveAvatar(_activeAvatarIndex);
							m_activeAvatar = ControlManager::getInstance()->getActiveAvatar();
							//m_activeAvatar->SetThink(false);
							SelectActiveAvatar = true; // allow change avatar camera and tracker when active avatar changed
							
						}
					} else if ( ea.getKey() == ea.KEY_F10 ) { // camera toggle view
						 ChangeCamera^=true;
						 AllowRefreshCamera = true;		
					//} else if ( ea.getKey() == ea.KEY_F11 ) { // refresh avatars parameters
						 //if (DCOLL != NULL)
						 //{
						 //	 DCOLL->Refresh();
						 //}
					}  else if ( ea.getKey() == ea.KEY_F3 ) {
						if (m_activeAvatar != NULL)
						{
							OsgAvatar* avImpl = (OsgAvatar*)m_activeAvatar->getImplementation();
							float newSpeedFactor = avImpl->getDestSpeedFactor() * 1.1f;
							newSpeedFactor = newSpeedFactor > avImpl->getSpeedFactorMax() ? avImpl->getSpeedFactorMax() :  newSpeedFactor;
							avImpl->setDestSpeedFactor(newSpeedFactor);
//							std::cout << " new speed factor " << newSpeedFactor << " curr " << avImpl->getCurrSpeedFactor() << std::endl;
						}
					}  else if ( ea.getKey() == ea.KEY_F4 ) {
						if (m_activeAvatar != NULL)
						{
							OsgAvatar* avImpl = (OsgAvatar*)m_activeAvatar->getImplementation();
				            float newSpeedFactor = avImpl->getDestSpeedFactor() * 0.9f;
				            newSpeedFactor = newSpeedFactor < avImpl->getSpeedFactorMin() ? avImpl->getSpeedFactorMin() :  newSpeedFactor;
							avImpl->setDestSpeedFactor(newSpeedFactor);
//							std::cout << " new speed factor " << newSpeedFactor << " curr " << avImpl->getCurrSpeedFactor() << std::endl;
						}
					}
                }
                default: break;
            }
        
            return false;
        }
    
        /** Get the keyboard and mouse usage of this manipulator.*/
        virtual void getUsage(osg::ApplicationUsage& usage) const
        {
            usage.addKeyboardMouseBinding( std::string( 1, key ), help );
        }

    private:

        bool*       toggleVar;
        char        key;
        std::string help;
		Avatar* m_activeAvatar;
};


class CompileStateSets : public osg::Operation
{
    public:
        CompileStateSets( osg::Node* node )
            : osg::Operation( "CompileStateSets", false )
            , node( node )
        {}
        
        virtual void operator () ( osg::Object* object )
        {
            osg::GraphicsContext* context = dynamic_cast< osg::GraphicsContext* >( object );
            
            if ( context )
            {
                osg::ref_ptr< osgUtil::GLObjectsVisitor > glov = new osgUtil::GLObjectsVisitor;
                glov->setState( context->getState() );
                node->accept( *(glov.get()) );
            }
        }

    private:
        
        osg::Node* node;
};

// -- Expectance initializations --
void InitActionsForType(World* world, const std::string& avatarType)
{
	//---------------- IDLE
	Action* action = new Action(avatarType, "idle");

	Motion* idle = DataProvider::getInstance()->getMotion(avatarType, "idle");
	action->setMotion(idle);
	world->AddAction(avatarType, action);


	//---------------- WALK
	action = new Action(avatarType, "walk");
	Motion* walk = DataProvider::getInstance()->getMotion(avatarType, "walk");
	action->setMotion(walk);
	world->AddAction(avatarType, action);

	//---------------- RUN
	action = new Action(avatarType, "run");
    Motion* run = DataProvider::getInstance()->getMotion(avatarType, "run");
	action->setMotion(run);
	world->AddAction(avatarType, action);
}

void InitGraphForType(World* world, const std::string& avatarType)
{
	Transition* transition = new Transition("idle","walk");
	Motion* walkstart = DataProvider::getInstance()->getMotion(avatarType, "walkstart");
	transition->setMotion(walkstart);
	world->AddTransition(avatarType, transition);


	transition = new Transition("walk","idle");
	Motion* walkstop = DataProvider::getInstance()->getMotion(avatarType, "walkstop");    
	transition->setMotion(walkstop);
	world->AddTransition(avatarType, transition);

	transition = new Transition("walk","run");
	Motion* runstart = DataProvider::getInstance()->getMotion(avatarType, "runstart");    
    transition->setMotion(runstart);
	world->AddTransition(avatarType, transition);

	transition = new Transition("run", "walk");
	Motion* runstop = DataProvider::getInstance()->getMotion(avatarType, "runstop");    
    transition->setMotion(runstop);
	world->AddTransition(avatarType, transition);
}

void InitWorld(World* world)
{
	Goal* goal = new RandomMoveGoal();
	world->AddGoal(goal);

	goal = new ChangeDirGoal();
	world->AddGoal(goal);

	Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
	world->AddRule(rule);

	//world->LoadDataForType("data/motions.xml", "cally");

	std::string _models_str = "";
	if (Config::getInstance()->IsKey("avatar_models"))
	{
		_models_str = Config::getInstance()->GetStrVal("avatar_models");
	}
	vector<std::string>  models = StringHelper::Split(_models_str,",");

	//abak: NOTE that is only one motions file for all avatar types. It can be extended in application.cfg to handle separate
	// motions file for each avatar type.
	std::string motions_file = "";
	if (Config::getInstance()->IsKey("motions_file"))
	{
		motions_file = Config::getInstance()->GetStrVal("motions_file");
	}

	for (unsigned int i=0; i < models.size(); i++)
	{
		world->LoadDataForType(FT_DATA_PATH + motions_file, OsgAvatarFactory::getAvatarPath(models[0]));
		InitActionsForType(world, OsgAvatarFactory::getAvatarPath(models[0]));
		InitGraphForType(world, OsgAvatarFactory::getAvatarPath(models[0]));
	}
}




void InitSceneFromFile(World* world, osg::MatrixTransform* worldTransformNode)
{
	std::string scene_file = "";
	if (Config::getInstance()->IsKey("scene_file"))
	{
		scene_file = Config::getInstance()->GetStrVal("scene_file");

//to fix under linux
//		XMLSceneParser parser;
//		parser.LoadScene(FT_DATA_PATH + scene_file, world, worldTransformNode);
	}
}

void InitSceneFromCode(World* world, osg::MatrixTransform* worldTransformNode)
{
	// floor

	osg::Vec3 center(0.0f,0.0f,0.0f);
    float radius = 2000.0f;
    osg::Node* floorModel = createFloor(center,radius);
	worldTransformNode->addChild(floorModel);


    int avatar_number = -1;
    if (Config::getInstance()->IsKey("avatars_number"))
    {
        avatar_number = Config::getInstance()->GetIntVal("avatars_number");
    }
    else
    {
        avatar_number = 1;
    }

    Avatar* av;
	 
	osg::Vec3d vStartPos(0,0,0);
	osg::Quat  vStartAttitude;
    srand(time(NULL));

	float x_off = -150, z_off = 0;
	
	std::string _nameHelper;
    for (int i=0; i<avatar_number; i++)
    {
		_nameHelper.empty();
		_nameHelper = "Avatar" + ft::StringHelper::itos(i);

//		if (i%2==0)
			
			std::string modelPath = OsgAvatarFactory::getAvatarPath("freebie");
//	    else
//			modelName = "../../data/models/freebie/cally.cfg";

		av = dynamic_cast<Avatar*>(ft::Factory::getInstance()->createAvatar(_nameHelper, modelPath)); 
        if (av != NULL)  
        {
            //av->Init();
            //av->Dump();
			OsgAvatar* avImpl = static_cast<OsgAvatar*>(av->getImplementation());
			avImpl->getOffsetTransform()->setName(_nameHelper);
			// set random position and direction
			bool isInScope = true;
			int scopeRadius = (int)radius; //2000;
			vStartAttitude = osg::Quat(osg::DegreesToRadians((float)(rand()%360)), osg::Vec3(0,0,1));
			while (isInScope)
			{
				int sign=((rand()%2)==1)?-1:1;
				vStartPos.x() =  (rand() % scopeRadius-300)*sign;
				vStartPos.y() =  (rand() % scopeRadius-300)*sign;

				// check collision by radius distance
				int children = worldTransformNode->getNumChildren();
				int distCnt = 0;
				for (int j = 0; j < children; j++)
				{
					osg::Node * avTmp =  worldTransformNode->getChild(j);
					osg::Vec3d _center = avTmp->getBound().center();
					double radius1 = 300; //avTmp->getBound().radius();
					double distance = sqrt( (vStartPos.x()-center.x())*(vStartPos.x()-center.x())
						+(vStartPos.y()-center.y())*(vStartPos.y()-center.y()) );
					if ((distance <= radius1*3) && (children > 1))
						distCnt++;
				}
				if(distCnt == 0) isInScope = false;
				else isInScope = true;
			//	vStartPos.set(vStartPos.x() + x_off, vStartPos.y(), vStartPos.z() + z_off);
			}

			ControlManager::getInstance()->AddAvatar(av);
			av->AddController(new LCSModifier());
			SpeedController* speedCtl = new SpeedController();
			av->AddController(speedCtl);
			av->AddController(avImpl->getFootDetector());
			av->AddController(avImpl->getStopController()); //mka 2008.08.19
			world->AddAvatar(av);
			av->StartSimulation();

			OsgAvatarFactory::AddAvatarToScene(av, worldTransformNode, vStartPos, vStartAttitude);
        }
    }
}




EXPECTANCE_API int RunOSGApp(int argc, char *argv[])
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc, argv);

    arguments.getApplicationUsage()->setApplicationName("osgCalViewer");
    arguments.getApplicationUsage()->setDescription("osgCalViewer");
    arguments.getApplicationUsage()->setCommandLineUsage("osgCalViewer [options] cal3d.cfg ...");
    arguments.getApplicationUsage()->addCommandLineOption("--sw", "Use software skinning and fixed-function drawing");
    arguments.getApplicationUsage()->addCommandLineOption("--hw", "Use hardware (GLSL) skinning and drawing");
    arguments.getApplicationUsage()->addCommandLineOption("--df", "Use depth first meshes (improve performance when pixel shading is a bottleneck)");
    arguments.getApplicationUsage()->addCommandLineOption("--no-debug", "Don't display debug information");
    arguments.getApplicationUsage()->addCommandLineOption("--four-window", "Run viewer in four window setup (to test multi-context applications)");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display command line parameters");
    arguments.getApplicationUsage()->addCommandLineOption("--help-env","Display environmental variables available");
    arguments.getApplicationUsage()->addCommandLineOption("--help-all","Display all command line, env vars and keyboard & mouse bindings.");
    arguments.getApplicationUsage()->addCommandLineOption("--SingleThreaded","Select SingleThreaded threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--CullDrawThreadPerContext","Select CullDrawThreadPerContext threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--DrawThreadPerContext","Select DrawThreadPerContext threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--CullThreadPerCameraDrawThreadPerContext","Select CullThreadPerCameraDrawThreadPerContext threading model for viewer.");

    // if user request help write it out to cout.
    bool helpAll = arguments.read("--help-all");
    unsigned int helpType =
        ((helpAll || arguments.read("-h") || arguments.read("--help"))
         ? osg::ApplicationUsage::COMMAND_LINE_OPTION : 0 ) |
        ((helpAll || arguments.read("--help-env"))
         ? osg::ApplicationUsage::ENVIRONMENTAL_VARIABLE : 0 );
    
    if ( helpType )
    {
        arguments.getApplicationUsage()->write( std::cout, helpType );
        return 1;
    }

    // report any errors if they have occurred when parsing the program arguments.
    if ( arguments.errors() )
    {
        arguments.writeErrorMessages( std::cout );
        return 1;
    }
    
    std::string fn;

    // note currently doesn't delete the loaded file entries from the command line yet...
    for(int pos=1;pos<arguments.argc();++pos)
    {
        if (!arguments.isOption(pos))
        {
            fn = arguments[pos];
        }
    }

//    if ( arguments.argc() <= 1 || fn == "" )
//    {
//        arguments.getApplicationUsage()->write( std::cout,
//                                                osg::ApplicationUsage::COMMAND_LINE_OPTION );
//        return 1;
//    }

    if ( arguments.read( "--no-debug" ) == false )
    {
        osg::setNotifyLevel( osg::DEBUG_FP );
    }

	// Loads configuration from 'application.cfg'
    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();
	


	osg::ref_ptr< osg::Group > root = new osg::Group();
	
	//------------ EVOLUTION init

	Cal3dImpl m_evolutionImpl;
	Cal3dAnimProvider m_animProvider;
	World* m_world;

	m_evolutionImpl.Init();
	m_animProvider.Init();
	m_world = ft::Factory::getInstance()->CreateWorld();
	//EvoDBG::setTimelineLevel(1);
	ft::ControlManager::getInstance()->Init(); //enforced creation of singleton

	osg::ref_ptr<osg::MatrixTransform> worldTransformNode = new osg::MatrixTransform;
	worldTransformNode->setName("worldTransformNode");
	worldTransformNode->setMatrix(osg::Matrix::rotate(osg::inDegrees(5.0f),1.0f,0.0f,0.0f));
	WorldManager::getInstance()->Init(worldTransformNode.get()); //Initialize osg nodes collector
	root->addChild(WorldManager::getInstance()->getWorldNode());


	InitSceneFromFile(m_world, WorldManager::getInstance()->getWorldNode());
	InitSceneFromCode(m_world, WorldManager::getInstance()->getWorldNode());

	ControlManager::getInstance()->setActiveAvatar(0);
	



//	analyse(worldTransformNode.get());

	InitWorld(m_world);
	m_world->DumpActions();
	m_world->DumpAvatars();
	m_world->SetActionForAvatars("idle");

	m_world->StartThinking();

	// read actual configuration and dump it
	///DCOLL = new DataCollector();
	///DCOLL->Init();

// -- Setup viewer --
    osgViewer::Viewer viewer;

    if ( arguments.read( "--four-window" ) )
    {
        addWindow( viewer,   0,   0, 640, 480,  1.0, -1.0 );
        addWindow( viewer, 640,   0, 640, 480, -1.0, -1.0 );
        addWindow( viewer,   0, 480, 640, 480,  1.0,  1.0 );
        addWindow( viewer, 640, 480, 640, 480, -1.0,  1.0 );
    }
	//viewer.setUpViewInWindow(0,0,640,480);

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator( viewer.getCamera()->getOrCreateStateSet() ) );
    
    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the full screen toggle handler
    viewer.addEventHandler( new osgViewer::WindowSizeHandler );

    // add the stats handler
    viewer.addEventHandler( new osgViewer::StatsHandler );

    // add the help handler
    viewer.addEventHandler(new osgViewer::HelpHandler( arguments.getApplicationUsage() ) );

    // add the animation toggle handler
    //viewer.addEventHandler( new AnimationToggleHandler( (osgCal::Model*)root->getChild(0), animationNames ) );
    
    // add the pause handler and avatar controlling
    bool paused = false;
    viewer.addEventHandler( new ToggleHandler( paused, 'p', "Pause animation", ControlManager::getInstance()->getActiveAvatar() ) );

    while (arguments.read("--SingleThreaded")) viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    while (arguments.read("--CullDrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::CullDrawThreadPerContext);
    while (arguments.read("--DrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::DrawThreadPerContext);
    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::CullThreadPerCameraDrawThreadPerContext);


// light is already smooth by default
    osg::ShadeModel* shadeModel = new osg::ShadeModel;
    shadeModel->setMode( osg::ShadeModel::SMOOTH );
    root->getOrCreateStateSet()->setAttributeAndModes( shadeModel, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

     osg::LightModel* lightModel = new osg::LightModel();
//    lightModel->setColorControl( osg::LightModel::SINGLE_COLOR );
//    lightModel->setColorControl( osg::LightModel::SEPARATE_SPECULAR_COLOR );
    lightModel->setAmbientIntensity( osg::Vec4(0.1,0.1,0.1,1) );
    root->getOrCreateStateSet()->setAttributeAndModes( lightModel, osg::StateAttribute::ON );

    // -- light #0 --
    osg::Light* light0 = new osg::Light();
    light0->setLightNum( 0 );
    light0->setAmbient( osg::Vec4( 0, 0, 0, 1 ) );
    light0->setDiffuse( osg::Vec4( 0.8, 0.8, 0.8, 1 ) );
    light0->setSpecular( osg::Vec4( 1, 1, 1, 0 ) );
    // as in SceneView, except direction circa as in 3DSMax
    light0->setPosition( normalize(osg::Vec4( 0.15, 0.4, 1, 0 )) ); // w=0 - Directional
    light0->setDirection( osg::Vec3( 0, 0, 0 ) );  // Direction = (0,0,0) - Omni light

    osg::LightSource* lightSource0 = new osg::LightSource();
    lightSource0->setLight( light0 );
    lightSource0->setReferenceFrame( osg::LightSource::ABSOLUTE_RF );
    lightSource0->addChild( root.get() );

    osg::Light* light = (osg::Light*)
        root->getOrCreateStateSet()->getAttribute( osg::StateAttribute::LIGHT );
	//viewer.setSceneData(/*root*/lightSource0);
    root->getOrCreateStateSet()->setMode( GL_LIGHTING,osg::StateAttribute::ON );
    root->getOrCreateStateSet()->setAttributeAndModes( light, osg::StateAttribute::ON );

	viewer.setSceneData(/*root*/lightSource0);



//-- Tracking Camera Definition
	// Declare and set up a transform to 'follow' the avatar node.
	osg::ref_ptr<osg::PositionAttitudeTransform> followerPAT = new osg::PositionAttitudeTransform();
	followerPAT->setPosition( osg::Vec3(0,-1000,200) );
	followerPAT->setAttitude( osg::Quat( osg::DegreesToRadians(-10.0f), osg::Vec3(1,0,0) ));
	followerPAT->setName("CameraFollowerPAT");
	OsgAvatar* activeAvatar = static_cast<OsgAvatar*>(ft::ControlManager::getInstance()->getActiveAvatar()->getImplementation());
	activeAvatar->getOffsetTransform()->addChild(followerPAT.get());

	transformAccumulator* avatarWorldCoords = new transformAccumulator();
	avatarWorldCoords->attachToGroup(followerPAT.get());

	osg::ref_ptr<followNodeMatrixManipulator> followAvatar = new followNodeMatrixManipulator(avatarWorldCoords);
	followAvatar->setName("followAvatarMatrix");
// next camera
	osg::ref_ptr<osgGA::TrackballManipulator> Tman = new osgGA::TrackballManipulator();
	Tman->setAutoComputeHomePosition(true);
    Tman->setHomePosition(osg::Vec3d(-3000.0, -3000.0, 600.0), osg::Vec3d(0.0,0.0, 0.0), osg::Vec3d(0.0, 0.0, 1.0),false); // do not auto calculate home
	viewer.setCameraManipulator(Tman.get());

// create avatar tracker and attach it to active avatar
	osg::ref_ptr<osg::Node> avatarTracker = createTracker();
	activeAvatar->getOffsetTransform()->addChild(avatarTracker.get());
	OsgAvatar* lastActiveAvatar = activeAvatar;

	viewer.setRealizeOperation( new CompileStateSets( lightSource0 ) );
    viewer.realize();

// -- Main loop --
    osg::Timer_t startTick = osg::Timer::instance()->tick();

    enum PauseState { Unpaused, Paused };
    PauseState   pauseState = Unpaused;
    osg::Timer_t pauseStartTick = 0;
    double       totalPauseTime = 0; 


    while ( !viewer.done() )
    {
        osg::Timer_t tick = osg::Timer::instance()->tick();

        if ( pauseState == Unpaused && paused )
        {
            pauseState = Paused;
            pauseStartTick = tick;
        }
        if ( pauseState == Paused && !paused )
        {
            pauseState = Unpaused;
            totalPauseTime += osg::Timer::instance()->delta_s( pauseStartTick, tick );
        }

        double currentTime = osg::Timer::instance()->delta_s( startTick, pauseState == Unpaused ? tick : pauseStartTick );

		if (SelectActiveAvatar) //allow to change camera and marker after active avatar was changed
		{
			OsgAvatar* activeAvatar = static_cast<OsgAvatar*>(ft::ControlManager::getInstance()->getActiveAvatar()->getImplementation());
			
			if ( !activeAvatar->getOffsetTransform()->containsNode(followerPAT.get()) ) //if camera node is not already attached to active avatar
			{
				activeAvatar->getOffsetTransform()->addChild(followerPAT.get()); // attach camera to new avatar
				activeAvatar->getOffsetTransform()->addChild(avatarTracker.get()); // attach marker to new active avatar
				if (lastActiveAvatar!=NULL)
				{
					lastActiveAvatar->getOffsetTransform()->removeChild(followerPAT.get()); // and release from previous one
					lastActiveAvatar->getOffsetTransform()->removeChild(avatarTracker.get()); // and release from previous one
				}
			}
			lastActiveAvatar = activeAvatar; // update current avatar pointer
			SelectActiveAvatar = false;
		}
		
		
		if (AllowRefreshCamera) // allow switch between global and local camera
		{
			if (ChangeCamera) // helps to choose between global and local camera
			{
				viewer.setCameraManipulator(followAvatar.get());
				AllowRefreshCamera = false;	
			}
			else
			{
				viewer.setCameraManipulator(Tman.get());
				AllowRefreshCamera = false;
			}
			
		}

        viewer.frame( currentTime - totalPauseTime );

    }

    return 0;
}




/**
 * \brief Main entry to application
 *
 * \param int argc - number of arguments
 * \param char *[] argv - list of arguments
 * \return int - result of application
 **/
int main(int argc, char *argv[])
{
    return RunOSGApp(argc, argv);
}

