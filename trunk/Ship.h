/* Ship.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef SHIP_H_
#define SHIP_H_

#include "Angle.h"
#include "Animation.h"
#include "Armament.h"
#include "Controllable.h"
#include "DataFile.h"
#include "Outfit.h"
#include "Point.h"

#include <ostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class GameData;
class Government;
class Planet;
class Projectile;
class System;



// Class representing a ship (either a model for sale or an instance of it).
class Ship : public Controllable {
public:
	// Default constructor.
	Ship();
	
	// Load data for a type of ship:
	void Load(const DataFile::Node &node, const GameData &data);
	// When loading a ship, some of the outfits it lists may not have been
	// loaded yet. So, wait until everything has been loaded, then call this.
	void FinishLoading();
	// Save a full description of this ship, as currently configured.
	void Save(std::ostream &out) const;
	
	// Get the name of this model of ship.
	const std::string &ModelName() const;
	// Get this ship's description.
	const std::string &Description() const;
	// Get this ship's cost.
	int Cost() const;
	
	// When creating a new ship, you must set the following:
	void Place(Point position = Point(), Point velocity = Point(), Angle angle = Angle());
	void SetName(const std::string &name);
	void SetSystem(const System *system);
	void SetPlanet(const Planet *planet);
	void SetGovernment(const Government *government);
	void SetIsSpecial(bool special = true);
	
	// Move this ship. A ship may create effects as it moves, in particular if
	// it is in the process of blowing up. If this returns false, the ship
	// should be deleted.
	bool Move(std::list<Effect> &effects);
	// Launch any ships that are ready to launch.
	void Launch(std::list<std::shared_ptr<Ship>> &ships);
	
	// Fire any weapons that are ready to fire. If an anti-missile is ready,
	// instead of firing here this function returns true and it can be fired if
	// collision detection finds a missile in range.
	bool Fire(std::list<Projectile> &projectiles);
	// Fire an anti-missile. Returns true if the missile was killed.
	bool FireAntiMissile(const Projectile &projectile, std::list<Effect> &effects);
	
	// Get the system this ship is in.
	const System *GetSystem() const;
	// If the ship is landed, get the planet it has landed on.
	const Planet *GetPlanet() const;
	
	bool IsTargetable() const;
	bool IsDisabled() const;
	bool IsFullyDisabled() const;
	bool IsLanding() const;
	bool IsHyperspacing() const;
	// Check if this ship is currently able to begin landing on its target.
	bool CanLand() const;
	// Check if this ship is currently able to enter hyperspace to it target.
	bool CanHyperspace() const;
	
	// Get information on this particular ship, for displaying it.
	const Animation &GetSprite() const;
	// Get the ship's government.
	const Government *GetGovernment() const;
	// Get this ship's zoom factor (due to landing on a planet).
	double Zoom() const;
	// Get the name of this particular ship.
	const std::string &Name() const;
	
	// Get the points from which engine flares should be drawn. If the ship is
	// not thrusting right now, this will be empty.
	const std::vector<Point> &EnginePoints() const;
	// Get the sprite to be used for an engine flare.
	const Animation &FlareSprite() const;
	
	// Get the position of this ship.
	const Point &Position() const;
	const Point &Velocity() const;
	const Angle &Facing() const;
	// Get the facing unit vector times the scale factor.
	Point Unit() const;
	
	// Recharge and repair this ship (e.g. because it has landed).
	void Recharge();
	
	// Get characteristics of this ship, as a fraction between 0 and 1.
	double Shields() const;
	double Hull() const;
	double Fuel() const;
	int JumpsRemaining() const;
	double Energy() const;
	double Heat() const;
	// TODO: track crew on all ships.
	int Crew() const;
	// Check if this ship should be deleted.
	bool ShouldDelete() const;
	
	// Get this ship's movement characteristics.
	double Mass() const;
	double TurnRate() const;
	double Acceleration() const;
	double MaxVelocity() const;
	
	// This ship just got hit by the given projectile. Take damage according to
	// what sort of weapon the projectile it.
	void TakeDamage(const Projectile &projectile);
	// Apply a force to this ship, accelerating it. This might be from a weapon
	// impact, or from firing a weapon, for example.
	void ApplyForce(const Point &force);
	
	// Get cargo information.
	const std::map<std::string, int> &Cargo() const;
	int Cargo(const std::string &type) const;
	int FreeCargo() const;
	int AddCargo(int tons, const std::string &type);
	
	// Get outfit information.
	const std::map<const Outfit *, int> &Outfits() const;
	int OutfitCount(const Outfit *outfit) const;
	const Outfit &Attributes() const;
	// Get the attributes of this ship chassis before any outfits were added.
	const Outfit &BaseAttributes() const;
	// Add or remove outfits. (To remove, pass a negative number.)
	void AddOutfit(const Outfit *outfit, int count);
	
	// Get the list of weapons.
	const std::vector<Armament::Weapon> &Weapons() const;
	// Check if we are able to fire the given weapon (i.e. there is enough
	// energy, ammo, and fuel to fire it).
	bool CanFire(const Outfit *outfit);
	// Fire the given weapon (i.e. deduct whatever energy, ammo, or fuel it uses
	// and add whatever heat it generates. Assume that CanFire() is true.
	void ExpendAmmo(const Outfit *outfit);
	
	
private:
	void CreateExplosion(std::list<Effect> &effects);
	
	
private:
	// Characteristics of the chassis:
	std::string modelName;
	std::string description;
	Animation sprite;
	// Characteristics of this particular ship:
	std::string name;
	const Government *government;
	
	bool isInSystem;
	int forget;
	// "Special" ships cannot be forgotten, and if they land on a planet, they
	// continue to exist and refuel instead of being deleted.
	bool isSpecial;
	bool isOverheated;
	bool isDisabled;
	
	// Installed outfits, cargo, etc.:
	Outfit attributes;
	Outfit baseAttributes;
	const Outfit *explosionWeapon;
	std::map<const Outfit *, int> outfits;
	std::map<std::string, int> cargo;
	int cargoMass;
	// TODO: spare outfits (from plunder or previous ship when selling).
	
	std::vector<Point> enginePoints;
	Armament armament;
	
	// Various energy levels:
	double shields;
	double hull;
	double fuel;
	double energy;
	double heat;
	
	// Current status of this particular ship:
	const System *currentSystem;
	Point position;
	Point velocity;
	Angle angle;
	
	// A Ship can be locked into one of three special states: landing,
	// hyperspacing, and exploding. Each one must track some special counters:
	double zoom;
	const Planet *landingPlanet;
	
	int hyperspaceCount;
	const System *hyperspaceSystem;
	
	std::map<const Effect *, int> explosionEffects;
	int explosionRate;
	int explosionCount;
	int explosionTotal;
};



#endif
