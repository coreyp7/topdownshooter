#include "QuadTree.h"

// Set defaults to static limit stuff (these are good values I've found).
int QuadTree::LIMIT = 4;
int QuadTree::DEPTH_LIMIT = 5;
//int QuadTree::LIMIT = 1;
//int QuadTree::DEPTH_LIMIT = 7;

QuadTree::QuadTree(float x, float y, float width, float height) {
	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
	isLeaf = true;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;
}

QuadTree::~QuadTree() {
	if (!isLeaf) {
		delete nw;
		delete ne;
		delete sw;
		delete se;
	}
}

void QuadTree::insert(Entity* point) {

	// First check if this isn't a leaf node.
	if (!isLeaf) {
		if (nw->contains(point)) {
			nw->insert(point);
		}
		if (ne->contains(point)) {
			ne->insert(point);
		}
		if (sw->contains(point)) {
			sw->insert(point);
		}
		if (se->contains(point)) {
			se->insert(point);
		}
	}
	else {
		points.push_back(point);

		// Are there too many points in this leaf (square)?
		// If so, then create our 4 quadtrees and insert
		// all my points into my (appropriate) children.
		if (points.size() >= LIMIT) {

			if (depth >= DEPTH_LIMIT) {
				// ignore; stop making new quadtrees.
				return;
			}

			isLeaf = false;

			nw = new QuadTree(x, y, width / 2, height / 2);
			ne = new QuadTree(x + (width / 2), y, width / 2, height / 2);
			sw = new QuadTree(x, y + (height / 2), width / 2, height / 2);
			se = new QuadTree(x + (width / 2), y + (height / 2), width / 2, height / 2);
			// todo: put this in quadtree constructor.
			nw->depth = this->depth + 1;
			ne->depth = this->depth + 1;
			sw->depth = this->depth + 1;
			se->depth = this->depth + 1;

			// Add entity to each leaf it belongs to.
			for (int i = 0; i < points.size(); i++) {
				if (nw->contains(points[i])) {
					nw->insert(points[i]);
				}
				if (ne->contains(points[i])) {
					ne->insert(points[i]);
				}
				if (sw->contains(points[i])) {
					sw->insert(points[i]);
				}
				if (se->contains(points[i])) {
					se->insert(points[i]);
				}
			}

			// Only leafs can contain entities, so clear this tree's points.
			points.clear();
		}
	}
}

// Check if an entity is inside of this QuadTree.
bool QuadTree::contains(Entity* point) {
	SDL_FRect rect = *point->getFRect();

	bool colX = ((rect.x + rect.w) >= x) && ((x + width) >= rect.x);
	bool colY = ((rect.y + rect.h) >= y) && ((y + height) >= rect.y);

	return colX && colY;
}

// Will return all leafs which the passed in Entity
// is contained in. (An object can be in more than one leaf at a time).
std::vector<QuadTree*> QuadTree::getLeafs(Entity* dot) {

	// If not leaf; call getLeaf on children which contain rect.
  // Else, return yourself in a vector.
	std::vector<QuadTree*> trees;
	if (!isLeaf) {
		if (nw->contains(dot)) {
			std::vector<QuadTree*> newTrees = nw->getLeafs(dot);
			trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
		if (ne->contains(dot)) {
			std::vector<QuadTree*> newTrees = ne->getLeafs(dot);
			trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
		if (sw->contains(dot)) {
			std::vector<QuadTree*> newTrees = sw->getLeafs(dot);
			trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
		if (se->contains(dot)) {
			std::vector<QuadTree*> newTrees = se->getLeafs(dot);
			trees.insert(trees.end(), newTrees.begin(), newTrees.end());
		}
	}
	else {
		trees.push_back(this);
	}
	return trees;
}

std::set<std::tuple<Uint16, Uint16>> QuadTree::getCollisionsWithEntity(Entity* entity) {
	std::set<std::tuple<Uint16, Uint16>> collisionList;

	// Get all the leafs which contain entity. (max of 4)
	std::vector<QuadTree*> currentEntityLeafs = getLeafs(entity);

	for (int j = 0; j < currentEntityLeafs.size(); j++) {
		QuadTree* currLeaf = currentEntityLeafs[j];

		//	Iterate through other entities inside of this leaf
		//	and check for collision with 'curr' rect.
		for (int k = 0; k < currLeaf->points.size(); k++) {
			Entity* otherEntity = currLeaf->points[k];

			if (entity->id == otherEntity->id) {
				continue; // ignore; comparing entity with itself
			}

			if (entity == nullptr || otherEntity == nullptr) {
				printf("!!!!hit that continue!!!!\n");
				continue; // ignore; this entity has been removed during this frame
			}

			// If there's a collision between the two entities,
			// add ordered tuple containing the two ids to collision vec.
			bool collision = checkCollision(entity, otherEntity);
			if (collision) {
				std::tuple<Uint16, Uint16> newtuple;
				if (entity->getEntityType() < otherEntity->getEntityType()) {
					newtuple = { entity->id, otherEntity->id };
				}
				else {
					newtuple = { otherEntity->id, entity->id };
				}
				collisionList.insert(newtuple);
			}
		}
	}
	return collisionList;
}

bool QuadTree::checkCollision(Entity* entity1, Entity* entity2) {
	SDL_FRect rect1 = *entity1->getFRect();
	SDL_FRect rect2 = *entity2->getFRect();

	bool xCollision = (((rect1.x + rect1.w) >= (rect2.x)) && ((rect2.x + rect2.w) >= (rect1.x)));
	bool yCollision = (((rect1.y + rect1.h) >= (rect2.y)) && ((rect2.y + rect2.h) >= (rect1.y)));

	return xCollision && yCollision;
}

bool QuadTree::remove(Entity* entity) {
	SDL_FRect thisNode = { x, y, width, height };
	bool successful = true;

	// if its not a leaf, that means its the encompassing QuadTree.
	// otherwise, its a leaf which will delete any instances it can
	// find of the entity. Kindof a werider semi-recursive method.
	// Could be split into two methods actually...
	// Only called from outside the QuadTree.
	if (!isLeaf) {
		std::vector<QuadTree*> leafsToRemoveFrom = getLeafs(entity);

		for (int i = 0; i < leafsToRemoveFrom.size(); i++) {
			QuadTree* leaf = leafsToRemoveFrom[i];
			if (leaf->remove(entity) == false) {
				successful = false;
			}
		}
		return successful;
	}
	else {
		for (int i = 0; i < points.size(); i++) {
			if (points[i]->id == entity->id) {
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	return false; // ??????? it shouldn't ever reach here lol
}


// Legacy function from original demo; keeping for convenience.
//void QuadTree::draw(Renderer* renderer) {
//	SDL_FRect rect = { x,y,width,height };
//
//	/*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//	SDL_RenderDrawRect(renderer, &rect);
//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);*/
//	//renderer->drawRectRelativeToCamera(rect);
//
//	if (isLeaf) {
//		for (int i = 0; i < points.size(); i++) {
//			//SDL_RenderDrawRectF(renderer, points[i]->rect);
//			//SDL_RenderDrawRectF(renderer, &points[i]->pos);
//			//renderer->drawRectRelativeToCamera(rect);
//		}
//	}
//	else {
//		nw->draw(renderer);
//		ne->draw(renderer);
//		sw->draw(renderer);
//		se->draw(renderer);
//	}
//}

