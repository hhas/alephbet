#ifndef _INFO_TREE_
#define _INFO_TREE_

/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

/*
 *  Boost.PropertyTree-based structured-data reader and writer
 */

#include "FileHandler.hpp"
#include "FontHandler.hpp"
#include "cseries.hpp"
#include "map.hpp"
#include "world.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/any_range.hpp>
#include <sstream>
#include <string>

class InfoTree : public boost::property_tree::iptree {
  public:

    typedef boost::property_tree::xml_parser_error parse_error;
    typedef boost::property_tree::ini_parser_error ini_error;
    typedef boost::property_tree::ptree_bad_path path_error;
    typedef boost::property_tree::ptree_bad_data data_error;
    typedef boost::property_tree::ptree_error unexpected_error;

    InfoTree() {}

    explicit InfoTree(const data_type& data) : boost::property_tree::iptree(data) {}

    InfoTree(const boost::property_tree::iptree& rhs) : boost::property_tree::iptree(rhs) {}

    static InfoTree load_xml(FileSpecifier filename);
    static InfoTree load_xml(std::istringstream& stream);
    void save_xml(FileSpecifier filename) const;
    void save_xml(std::ostringstream& stream) const;

    static InfoTree load_ini(FileSpecifier filename);
    static InfoTree load_ini(std::istringstream& stream);
    void save_ini(FileSpecifier filename) const;
    void save_ini(std::ostringstream& stream) const;

    template <typename T>
    bool read(std::string path, T& value) const {
        try {
            value = get_child(path).get_value<T>();
            return true;
        } catch (const path_error& ep) {
        } catch (const data_error& ed) {}
        return false;
    }

    template <typename T>
    bool read_attr(std::string path, T& value) const {
        return read(std::string("<xmlattr>.") + path, value);
    }

    template <typename T>
    bool read_attr_bounded(std::string path, T& value, const T min, const T max) const {
        T temp;
        if (read_attr(path, temp) && temp >= min && temp <= max) {
            value = temp;
            return true;
        }
        return false;
    }

    bool read_indexed(std::string path, int16& value, int num_slots, bool allow_none = false) const {
        return read_attr_bounded<int16>(path, value, (allow_none ? NONE : 0), num_slots - 1);
    }

    template <typename T>
    void put_attr(std::string path, const T value) {
        put(std::string("<xmlattr>.") + path, value);
    }

    bool read_color(RGBColor& color) const;
    bool read_color(rgb_color& color) const;
    bool read_shape(shape_descriptor& descriptor, bool allow_empty = true) const;
    bool read_damage(damage_definition& definition) const;
    bool read_font(FontSpecifier& font) const;

    bool read_path(std::string key, FileSpecifier& file) const;
    bool read_path(std::string key, char* dest) const;
    bool read_cstr(std::string key, char* dest, int maxlen) const;
    bool read_fixed(std::string key, _fixed& value, float min = -SHRT_MAX, float max = SHRT_MAX) const;
    bool read_wu(std::string key, short& value, float min = -64, float max = 64) const;
    bool read_angle(std::string key, angle& value) const;

    void add_color(std::string path, const RGBColor& color);
    void add_color(std::string path, const RGBColor& color, size_t index);
    void add_color(std::string path, const rgb_color& color);
    void add_color(std::string path, const rgb_color& color, size_t index);

    void put_attr_path(std::string path, std::string filepath);
    void put_cstr(std::string path, std::string cstr);
    void put_attr_cstr(std::string path, std::string cstr);

    typedef boost::any_range<const InfoTree, boost::forward_traversal_tag, const InfoTree, std::ptrdiff_t>
            const_child_range;
    const_child_range children_named(std::string key) const;
};

#endif
