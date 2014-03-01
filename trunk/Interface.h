/* Interface.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "Color.h"
#include "DataFile.h"
#include "Set.h"
#include "Sprite.h"
#include "Point.h"
#include "Information.h"

#include <string>
#include <vector>



// Class representing a user interface, specified in a data file and filled with
// the contents of an Information object.
class Interface {
public:
	void Load(const DataFile::Node &node, const Set<Color> &colors);
	
	void Draw(const Information &info) const;
	
	char OnClick(const Point &point) const;
	
	
private:
	class SpriteSpec {
	public:
		SpriteSpec(const std::string &str, const Point &position);
		SpriteSpec(const Sprite *sprite, const Point &position);
		
		std::string name;
		const Sprite *sprite;
		Point position;
		Point size;
		
		std::string condition;
	};
	
	class StringSpec {
	public:
		StringSpec(const std::string &str, const Point &position);
		
		std::string str;
		Point position;
		double align;
		int size;
		Color color;
		
		std::string condition;
	};
	
	class BarSpec {
	public:
		BarSpec(const std::string &name, const Point &position);
		
		std::string name;
		Point position;
		Point size;
		Color color;
		float width;
		
		std::string condition;
	};
	
	class ButtonSpec {
	public:
		ButtonSpec(char key, const Point &position);
		
		Point position;
		Point size;
		char key;
		
		mutable bool active;
		std::string condition;
	};
	
	class RadarSpec {
	public:
		RadarSpec(const Point &position = Point());
		
		Point position;
		double scale;
		double radius;
		double pointerRadius;
		
		std::string condition;
	};
	
	
private:
	Point position;
	
	std::vector<SpriteSpec> sprites;
	std::vector<SpriteSpec> outlines;
	
	std::vector<StringSpec> labels;
	std::vector<StringSpec> strings;
	
	std::vector<BarSpec> bars;
	std::vector<BarSpec> rings;
	
	std::vector<ButtonSpec> buttons;
	
	std::vector<RadarSpec> radars;
};



#endif
