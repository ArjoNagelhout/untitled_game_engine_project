//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include "source_directory.h"

#include <iostream>

namespace fs = std::filesystem;
using namespace assets;

#include "nlohmann/json.hpp"

using namespace nlohmann;

#include "reflection/type_info.h"

using namespace reflection;

struct MeshImportSettings
{
    bool doSomething = false;
    bool somethingElse = true;
    float someValue = 1324.0f;
    int otherValue = 98;

    json serialize()
    {
        json j = json::object();
        j["doSomething"] = doSomething;
        j["somethingElse"] = somethingElse;
        j["someValue"] = someValue;
        j["otherValue"] = otherValue;
        return j;
    }

    void deserialize(std::string const& json)
    {
        auto j = json::parse(json);
        doSomething = j["doSomething"];
        somethingElse = j["somethingElse"];
        someValue = j["someValue"];
        otherValue = j["otherValue"];
    }
};

int main(int argc, char* argv[])
{
    fs::path sourceDirectoryPath(argv[1]);
    fs::path cacheDirectoryPath;

    SourceDirectory src(sourceDirectoryPath, cacheDirectoryPath);

    TypeInfoRegistry r;

    TypeInfo info = TypeInfoBuilder<MeshImportSettings>("MeshImportSettings")
        .addProperty<&MeshImportSettings::doSomething>("doSomething")
        .addProperty<&MeshImportSettings::somethingElse>("somethingElse")
        .addProperty<&MeshImportSettings::someValue>("someValue")
        .addProperty<&MeshImportSettings::otherValue>("otherValue")
        .build();
    r.add<MeshImportSettings>(std::move(info));

    TypeInfo& storedInfo = r.get<MeshImportSettings>();

    for (auto& property: storedInfo.properties)
    {
        std::cout << property.name << std::endl;
    }

    return 0;
}