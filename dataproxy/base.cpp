#include "common.h"
#define FCEXPORTIMPL
#include "../common/exception.h"
#include "../common/logging.h"
#include "config.h"
#include "db.h"
#include "basemembsers.h"

BaseObject::BaseObject(const std::string& collection, const std::string& uniqueid)
	: _collection(collection), _uniqueid(uniqueid), _members(new BaseObject::Members) {
		Synchronize();
}

BaseObject::BaseObject(const std::string& uniqueid)
	: _collection(GetConfig().GetAttribute("Database.undefined").ToString()), _uniqueid(uniqueid),_members(new BaseObject::Members) {
		Synchronize();
}

void BaseObject::SetAttribute(const std::string& key, const AttributeElement& value) {
	_members->set[key] = Members::ElementPtr(new AttributeElement(value));
}

void BaseObject::SetAttributeIncrease(const std::string& key, double value) {
	_members->increase[key] = value;
}

void BaseObject::RemoveAttribute(const std::string& key) {
	_members->remove.push_back(key);
}

AttributeElement BaseObject::GetAttribute(const std::string& key) const {
	try {
		return *_members->loaded.at(key).get();
	} catch(...) {
		LOGERROR("不存在的键值 " + key);
		throw FCException("尝试查找并获取不存在的值");
	}
}

void BaseObject::Synchronize() {
	mongo::BSONObjBuilder set;
	mongo::BSONObjBuilder remove;
	mongo::BSONObjBuilder increase;
	bool emptyset = _members->set.empty();
	bool emptyincrease = _members->increase.empty();
	bool emptyremove = _members->remove.empty();

	// Set
	if(!emptyset) {
		for(Members::ElementBuffer::iterator mapiter = _members->set.begin(); mapiter != _members->set.end(); mapiter++) {
			switch(mapiter->second->GetValueType()) {
			case AttributeElement::Type::STRING:
				set.append("attribute." + mapiter->first, mapiter->second->ToString());
				break;

			case AttributeElement::Type::NUMBER:
				set.appendNumber("attribute." + mapiter->first, mapiter->second->ToNumber());
				break;

			case AttributeElement::Type::BOOL:
				set.appendBool("attribute." + mapiter->first, mapiter->second->ToBool());
				break;
			
			default:
				break;
			}
		}
		_members->set.clear();
	}

	// Increase
	if(!emptyincrease) {
		for(Members::IncreaseBuffer::iterator mapiter = _members->increase.begin(); mapiter != _members->increase.end(); mapiter++) {
			increase.appendNumber("attribute." + mapiter->first, mapiter->second);
		}
		_members->increase.clear();
	}

	// Remove
	if(!emptyremove) {
		for(Members::RemoveList::iterator iter = _members->remove.begin(); iter != _members->remove.end(); iter++) {
			remove.appendNumber("attribute." + *iter, 1);
		}
		_members->remove.clear();
	}

	try {
		if(!(emptyset && emptyincrease && emptyremove)) {
			DBInstance::GetDB().update(_collection, BSON("_id" << mongo::OID(_uniqueid)),
				BSON("$set" << set.obj() <<
				"$unset" << remove.obj() <<
				"$inc" << increase.obj()));
		}

		_members->rawdata = DBInstance::GetDB().findOne(_collection, BSON("_id" << mongo::OID(_uniqueid)));

		if(_members->rawdata.isEmpty()) {
			LOGERROR("指定对象 " + _collection + " ( ObjectID: " + _uniqueid + " ) 不存在");
			throw FCException("尝试获取不存在的对象");
		}

		if(_members->rawdata.hasField("attribute")) {
			mongo::BSONObj attributes = _members->rawdata.getObjectField("attribute");
			_members->loaded.clear();
			for(mongo::BSONObjIterator iter(attributes); iter.more();) {
				mongo::BSONElement e = iter.next();
				switch(e.type()) {
				case mongo::BSONType::NumberDouble:
					_members->loaded.insert(std::make_pair(e.fieldName(), Members::ElementPtr(new AttributeElement(e.Number()))));
					break;

				case mongo::BSONType::Bool:
					_members->loaded.insert(std::make_pair(e.fieldName(), Members::ElementPtr(new AttributeElement(e.Bool()))));
					break;

				case mongo::BSONType::String:
					_members->loaded.insert(std::make_pair(e.fieldName(), Members::ElementPtr(new AttributeElement(e.String()))));
					break;
				}
			}
		}
	} catch(mongo::UserException& e) {
		LOGERROR("同步对象数据时发生错误: " + e.toString());
		throw FCException("无法同步对象数据");
	}

	return;
}
