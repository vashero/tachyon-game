#include "arenaView.hpp"

ArenaView::ArenaView(RenderWindow *win, const String &shipCfgFilename,
		const String &mgrName): mShipViewMgr(shipCfgFilename)
		{
	mCamPos = ARENA_VIEW_DEF_CAM_POS;
	mTargetPos = ARENA_VIEW_DEF_TARGET_POS;
	mWin = win;
	this->create_scene(mgrName);
		}

ArenaView::~ArenaView()
{
}

void ArenaView::create_scene(const String &mgrName)
{
	// See if there already is a scene manager with this name
	mSceneMgr = Root::getSingletonPtr()->getSceneManager(mgrName);
	if (!mSceneMgr) {
		// If not, create one
		mSceneMgr = Root::getSingletonPtr()->createSceneManager(ST_GENERIC,
				mgrName);
	}

	// Set the shadow technique
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);

	// Set the number of mipmaps
	//TextureManager::getSingleton().setDefaultNumMipmaps(0);

	// Create a default skybox
	mSceneMgr->setSkyBox(true, ARENA_VIEW_SKYBOX_NAME);

	// Create ambient light
	mSceneMgr->setAmbientLight(ARENA_VIEW_AMB_LIGHT);
	//mSceneMgr->setAmbientLight(ColourValue::Black);

	// Create directional light sources
	int lightCount = sizeof(ARENA_VIEW_DIR_LIGHT_NAME) / sizeof(String);
	Light *light;
	for (int lightIdx = 0; lightIdx < lightCount; lightIdx++) {
		light = mSceneMgr->createLight(ARENA_VIEW_DIR_LIGHT_NAME[lightIdx]);
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(ARENA_VIEW_DIR_LIGHT_DIR[lightIdx]);
		light->setDiffuseColour(ARENA_VIEW_DIR_LIGHT_DIFFUSE[lightIdx]);
		light->setSpecularColour(ARENA_VIEW_DIR_LIGHT_SPECULAR[lightIdx]);
	}

	// Create a camera
	Camera *camera = mSceneMgr->createCamera(ARENA_VIEW_CAM_NAME);
	camera->setNearClipDistance(ARENA_VIEW_CAM_CLIP);

	// Create the node that the camera is looking at
	SceneNode *targetNode = mSceneMgr->getRootSceneNode()->createChildSceneNode
	(ARENA_VIEW_TARGET_NODE_NAME);
	targetNode->setPosition(mTargetPos);
	targetNode->lookAt(mCamPos, Node::TS_WORLD);

	// Create the camera node
	SceneNode *cameraNode = targetNode->createChildSceneNode(ARENA_VIEW_CAM_NODE_NAME);
	cameraNode->translate(Vector3(0, 0, mCamPos.distance(mTargetPos)), Node::TS_PARENT);
	cameraNode->lookAt(Vector3(0, 0, 0), Node::TS_PARENT);
	cameraNode->attachObject(camera);

	// Create the viewport to hold the camera
	Viewport *vp = mWin->addViewport(camera);
	vp->setBackgroundColour(ColourValue::Black);
	camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void ArenaView::add_ship(const ShipViewType &type, const Vector3 &pos,
		const Radian &yaw)
{
	mShips.push_back(ShipView(type, mSceneMgr, &mShipViewMgr, pos, yaw));
}

void ArenaView::clear_ships()
{
	vector<ShipView>::iterator i;
	for (i = mShips.begin(); i != mShips.end(); ++i) {
		i->destroy();
	}
	mShips.clear();
}

void ArenaView::rotate_camera(const Radian &xAngle, const Radian &yAngle)
{
	SceneNode *node = mSceneMgr->getSceneNode(ARENA_VIEW_TARGET_NODE_NAME);
	node->yaw(xAngle, Node::TS_WORLD);
	node->pitch(yAngle, Node::TS_LOCAL);
}
