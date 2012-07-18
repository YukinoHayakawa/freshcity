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

class Cell;
class Data;
class Identifier;
class Grid;
class Streamer;

struct StreamerPlayer;

typedef std::pair<int, int> CellID;
typedef boost::intrusive_ptr<Cell> SharedCell;

namespace Element
{
	struct Area;
	struct Checkpoint;
	struct MapIcon;
	struct StreamerObject;
	struct Pickup;
	struct RaceCheckpoint;
	struct TextLabel;

	typedef boost::intrusive_ptr<Area> SharedArea;
	typedef boost::intrusive_ptr<Checkpoint> SharedCheckpoint;
	typedef boost::intrusive_ptr<MapIcon> SharedMapIcon;
	typedef boost::intrusive_ptr<StreamerObject> SharedObject;
	typedef boost::intrusive_ptr<Pickup> SharedPickup;
	typedef boost::intrusive_ptr<RaceCheckpoint> SharedRaceCheckpoint;
	typedef boost::intrusive_ptr<TextLabel> SharedTextLabel;

	typedef boost::geometry::model::box<Eigen::Vector2f> Box2D;
	typedef boost::geometry::model::box<Eigen::Vector3f> Box3D;
	typedef boost::tuple<boost::geometry::model::polygon<Eigen::Vector2f>, Eigen::Vector2f> Polygon2D;
}

namespace boost { namespace geometry { namespace traits
{
	template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	struct tag<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
	{
		typedef point_tag type;
	};

	template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	struct coordinate_type<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
	{
		typedef _Scalar type;
	};

	template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	struct coordinate_system<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
	{
		typedef cs::cartesian type;
	};

	template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	struct dimension<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> > : boost::mpl::int_<_Rows> {};

	template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols, std::size_t Dimension>
	struct access<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>, Dimension>
	{
		static inline _Scalar get(Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> const &matrix)
		{
			return matrix[Dimension];
		}

		static inline void set(Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &matrix, _Scalar const &value)
		{
			matrix[Dimension] = value;
		}
	};
}}}
