#pragma once

#include "Ogre.h"
using namespace Ogre;

#include <vector>
using namespace std;

const String SHIP_VIEW_MANAGER_CFG_MESH_NAME = "meshName";
const String SHIP_VIEW_MANAGER_CFG_SCALE = "scale";
const String SHIP_VIEW_MANAGER_NODE_SUFFIX = "Node";

/**
   Manages required properties for each ship type
 */
class ShipViewManager
{
public:
	// Methods --------------------
	/** The default constructor
      @param configFilename The config file that contains ship property
      information
	 */
	ShipViewManager(const String &configFilename);

	/** The destructor */
	~ShipViewManager();

	/** Register as a ship view
      @param shipName A name identifying the current ship.  It must be
      in the mTemplateNames list
      @return The identifier the registered view is to use
	 */
	int register_ship(String shipName);

	/** Get the name of the entity to be created
      @param id The id of the relevant ship
      @return The entity name
	 */
	String get_entity_name(int id);

	/** Get the mesh name for the entity to be created
      @param id The id of the relevant ship
      @return The mesh name
	 */
	String get_mesh_name(int id);

	/** Get the name of the node the relevant ship is attached to
      @param id The id of the relevant ship
      @return The node name
	 */
	String get_node_name(int id);

	/** Get the scale to be applied to the relevant ship
      @param id The id of the relevant ship
      @return The scale
	 */
	Vector3 get_scale(int id);

protected:
	// Variables --------------------
	/** Holds configuration information for the different ship types */
	ConfigFile cf;

	// Entities used to create ships ----------
	vector<String> mTemplateNames;
	vector<String> mTemplateEntityNames;
	vector<String> mTemplateMeshNames;
	vector<String> mTemplateNodeNames;
	vector<Vector3> mTemplateScales;

	/** Indicates the number of times this template has been used to create a
      ship */
	vector<int> mTemplateCounts;

	// Ships that have been created ----------
	vector<String> mShipNames;
	vector<String> mShipEntityNames;
	vector<String> mShipMeshNames;
	vector<String> mShipNodeNames;
	vector<Vector3> mShipScales;

	// Methods --------------------
	/** Create a ship template
      @param typeName The name of the ship type.  Must be in SHIP_TYPE_NAMES
      @param settings The settings used for this ship type
      @return Whether or not we successfully added a ship template
	 */
	bool create_ship_template(String typeName,
			ConfigFile::SettingsMultiMap *settings);
};
