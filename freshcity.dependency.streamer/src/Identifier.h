/*
    SA-MP Streamer Plugin v2.6.1
    Copyright ?2012 Incognito

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IDENTIFIER_H
#define IDENTIFIER_H

class Identifier
{
public:
	Identifier();

	int get();
	void remove(int id, std::size_t remaining);
	void reset();
private:
	int highestID;

	std::priority_queue<int, std::vector<int>, std::greater<int> > removedIDs;
};

#endif
