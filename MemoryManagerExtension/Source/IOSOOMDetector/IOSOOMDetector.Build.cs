using System.IO;
using UnrealBuildTool;

public class IOSOOMDetector : ModuleRules
{
	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty")); }
	}

	public IOSOOMDetector(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));
			PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "libSeriaOOMDetector.a"));
		}

		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("Engine");
	}
}