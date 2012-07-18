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

#ifndef STREAMER_H
#define STREAMER_H

#define INVALID_ALTERNATE_ID (-1)
#define INVALID_GENERIC_ID (0xFFFF)

class Streamer
{
public:
	Streamer();

	bool setTickRate(std::size_t value);

	std::size_t getVisibleItems(int type);
	bool setVisibleItems(int type, std::size_t value);

	void processActiveItems();
	void startAutomaticUpdate();
	void startManualUpdate(StreamerPlayer &player);

	boost::unordered_set<Element::SharedArea> attachedAreas;
	boost::unordered_set<Element::SharedObject> attachedObjects;
	boost::unordered_set<Element::SharedTextLabel> attachedTextLabels;
	boost::unordered_set<Element::SharedObject> movingObjects;

	boost::unordered_map<int, int> internalPickups;
private:
	void calculateAverageUpdateTime();

	void performPlayerUpdate(StreamerPlayer &player, bool automatic);

	void executeCallbacks(const std::multimap<bool, boost::tuple<int, int> > &areaCallbacks);
	void executeCallbacks(const std::vector<int> &objectCallbacks);

	void processAreas(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processCheckpoints(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processMapIcons(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processObjects(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processPickups(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processRaceCheckpoints(StreamerPlayer &player, const std::vector<SharedCell> &cells);
	void processTextLabels(StreamerPlayer &player, const std::vector<SharedCell> &cells);

	void processMovingObjects();
	void processAttachedAreas();
	void processAttachedObjects();
	void processAttachedTextLabels();

	std::size_t tickCount;
	std::size_t tickRate;

	boost::tuple<float, float> velocityBoundaries;

	std::size_t visibleMapIcons;
	std::size_t visibleObjects;
	std::size_t visiblePickups;
	std::size_t visibleTextLabels;

	float averageUpdateTime;
	bool processingFinalPlayer;

	template<std::size_t N>
	inline bool checkPlayer(const std::bitset<N> &a, const int b, const boost::unordered_set<int> &c, const int d, const boost::unordered_set<int> &e, const int f)
	{
		return (a[b] && (c.empty() || c.find(d) != c.end()) && (e.empty() || e.find(f) != e.end()));
	}

	template<class Iterator, class Container>
	bool isLastPlayer(Iterator iterator, const Container &container)
	{
		return ((iterator != container.end()) && (++iterator == container.end()));
	}
};

#endif
