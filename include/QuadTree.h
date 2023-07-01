#pragma once
#include <SDL.h> // using only for certain structs
#include <vector>
#include <tuple>
#include <set>
//#include "Entity.h"
#include "Entity.h"

class Renderer;  // forward declaration that should only be needed for debugging quadtree.

class QuadTree {
public:
	// LIMIT: max count of rects in a QuadTree
	// DEPTH_LIMIT: maximum depth that an entire QuadTree can have.
	static int LIMIT, DEPTH_LIMIT; // values declared in cpp file

	// Pointers to all children of this 'node'. 
	// @refactor: i shouldve just made this a list.
	QuadTree* nw;
	QuadTree* ne;
	QuadTree* sw;
	QuadTree* se;

	// Leaving for convenience to easily check if this is a leaf 
	// (if true, all children references are nullptr).
	bool isLeaf;

	// Entities inside this QuadTree.
	// NOTE: only contains entities if isLeaf==true.
	std::vector<Entity*> points;

	// Position of this QuadTree square (from top left).
	// @refactor: could make this an FRect.
	int x, y, width, height;

	// Used to determine "how deep" we are down the tree.
	// Maximum depth is specified by static DEPTH_LIMIT.
	int depth;

	// @todo: put depth of quadtree in constructor
	// instead of manually setting it LOL
	QuadTree(float x, float y, float width, float height);
	~QuadTree();

	// NOTE: its your job to delete the entity still after calling this.
	bool remove(Entity* entity);

	void insert(Entity* point);

	// For confirming if an entity is contained in this QuadTree.
	// (Note: only leafs will actually store references to the Entities
	// inside the points vector.)
	bool contains(Entity* point);

	// Obtain list of all leaf nodes which the provided entity is inside of.
	std::vector<QuadTree*> getLeafs(Entity* dot);

	// Returns a set containing tuple pairs of entity ids (in order),
	// where ids indicate which entities collided with each other.
	std::set<std::tuple<Uint16, Uint16>> getCollisionsWithEntity(Entity* entity);

	// AABB collision.
	bool checkCollision(Entity* entity1, Entity* entity2);

	// Legacy function from original demo; keeping for convenience.
	//void draw(Renderer* renderer);
};
