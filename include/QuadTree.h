#pragma once
#include <SDL.h> // using only for certain structs
#include <vector>
#include <set>
#include "Projectile.h"

class Renderer;  // forward declaration that should only be needed for debugging quadtree.

class QuadTree {
public:

  // These settings are the best I've found so far.
  // LIMIT=4, DEPTH_LIMIT=5.

  // LIMIT: max count of rects in a QuadTree
  // DEPTH_LIMIT: maximum depth that an entire QuadTree can have.
  static int LIMIT, DEPTH_LIMIT;

	// Pointers to all children of this 'node'. 
	QuadTree* nw;
	QuadTree* ne;
	QuadTree* sw;
	QuadTree* se;

	// Leaving for convenience to easily check if this is a leaf 
  // (and thus, all children references are nullptr).
	bool isLeaf;

  // Entities inside this QuadTree.
  // NOTE: only contains entities if isLeaf==true.
	std::vector<Projectile*> points;

	// Position of this QuadTree square (from top left).
	int x, y, width, height;

  // Used to determine "how deep" we are down the tree.
  // Maximum depth is specified by static DEPTH_LIMIT.
  int depth;

	QuadTree(float x, float y, float width, float height);
	~QuadTree();

	void insert(Projectile* point);

	// For confirming if a point belongs in this QuadTree (square).
  // TODO: should probably change this to be 'contains' or something
  // less stupid.
	bool insideOf(Projectile* point);

	// Will draw this entire QuadTree and all of its children.
	// (Will also draw the points).
	//void draw(SDL_Renderer* renderer);
	void draw(Renderer* renderer);

	//void update();

  // Obtain list of all leaf nodes which thie provided entity is inside of.
  std::vector<QuadTree*> getLeafs(Projectile* dot);

  // Counts amount of collisions with an entity. 
  // Will increment collisions/comparisons as the entities are checked.
  void getCollisionsWithEntity(Projectile* entity, int* collisions, int* comparisons);

  // AABB collision.
  bool checkCollision(Projectile* entity1, Projectile* entity2);
};
