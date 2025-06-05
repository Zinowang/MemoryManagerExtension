using UnrealBuildTool;

public class MemoryModuleControlCenter : ModuleRules
{
	public MemoryModuleControlCenter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"ObjectLifeTimeListener",
			}
		);
	}
}