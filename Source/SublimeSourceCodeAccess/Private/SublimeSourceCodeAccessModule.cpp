#include "SublimeSourceCodeAccessModule.h"

#include "Modules/ModuleManager.h"
#include "Features/IModularFeatures.h"
#include "SublimeSourceCodeAccessor.h"

IMPLEMENT_MODULE( FSublimeSourceCodeAccessModule, SublimeSourceCodeAccess );

FSublimeSourceCodeAccessModule::FSublimeSourceCodeAccessModule()
	: SourceCodeAccessor(MakeShareable(new FSublimeSourceCodeAccessor()))
{
}

void FSublimeSourceCodeAccessModule::StartupModule()
{
	SourceCodeAccessor->Startup();

	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &SourceCodeAccessor.Get() );
}

void FSublimeSourceCodeAccessModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &SourceCodeAccessor.Get());

	SourceCodeAccessor->Shutdown();
}

FSublimeSourceCodeAccessor& FSublimeSourceCodeAccessModule::GetAccessor()
{
	return SourceCodeAccessor.Get();
}
