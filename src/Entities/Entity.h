/*
 * Entity.h
 *
 *  Created on: Apr 2, 2017
 *      Author: user
 */

#ifndef SRC_TOOLS_ENTITY_H_
#define SRC_TOOLS_ENTITY_H_

class Entity {
private:
	int x, y;
	float yaw;
	
public:
	Entity();

	int getX();
	int getY();
	int getYaw();
	
	virtual ~Entity();
};

#endif /* SRC_TOOLS_ENTITY_H_ */
