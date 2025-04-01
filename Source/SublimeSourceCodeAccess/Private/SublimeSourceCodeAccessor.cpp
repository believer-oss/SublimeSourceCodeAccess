#include "SublimeSourceCodeAccessor.h"
#include "Misc/Paths.h"
#include "Misc/UProjectInfo.h"

#define LOCTEXT_NAMESPACE "SublimeSourceCodeAccessor"

static const TCHAR* GSublimeWorkspaceExtension = TEXT(".sln");
static const TCHAR* GSublimeDefaultExePath = TEXT("C:\\Program Files\\Sublime Text\\subl.exe");

static FString GetSublimeExecutablePath()
{
	const USublimeSourceCodeSettings* Settings = GetDefault<USublimeSourceCodeSettings>();
	if (IsValid(Settings))
	{
		return Settings->SublimeExecutablePath;
	}

	return GSublimeDefaultExePath;
}

USublimeSourceCodeSettings::USublimeSourceCodeSettings()
	: SublimeExecutablePath(GSublimeDefaultExePath)
{
}

const FString& FSublimeSourceCodeAccessor::GetSolutionPath() const
{
	FScopeLock Lock(&CachedSolutionPathCriticalSection);

	if (CachedSolutionPath.IsEmpty() && IsInGameThread())
	{
		CachedSolutionPath = FPaths::ProjectDir();

		if (!FUProjectDictionary::GetDefault().IsForeignProject(CachedSolutionPath))
		{
			CachedSolutionPath = FPaths::Combine(FPaths::RootDir(), FString("UE5") + GSublimeWorkspaceExtension);
		}
		else
		{
			FString BaseName = FApp::HasProjectName() ? FApp::GetProjectName() : FPaths::GetBaseFilename(CachedSolutionPath);
			CachedSolutionPath = FPaths::Combine(CachedSolutionPath, BaseName + GSublimeWorkspaceExtension);
		}
	}

	return CachedSolutionPath;
}

void FSublimeSourceCodeAccessor::Startup()
{
	GetSolutionPath();
}

void FSublimeSourceCodeAccessor::Shutdown()
{
}

bool FSublimeSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	if (CanAccessSourceCode())
	{
		return Launch(AbsoluteSourcePaths);
	}

	return false;
}

bool FSublimeSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
	return true;
}

bool FSublimeSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	if (CanAccessSourceCode())
	{
		LineNumber = FMath::Max(LineNumber, 0);
		ColumnNumber = FMath::Max(ColumnNumber, 0);

		TArray<FString> Args;
		Args.Add(FString::Printf(TEXT("%s:%d:%d"), *FullPath, LineNumber, ColumnNumber));
		return Launch(Args);
	}

	return false;
}

void FSublimeSourceCodeAccessor::RefreshAvailability()
{
}

bool FSublimeSourceCodeAccessor::CanAccessSourceCode() const
{
	const FString Path = GetSublimeExecutablePath();
	const bool bAppExists = FPaths::FileExists(Path);
	return bAppExists;
}

FName FSublimeSourceCodeAccessor::GetFName() const
{
	return FName("Sublime Text");
}

FText FSublimeSourceCodeAccessor::GetNameText() const
{
	return LOCTEXT("SublimeDisplayName", "Sublime Text");
}

FText FSublimeSourceCodeAccessor::GetDescriptionText() const
{
	return LOCTEXT("SublimeDisplayDesc", "Open source code files in Sublime Text");
}

void FSublimeSourceCodeAccessor::Tick(const float DeltaTime)
{
}

bool FSublimeSourceCodeAccessor::OpenSolution()
{
	if (CanAccessSourceCode())
	{
		return OpenSolutionAtPath(GetSolutionPath());
	}

	return false;
}

bool FSublimeSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
	if (CanAccessSourceCode())
	{
		FString SolutionPath = InSolutionPath;

		if (!SolutionPath.EndsWith(GSublimeWorkspaceExtension))
		{
			SolutionPath = SolutionPath + GSublimeWorkspaceExtension;
		}

		TArray<FString> Args;
		Args.Add(SolutionPath);
		return Launch(Args);
	}

	return false;
}

bool FSublimeSourceCodeAccessor::DoesSolutionExist() const
{
	return FPaths::FileExists(GetSolutionPath());
}

bool FSublimeSourceCodeAccessor::SaveAllOpenDocuments() const
{
	return true;
}

bool FSublimeSourceCodeAccessor::Launch(const TArray<FString>& InArgs)
{
	if (CanAccessSourceCode())
	{
		FString ArgsString;
		for (const FString& Arg : InArgs)
		{
			ArgsString.Append(Arg);
			ArgsString.Append(TEXT(" "));
		}

		const FString ApplicationFilePath = GetSublimeExecutablePath();

		uint32 ProcessID;
		FProcHandle hProcess = FPlatformProcess::CreateProc(*ApplicationFilePath, *ArgsString, true, false, false, &ProcessID, 0, nullptr, nullptr, nullptr);
		return hProcess.IsValid();
	}
	
	return false;
}

#undef LOCTEXT_NAMESPACE
