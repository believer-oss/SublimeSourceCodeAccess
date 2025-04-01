#pragma once

#include "Modules/ModuleInterface.h"
#include "Templates/SharedPointer.h"

class FSublimeSourceCodeAccessor;

class FSublimeSourceCodeAccessModule : public IModuleInterface
{
public:
	FSublimeSourceCodeAccessModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FSublimeSourceCodeAccessor& GetAccessor();

private:

	TSharedRef<FSublimeSourceCodeAccessor> SourceCodeAccessor;
};
