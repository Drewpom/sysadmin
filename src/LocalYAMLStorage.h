
#pragma once

#include "CommitHistory.h"
#include "ConfigTypes.h"
#include "IConfigurator.h"
#include "LockedConfigPairMap.h"

#include "sysadmin-api/sysadminctl.pb.h"

#include <boost/filesystem.hpp>

#include <yaml-cpp/yaml.h>

#define DYNAMIC_UNPACK(value, storage)                      \
    if (value.Is<int32_t>())                                \
    {                                                       \
        storage = value.Unpack<int32_t>();                  \
    }                                                       \
    else if (value.Is<std::string>())                       \
    {                                                       \
        storage = value.Unpack<std::string>();              \
    }                                                       \
    else if (value.Is<bool>())                              \
    {                                                       \
        storage = value.Unpack<bool>();                     \
    }                                                       \
    else if (value.Is<std::vector<int32_t>>())              \
    {                                                       \
        storage = value.Unpack<std::vector<int32_t>>();     \
    }                                                       \
    else if (value.Is<std::vector<std::string>>())          \
    {                                                       \
        storage = value.Unpack<std::vector<std::string>>(); \
    }                                                       \
    else if (value.Is<std::vector<bool>>())                 \
    {                                                       \
        storage = value.Unpack<std::vector<bool>>();        \
    }

std::pair<boost::filesystem::path, std::string> TranslateKeyToPathPair(const ConfigPair::Key& key);

std::string TranslatePathToKey(const boost::filesystem::path& rootpath,
                               boost::filesystem::path valuepath,
                               const std::string& bottomLevelKey);

ConfigValue ValueFromYAML(const std::string& type, const YAML::Node& node);

YAML::Node UnpackIntoYAML(const ConfigPair& value);

class LocalYAMLStorage : public IConfigurator
{
public:
    LocalYAMLStorage(const std::string& root);

    LocalYAMLStorage(const boost::filesystem::path& root);

    virtual ~LocalYAMLStorage();

public: // IConfigurator
    IConfigurator::SetStatus Set(const ConfigPair& item, decibel::messaging::Protocol::Id clientId);

    ConfigPairMap Get(const ConfigPair::Key& key) const;

    bool TypeCheck(const ConfigPair& pair) const;

    ConfigPairMap GetAll() const;

    std::vector<ConfigPair::Key> GetAllKeys(const ConfigPair::Key& ns) const;

    folly::Future<CommitHistory::CommitId>
    Commit(size_t id,
           sysadminctl::CommitConfig config_opt = sysadminctl::CommitConfig::DEFAULT);

    void Drop(size_t id);

    void Erase(const ConfigPair::Key& key, size_t id);

    ConfigPairList InFlightItems(size_t id);

public: // This method is effectively private given this class is only accessed through an interface
    void GroupSet(const ConfigPairList& items);

private:
    boost::optional<ConfigPair> InternalGet(const ConfigPair::Key& key) const;

private:
    boost::filesystem::path mRoot;
    LockedConfigPairMap mInFlightItems;
};
