#pragma once
#include "../../pch.h"

class ULevelStreamingDynamic : public UObject
{
public:
    UCLASS_COMMON_MEMBERS(ULevelStreamingDynamic);

    DEFINE_STATIC_FUNC(LoadLevelInstanceBySoftObjectPtr, ULevelStreamingDynamic*);
};