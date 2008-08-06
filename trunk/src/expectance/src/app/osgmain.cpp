/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */

#include "osgmain.h"

#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include <osg/MatrixTransform>
#include <osg/CullFace>
#include <osg/Light>
#include <osg/Notify>
#include <osg/ShadeModel>
#include <osg/LightModel>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Texture2D>
#include <osgUtil/GLObjectsVisitor>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

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


using namespace ft;

const std::string  AVATAR_TYPE = "../../data/models/freebie/freebie.cfg";

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
			, m_activeAvatar( activeAvatar )
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
									m_activeAvatar->ExecuteAction("idle");
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
							osg::Node* node = avImpl->getOsgModel();
							// set rotation here
						}

					}  else if ( ea.getKey() == ea.KEY_Right ) {
						if (m_activeAvatar != NULL)
						{
							OsgAvatar* avImpl = (OsgAvatar*)m_activeAvatar->getImplementation();
							osg::Node* node = avImpl->getOsgModel();
							// set rotation here
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
//	Goal* goal = new RandomMoveGoal();
//	m_world->AddGoal(goal);

//	goal = new ChangeDirGoal();
//	m_world->AddGoal(goal);

//	Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
//	m_world->AddRule(rule);


	//world->LoadDataForType("data/motions.xml", "cally");
	world->LoadDataForType("../../data/motions.xml", AVATAR_TYPE);

	InitActionsForType(world, AVATAR_TYPE);
	//InitActionsForType(world, "cally");
	InitGraphForType(world, AVATAR_TYPE);
	//InitGraphForType(world, "cally");
	
	//ControlManager::getInstance()->getActiveAvatar()->ExecuteAction("idle");
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

    if ( arguments.argc() <= 1 || fn == "" )
    {
        arguments.getApplicationUsage()->write( std::cout,
                                                osg::ApplicationUsage::COMMAND_LINE_OPTION );
        return 1;
    }

    if ( arguments.read( "--no-debug" ) == false )
    {
        osg::setNotifyLevel( osg::DEBUG_FP );
    }

//    osg::Group* root = new osg::Group();

	//------------ EVOLUTION init

	Cal3dImpl m_evolutionImpl;
	Cal3dAnimProvider m_animProvider;
	World* m_world;

	m_evolutionImpl.Init();
	m_animProvider.Init();
	m_world = ft::Factory::getInstance()->CreateWorld();
	EvoDBG::setTimelineLevel(1);



    osg::ref_ptr< osg::Group > root = new osg::Group();

  Avatar* avatar = Factory::getInstance()->createAvatar("av1",AVATAR_TYPE);
  if (avatar != NULL)
  {
	  OsgAvatar* av = static_cast<OsgAvatar*>(avatar->getImplementation());
	  root->addChild(  av->getOffsetTransform()  );

	  m_world->AddAvatar(avatar);
	  avatar->StartSimulation();
	  
  }

  Avatar* avatar2 = Factory::getInstance()->createAvatar("av2",AVATAR_TYPE);
  if (avatar2 != NULL)
  {
	  OsgAvatar* av = static_cast<OsgAvatar*>(avatar2->getImplementation());
	  root->addChild(  av->getOffsetTransform()  );

	  m_world->AddAvatar(avatar2);
	  avatar2->StartSimulation();
	  av->setPosition(osg::Vec3d(300,0,0));
	  
  }

  InitWorld(m_world);
  m_world->DumpActions();
  m_world->DumpAvatars();

//        root->addChild( makeModel( coreModel.get(),
//                                   meshAdder.get(),
//                                   animNum ) );


	m_world->StartThinking();

//	if (avatar != NULL)
//		avatar->ExecuteAction("idle");

    // -- Setup viewer --
//    while ( true )
    // shaders recompiled and linked OK but doesn't work on reloads (glValidateProgram FAILED?)
    {
    osgViewer::Viewer viewer;

    if ( arguments.read( "--four-window" ) )
    {
        addWindow( viewer,   0,   0, 640, 480,  1.0, -1.0 );
        addWindow( viewer, 640,   0, 640, 480, -1.0, -1.0 );
        addWindow( viewer,   0, 480, 640, 480,  1.0,  1.0 );
        addWindow( viewer, 640, 480, 640, 480, -1.0,  1.0 );
    }

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
    viewer.addEventHandler( new ToggleHandler( paused, 'p', "Pause animation", avatar ) );

    while (arguments.read("--SingleThreaded")) viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    while (arguments.read("--CullDrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::CullDrawThreadPerContext);
    while (arguments.read("--DrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::DrawThreadPerContext);
    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) viewer.setThreadingModel(osgViewer::Viewer::CullThreadPerCameraDrawThreadPerContext);

//    viewer.getCullSettings().setDefaults();
//    viewer.getCullSettings().setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
//    viewer.getCullSettings().setCullingMode( osg::CullSettings::DEFAULT_CULLING & ~osg::CullSettings::NEAR_PLANE_CULLING);

//    root->getOrCreateStateSet()->setAttributeAndModes( new osg::CullFace, osg::StateAttribute::ON );
    // turn on back face culling

// light is already smooth by default
//    osg::ShadeModel* shadeModel = new osg::ShadeModel;
//    shadeModel->setMode( osg::ShadeModel::SMOOTH );
//    root->getOrCreateStateSet()->setAttributeAndModes( shadeModel, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

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

//     // -- light #1 --
//     osg::Light* light1 = new osg::Light();
//     light1->setLightNum( 1 );
//     light1->setAmbient( osg::Vec4( 0, 0, 0, 1 ) );
//     light1->setDiffuse( osg::Vec4( 0.4, 0.4, 0.4, 1 ) );
//     light1->setSpecular( osg::Vec4( 1, 1, 1, 0 ) );
//     // as in SceneView, except direction circa as in 3DSMax
//     light1->setPosition( normalize(osg::Vec4( -0.15, -0.4, -1, 0 )) ); // w=0 - Directional
//     light1->setDirection( osg::Vec3( 0, 0, 0 ) );  // Direction = (0,0,0) - Omni light

//     osg::LightSource* lightSource1 = new osg::LightSource();
//     lightSource1->setLight( light1 );
//     lightSource1->setReferenceFrame( osg::LightSource::ABSOLUTE_RF );
//     lightSource1->addChild( lightSource0 );

    viewer.setSceneData(/*root*/lightSource0);
//    root->getOrCreateStateSet()->setMode( GL_LIGHTING,osg::StateAttribute::ON );
//    root->getOrCreateStateSet()->setAttributeAndModes( light, osg::StateAttribute::ON );

//    osg::Light* light = (osg::Light*)
//        root->getOrCreateStateSet()->getAttribute( osg::StateAttribute::LIGHT );
//    std::cout << "light: " << light << std::endl;
//    viewer.getEventHandlerList().push_back( new osgGA::TrackballManipulator() );

    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
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

        double currentTime = osg::Timer::instance()->delta_s( 
            startTick,
            pauseState == Unpaused ? tick : pauseStartTick );

		m_world->Update(currentTime - totalPauseTime);
        viewer.frame( currentTime - totalPauseTime );

    }

//    viewer.setSceneData( new osg::Group() ); // destroy scene data before viewer
    // ^ buggy in multi-threaded mode, or not???
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

