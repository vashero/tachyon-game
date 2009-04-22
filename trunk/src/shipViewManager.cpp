#include "shipViewManager.hpp"
#include "gameException.hpp"

ShipViewManager::ShipViewManager(const String &configFilename)
{
	ConfigFile cf;
	cf.load(configFilename);
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	String typeName;
	ConfigFile::SettingsMultiMap *settings;
	while (seci.hasMoreElements()) {
		typeName = seci.peekNextKey();
		settings = seci.getNext();
		this->create_ship_template(typeName, settings);
	}
}

ShipViewManager::~ShipViewManager()
{
}

int ShipViewManager::register_ship(String shipName)
{
	// Find the relevant template
	vector<String>::iterator templateName;
	templateName = find(mTemplateNames.begin(), mTemplateNames.end(), shipName);

	if (templateName == mTemplateNames.end()) {
		// The relevant template has not been found
		throw GameConfigException("Cannot find the template " + shipName);
	}

	// Convert the found iterator into the index into the template list
	int idx = templateName - mTemplateNames.begin();

	// Create a suffix for the ship based the number of ships of that type
	ostringstream buffer;
	buffer << mTemplateCounts[idx]++;
	String suffix = buffer.str();

	// Add the relevant entries to our list
	mShipNames.push_back(mTemplateNames[idx] + suffix);
	mShipEntityNames.push_back(mTemplateEntityNames[idx] + suffix);
	mShipMeshNames.push_back(mTemplateMeshNames[idx]);
	mShipNodeNames.push_back(mTemplateNodeNames[idx] + suffix);
	mShipScales.push_back(mTemplateScales[idx]);

	// Return an index to the created ship
	return mShipNames.size() - 1;
}

String ShipViewManager::get_entity_name(int id)
{
	return mShipEntityNames[id];
}

String ShipViewManager::get_mesh_name(int id)
{
	return mShipMeshNames[id];
}

String ShipViewManager::get_node_name(int id)
{
	return mShipNodeNames[id];
}

Vector3 ShipViewManager::get_scale(int id)
{
	return mShipScales[id];
}

bool ShipViewManager::create_ship_template(String typeName,
		ConfigFile::SettingsMultiMap *settings)
{
	// Get the remaining information we need to create the template
	ConfigFile::SettingsMultiMap::iterator i;
	String meshName;
	Real scale;
	bool gotMeshName = false, gotScale = false;
	for (i = settings->begin(); i != settings->end(); ++i) {
		if (i->first == SHIP_VIEW_MANAGER_CFG_MESH_NAME) {
			meshName = i->second;
			gotMeshName = true;
		} else if (i->first == SHIP_VIEW_MANAGER_CFG_SCALE) {
			istringstream buffer(i->second);
			buffer >> scale;
			gotScale = true;
		}
	}

	// Make sure we have all the information we need
	if (!gotMeshName || !gotScale) {
		return false;
	}

	// Create the template
	mTemplateNames.push_back(typeName);
	mTemplateEntityNames.push_back(typeName);
	mTemplateMeshNames.push_back(meshName);
	mTemplateNodeNames.push_back(typeName + SHIP_VIEW_MANAGER_NODE_SUFFIX);
	mTemplateScales.push_back(Vector3(scale));
	mTemplateCounts.push_back(0);

	return true;
}
