/* Outfit.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef OUTFIT_H_
#define OUTFIT_H_

#include "Animation.h"
#include "DataFile.h"
#include "Effect.h"
#include "Set.h"

#include <map>
#include <string>



// Class representing an outfit that can be installed in a ship.
class Outfit {
public:
	Outfit();
	
	// An "outfit" can be loaded from an "outfit" node or from a ship's
	// "attributes" node.
	void Load(const DataFile::Node &node, const Set<Outfit> &outfits, const Set<Effect> &effects);
	
	const std::string &Name() const;
	const std::string &Category() const;
	const std::string &Description() const;
	int Cost() const;
	// Get the image to display in the outfitter when buying this item.
	const Sprite *Thumbnail() const;
	
	double Get(const std::string &attribute) const;
	const std::map<std::string, double> &Attributes() const;
	
	// Determine whether the given number of instances of the given outfit can
	// be added to a ship with the attributes represented by this instance. If
	// not, return the maximum number that can be added.
	int CanAdd(const Outfit &other, int count = 1) const;
	// For tracking a combination of outfits in a ship: add the given number of
	// instances of the given outfit to this outfit.
	void Add(const Outfit &other, int count = 1);
	// Modify this outfit's attributes.
	void Add(const std::string &attribute, double value = 1.);
	void Reset(const std::string &attribute, double value = 0.);
	
	// Get this outfit's engine flare sprite, if any.
	const Animation &FlareSprite() const;
	
	// Weapon attributes.
	
	// Check if this is a weapon.
	bool IsWeapon() const;
	// Get the weapon provided by this outfit, if any.
	const Animation &WeaponSprite() const;
	const Outfit *Ammo() const;
	const Sprite *Icon() const;
	double WeaponGet(const std::string &attribute) const;
	
	// TODO: also check if this is a gun or a turret.
	
	// Handle a weapon impacting something or reaching its end of life.
	const std::map<const Effect *, int> HitEffects() const;
	const std::map<const Effect *, int> DieEffects() const;
	const std::map<const Outfit *, int> Submunitions() const;
	
	
private:
	std::string name;
	std::string category;
	std::string description;
	const Sprite *thumbnail;
	
	std::map<std::string, double> attributes;
	
	Animation flare;
	Animation weaponSprite;
	const Outfit *ammo;
	const Sprite *icon;
	// Die and hit effects.
	std::map<const Effect *, int> hitEffects;
	std::map<const Effect *, int> dieEffects;
	std::map<const Outfit *, int> submunitions;
	
	std::map<std::string, double> weapon;
};



#endif