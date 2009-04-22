#pragma once

#include "Ogre.h"
using namespace Ogre;

class ShipViewManager;

/** Different ship types */
typedef enum {
	SHIP_VIEW_CRUISER,
	SHIP_VIEW_SUPERIORITY,
	SHIP_VIEW_TEST,
	SHIP_VIEW_NONE /**< Used to count the number of ship views; must always be
                     the final enum */
} ShipViewType;

const String SHIP_VIEW_TYPE_NAMES[] = {"Cruiser", "Superiority", "Ogre"};

/**
   Handles the appearance of a ship.
 */
class ShipView
{
public:
	// Methods --------------------
	/** The default constructor
      @param type The type of ship to create
      @param sceneMgr The scene manager to add the ship to
	 */
	ShipView(ShipViewType type, SceneManager *sceneMgr,
			ShipViewManager *shipViewMgr, Vector3 pos, Radian yaw);

	/** The destructor */
	~ShipView();

	/** Create the relevant ogre objects for this ship
      @param sceneMgr The scene manager to add this ship to
      @param pos The initial position of the ship
      @param rot The initial rotation of the ship
	 */
	void initialize(SceneManager *sceneMgr, Vector3 pos, Radian yaw);

	/** Remove this ship from the scene */
	void destroy();

private:
	/** The node holding our ship */
	SceneNode *mShipNode;

	/** Used to get static ship information */
	ShipViewManager *mShipViewMgr;

	/** Used to identify ourselves with our ship manager */
	int mId;

	/** The scene manager we add ourselves to */
	SceneManager *mSceneMgr;
};
