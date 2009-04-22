#pragma once

#include "shipView.hpp"
#include "shipViewManager.hpp"

#include <Ogre.h>
using namespace Ogre;

// Skybox
const String ARENA_VIEW_SKYBOX_NAME = "Skyboxes/Skybox";

// Lighting
const ColourValue ARENA_VIEW_AMB_LIGHT(0.75, 0.75, 0.75);
const String ARENA_VIEW_DIR_LIGHT_NAME[] = {"Light0", "Light1"};
const Vector3 ARENA_VIEW_DIR_LIGHT_DIR[] = {Vector3(0.1, -1, 0), Vector3(1, 0, 0)};
const ColourValue ARENA_VIEW_DIR_LIGHT_DIFFUSE[] = {ColourValue(1, 1, 1), ColourValue(1, 1, 1)};
const ColourValue ARENA_VIEW_DIR_LIGHT_SPECULAR[] = {ColourValue(1, 1, 1), ColourValue(1, 1, 1)};

// Cameras
const String ARENA_VIEW_CAM_NAME = "Camera";
const Real ARENA_VIEW_CAM_CLIP = 5;
const Vector3 ARENA_VIEW_DEF_CAM_POS(0, -1000, 0);
const Vector3 ARENA_VIEW_DEF_TARGET_POS(0, 0, 0);
const String ARENA_VIEW_TARGET_NODE_NAME = "TargetNode";
const String ARENA_VIEW_CAM_NODE_NAME = "CameraNode";

/**
   A generic arena display.  Handles skybox, cameras and lighting.
   Also holds ships.
 */
class ArenaView
{
public:

	/** The default constructor
      @param win The window we are rendering in
      @param mgrName The name of the scene manager we are creating
	 */

	ArenaView(RenderWindow *win, const String &shipCfgFilename,
			const String &mgrName = StringUtil::BLANK);

	/** The destructor */
	~ArenaView();

	/** Create a scene */
	void create_scene(const String &mgrName);

	/** Add a ship to the scene */
	void add_ship(const ShipViewType &type, const Vector3 &pos, const Radian &yaw);

	/** Remove all ships from the view */
	void clear_ships();

	/** Rotate the camera about the node */
	void rotate_camera(const Radian &xAngle, const Radian &yAngle);

protected:
	/** The point in space that the camera is located */
	Vector3 mCamPos;

	/** The point in space that the camera is looking at */
	Vector3 mTargetPos;

	/** The window we are rendering in */
	RenderWindow *mWin;

	/** The scene manager that contains all scene components */
	SceneManager *mSceneMgr;

	/** The ship manager */
	ShipViewManager mShipViewMgr;

	/** The ships in our view */
	vector<ShipView> mShips;
};
