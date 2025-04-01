using System;
using Microsoft.Win32;

namespace UnrealBuildTool.Rules
{
	public class SublimeSourceCodeAccess : ModuleRules
	{
        public SublimeSourceCodeAccess(ReadOnlyTargetRules Target) : base(Target)
		{
            PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"SourceCodeAccess",
				}
			);
		}
	}
}
