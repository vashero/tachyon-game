/*==============================================================
 * Copyright (c) 2009 Blake Fisher.  All Rights Reserved.
 *
 * This software is released under the BSD License
 * <http://www.opensource.org/licenses/bsd-license.php>
 *==============================================================
 */
#include "shipView.hpp"
#include "shipViewManager.hpp"

ShipView::ShipView(ShipViewType type, SceneManager *sceneMgr,
		ShipViewManager *shipViewMgr,
		Vector3 pos, Radian yaw)
{
	mSceneMgr = NULL;
	mShipViewMgr = shipViewMgr;
	mId = mShipViewMgr->register_ship(SHIP_VIEW_TYPE_NAMES[type]);

	this->initialize(sceneMgr, pos, yaw);
}

ShipView::~ShipView()
{
}

void ShipView::initialize(SceneManager *sceneMgr,
		Vector3 pos, Radian yaw)
{
	mSceneMgr = sceneMgr;

	// Create the entity
	String entityName = mShipViewMgr->get_entity_name(mId);
	String meshName = mShipViewMgr->get_mesh_name(mId);
	Entity *ent = mSceneMgr->createEntity(entityName, meshName);

	// Create the node to hold the entity
	String nodeName = mShipViewMgr->get_node_name(mId);
	mShipNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	mShipNode->attachObject(ent);
	mShipNode->setScale(mShipViewMgr->get_scale(mId));

	// Move he object to its initial position
	mShipNode->translate(pos, Node::TS_WORLD);
	mShipNode->yaw(yaw, Node::TS_WORLD);
}

void ShipView::destroy()
{
	if (mSceneMgr) {
		String nodeName = mShipViewMgr->get_node_name(mId);
		mSceneMgr->destroySceneNode(nodeName);
	}
}
