/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak,mka
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
#include <osg/BlendFunc>
#include <osg/AlphaFunc>
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
#include <osgGA/NodeTrackerManipulator>

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
#include "../avatar/avatarupdatecallback.h"
#include "../control/controlmanager.h"
#include "config.h"
#include "../utility/stringhelper.h"

//#include "../../FollowNodeManip.h"

using namespace ft;

const std::string  AVATAR_TYPE = "../../data/models/freebie/freebie.cfg";

bool PlaceCamera = false;
bool CameraSet = false;

void toggleView()
{
   PlaceCamera^=true;
   CameraSet = true;
}

 

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
							CameraSet = true;
						}
					} else if ( ea.getKey() == ea.KEY_F10 ) { //camera
						toggleView();			
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
	
}

void IntiUpdateCallbackForAvatar(Avatar* avatar)
{
	OsgAvatar* avImpl = static_cast<OsgAvatar*>(avatar->getImplementation());
	osgCal::Model* model = avImpl->getOsgModel();

	//replace update callback from OsgCal with proper one for evolution
	model->setUpdateCallback(0);
	// TODO: check how to destroy updatecallback
	//osg::NodeCallback* oldUpdateCallback = model->getUpdateCallback();
	//delete oldUpdateCallback.;
	model->setUpdateCallback(new AvatarUpdateCallback(avatar));

}

osg::Node* createFloor(const osg::Vec3& center,float radius)
{

   
    int numTilesX = 10;
    int numTilesY = 10;
   
    float width = 2*radius;
    float height = 2*radius;
   
    osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
    osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
    osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));
   
    // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
    osg::Vec3Array* coords = new osg::Vec3Array;
    int iy;
    for(iy=0;iy<=numTilesY;++iy)
    {
        for(int ix=0;ix<=numTilesX;++ix)
        {
            coords->push_back(v000+dx*(float)ix+dy*(float)iy);
        }
    }
   
    //colors when textures doesn't work
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1,1,1,1)); //(0.245f,0.245f,0.245f,1.0f)); // gray

   
    int numIndicesPerRow=numTilesX+1;
    osg::UByteArray* coordIndices = new osg::UByteArray; // assumes we are using less than 256 points...
    osg::UByteArray* textureIndices = new osg::UByteArray;
	osg::ref_ptr<osg::Vec2Array> texCoords (new osg::Vec2Array());
    for(iy=0;iy<numTilesY;++iy)
    {
        for(int ix=0;ix<numTilesX;++ix)
        {
            // four vertices per quad.
            coordIndices->push_back(ix    +(iy+1)*numIndicesPerRow);
            coordIndices->push_back(ix    +iy*numIndicesPerRow);
            coordIndices->push_back((ix+1)+iy*numIndicesPerRow);
            coordIndices->push_back((ix+1)+(iy+1)*numIndicesPerRow);
           
            // one texture per quad
            //textureIndices->push_back(ix+iy);
			
			texCoords->push_back (osg::Vec2 (0.0f, 0.0f));
			texCoords->push_back (osg::Vec2 (0.0f, 1.0f));
			texCoords->push_back (osg::Vec2 (1.0f, 1.0f));
			texCoords->push_back (osg::Vec2 (1.0f, 0.0f));
        }

    }
   

    // set up a single normal
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
   

    osg::Geometry* geom = new osg::Geometry;
    geom->setVertexArray(coords);
    geom->setVertexIndices(coordIndices);
    geom->setTexCoordArray (0, texCoords.get());
    geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
   
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
   
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));
   
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);
	
	// load an image by reading a file: 
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../../data/textures/0861_floor.tga");
    if (!image)
    {
	   std::cout << " couldn't find texture, quitting." << std::endl;
       exit (EXIT_FAILURE);
    }
	else
	{
		//_floorTexture->setBorderWidth(0.8f);
		// texture in stateset
		osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
		tex->setImage(image.get());
		// After creating the OpenGL texture object, release the internal ref_ptr<Image> (to delete the Image).
		tex->setUnRefImageDataAfterApply( true );
		osg::StateSet* state = new osg::StateSet();
		// Assign texture unit 0 of our new StateSet to the texture we just created and enable the texture.
		state->setTextureAttributeAndModes(0,tex.get());
		
		// Additionally but not necessary Turn on blending
		osg::BlendFunc* bf = new osg::BlendFunc(
			osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
		state->setAttributeAndModes( bf );

		// Turn on alpha testing
		osg::AlphaFunc* af = new osg::AlphaFunc(
			osg::AlphaFunc::GREATER, 0.05f );
		state->setAttributeAndModes( af );

		geode->setStateSet(state);
	}

    return geode;
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

	// Loads configuration from 'application.cfg'
    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();



	osg::ref_ptr< osg::Group > root = new osg::Group();
	
	// floor

	osg::Vec3 center(0.0f,0.0f,0.0f);
    float radius = 2000.0f;
    osg::Node* floorModel = createFloor(center,radius);
    osg::MatrixTransform* worldTransformNode = new osg::MatrixTransform;
	worldTransformNode->setMatrix(osg::Matrix::rotate(osg::inDegrees(5.0f),1.0f,0.0f,0.0f));

    worldTransformNode->addChild(floorModel);

	root->addChild(worldTransformNode);


	//------------ EVOLUTION init

	Cal3dImpl m_evolutionImpl;
	Cal3dAnimProvider m_animProvider;
	World* m_world;

	m_evolutionImpl.Init();
	m_animProvider.Init();
	m_world = ft::Factory::getInstance()->CreateWorld();
	//EvoDBG::setTimelineLevel(1);
	ft::ControlManager::getInstance()->Init(); //enforced creation of singleton

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
    float x_off = -150, z_off = 0;

	std::string _nameHelper;
    for (int i=0; i<avatar_number; i++)
    {
		_nameHelper.empty();
		_nameHelper = "Avatar" + ft::StringHelper::itos(i);

		std::string modelName;

//		if (i%2==0)
			modelName = "../../data/models/freebie/freebie.cfg";
//	    else
//			modelName = "../../data/models/freebie/cally.cfg";

		av = dynamic_cast<Avatar*>(ft::Factory::getInstance()->createAvatar(_nameHelper, modelName)); 
        if (av != NULL)  
        {
            //av->Init();
            //av->Dump();

			vStartPos.set(vStartPos.x() + x_off, vStartPos.y(), vStartPos.z() + z_off);

			OsgAvatar* avImpl = static_cast<OsgAvatar*>(av->getImplementation());
			worldTransformNode->addChild(  avImpl->getOffsetTransform()  );

			ControlManager::getInstance()->AddAvatar(av);
			av->AddController(new LCSModifier());
			av->AddController(avImpl->getStopController()); //mka 2008.08.19
			m_world->AddAvatar(av);
			IntiUpdateCallbackForAvatar(av);
			avImpl->setPosition(vStartPos);


            if (i==0)
            {
                ControlManager::getInstance()->setActiveAvatar(0);
            }
			av->StartSimulation();
        }
    }

	InitWorld(m_world);
	m_world->DumpActions();
	m_world->DumpAvatars();
	m_world->SetActionForAvatars("idle");

	m_world->StartThinking();

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

	// Declare and set up a transform to 'follow' the avatar node.
	//////osg::PositionAttitudeTransform *followerPAT = new osg::PositionAttitudeTransform();
	//////followerPAT->setPosition( osg::Vec3(0,-1000,200) );
	//////followerPAT->setAttitude( osg::Quat( osg::DegreesToRadians(-10.0f), osg::Vec3(1,0,0) ));

	//////OsgAvatar* activeAvatar = static_cast<OsgAvatar*>(ft::ControlManager::getInstance()->getActiveAvatar()->getImplementation());
	//////activeAvatar->getOffsetTransform()->addChild(followerPAT);

	//////transformAccumulator* avatarWorldCoords = new transformAccumulator();
	//////avatarWorldCoords->attachToGroup(followerPAT);

 //////   followNodeMatrixManipulator* followAvatar = new followNodeMatrixManipulator(avatarWorldCoords);

	//osgGA::TrackballManipulator *Tman = new osgGA::TrackballManipulator();
 //   
	//osgGA::MatrixManipulator *cView = Tman;
	//
	//viewer.setCameraManipulator(cView);
 //   
	//////osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
	//////keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() ); 
	//////keyswitchManipulator->addMatrixManipulator( '2', "Avatar", followAvatar );
 //////   viewer.setCameraManipulator( keyswitchManipulator.get() );    
	

// next camera
	osg::ref_ptr<osgGA::NodeTrackerManipulator> followAvatar = new osgGA::NodeTrackerManipulator;
	followAvatar->setTrackerMode(osgGA::NodeTrackerManipulator::TrackerMode::NODE_CENTER);
	followAvatar->setRotationMode(osgGA::NodeTrackerManipulator::ELEVATION_AZIM);

	osg::ref_ptr<osgGA::TrackballManipulator> Tman = new osgGA::TrackballManipulator();
	Tman->setAutoComputeHomePosition(true);
	
	viewer.setCameraManipulator(Tman.get());

	viewer.setRealizeOperation( new CompileStateSets( lightSource0 ) );
	//OsgAvatar* av = static_cast<OsgAvatar*>(avatar->getImplementation());
	//osg::Node *trObj = av->getOffsetTransform();
	//viewer.getCameraManipulator()->setNode(trObj);
	////viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(-1000,-1000,0),osg::Vec3d(0,0,0),osg::Vec3d(0,0,1));
	
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

		if (CameraSet)
		{
			if (PlaceCamera){

				OsgAvatar* activeAvatar = static_cast<OsgAvatar*>(ft::ControlManager::getInstance()->getActiveAvatar()->getImplementation());
				followAvatar->setTrackNode(activeAvatar->getOffsetTransform());
				viewer.setCameraManipulator(followAvatar.get());
	//			activeAvatar->getOffsetTransform()->addChild(followerPAT);
	//			keyswitchManipulator->getMatrixManipulatorWithIndex(2)->setNode(NULL);
	//			keyswitchManipulator->setNodeselectMatrixManipulator(2);
				CameraSet = false;
				
			}
			else
			{
				viewer.setCameraManipulator(Tman.get());
				CameraSet = false;
			}
		}
		//	//viewer.setCameraManipulator(followAvatar);
		//	cView = followAvatar;
		//	viewer.setCameraManipulator(cView);
		//}
		//else
		//{
		//	cView=Tman;
		//	viewer.getCamera()->setViewport viewer.setCameraManipulator(cView);
		//}
			//viewer.setCameraManipulator(Tman->);

		//m_world->Update(currentTime - totalPauseTime);
		//avatar->ManualUpdate(currentTime - totalPauseTime);
		//avatar2->ManualUpdate(currentTime - totalPauseTime);
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

