#pragma once

#include "SublimeSourceCodeSettings.generated.h"

UCLASS(config=EditorPerProjectUserSettings, MinimalAPI)
class USublimeSourceCodeSettings : public UObject
{
	GENERATED_BODY()

public:
	USublimeSourceCodeSettings();

	// Path to the sublime text executable on your machine
	UPROPERTY(EditAnywhere, Config)
	FString SublimeExecutablePath;
};
