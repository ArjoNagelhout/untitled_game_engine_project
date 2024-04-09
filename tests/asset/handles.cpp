//
// Created by Arjo Nagelhout on 08/04/2024.
//


#include <gtest/gtest.h>

#include <renderer/mesh.h>
#include <scene/scene.h>
#include <asset/asset_handle.h>

namespace handles_test
{
    struct AssetType1
    {

    };

    struct AssetType2
    {

    };

    struct AssetType3
    {

    };

    TEST(Asset, Handles)
    {
        asset::Asset<AssetType1> a = asset::makeAsset<AssetType1>(asset::AssetId{});
        asset::Asset<AssetType2> b = asset::makeAsset<AssetType2>(asset::AssetId{});
        asset::Asset<AssetType3> c = asset::makeAsset<AssetType3>(asset::AssetId{});


    }
}