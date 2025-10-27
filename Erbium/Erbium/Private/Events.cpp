#include "pch.h"
#include "../Public/Events.h"
#include <thread>
#include "../../FortniteGame/Public/FortGameModeAthena.h"

void Events::StartEvent()
{
	if (VersionInfo.FortniteVersion < 4.4) return; // no other events from what i know of?


	auto GameMode = (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
	auto Playlist = VersionInfo.FortniteVersion >= 4.0 ? GameMode->GameState->HasCurrentPlaylistInfo() ? GameMode->GameState->CurrentPlaylistInfo.BasePlaylist : GameMode->GameState->CurrentPlaylistData : nullptr;

	for (auto& Event : EventsArray)
	{
		if (Event.EventVersion != VersionInfo.FortniteVersion)
			continue;

		UObject* LoaderObject = nullptr;
		if (Event.LoaderClass)
			if (const UClass* LoaderClass = FindObject<UClass>(Event.LoaderClass))
			{
				auto AllLoaders = Utils::GetAll(LoaderClass);
				LoaderObject = AllLoaders.Num() > 0 ? AllLoaders[0] : nullptr;
			}

		UObject* ScriptingObject = nullptr;
		if (Event.ScriptingClass)
			if (const UClass* ScriptingClass = FindObject<UClass>(Event.ScriptingClass))
			{
				auto AllLoaders = Utils::GetAll(ScriptingClass);
				ScriptingObject = AllLoaders.Num() > 0 ? AllLoaders[0] : nullptr;
			}

		for (auto& EventFunction : Event.EventFunctions)
		{
			const UFunction* Function = FindObject<UFunction>(EventFunction.FunctionPath);
			if (!Function)
			{
				printf("[Events] failed to find func: %ls\n", EventFunction.FunctionPath);
				continue;
			}

			UObject* Target = EventFunction.bIsLoaderFunction ? LoaderObject : ScriptingObject;
			if (Target)
			{
				if (wcsstr(EventFunction.FunctionPath, L"OnReady"))
				{
					struct { UObject* GameState; const UFortPlaylistAthena* Playlist; FGameplayTagContainer PlaylistContextTags; }
					Params{ GameMode->GameState, Playlist, Playlist && Playlist->HasGameplayTagContainer() ? Playlist->GameplayTagContainer : FGameplayTagContainer() };
					Target->ProcessEvent(const_cast<UFunction*>(Function), &Params);
				}
				else if (wcsstr(EventFunction.FunctionPath, L"StartEventAtIndex"))
				{
					int StartingIndex = 0;
					Target->ProcessEvent(const_cast<UFunction*>(Function), &StartingIndex);
				}
				else
				{
					float fr = 0.f;
					Target->ProcessEvent(const_cast<UFunction*>(Function), &fr);
				}
			}

			if (VersionInfo.FortniteVersion == 8.51 && ScriptingObject && !EventFunction.bIsLoaderFunction)
			{
				std::thread([ScriptingObject] {
					std::this_thread::sleep_for(std::chrono::minutes(3));

					auto SetUnvaultFn = FindObject<UFunction>(L"/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.SetUnvaultItemName");
					auto PillarsFn = FindObject<UFunction>(L"/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.PillarsConcluded");
					FName Name = UKismetStringLibrary::Conv_StringToName(FString(L"DrumGun"));

					ScriptingObject->ProcessEvent(const_cast<UFunction*>(SetUnvaultFn), &Name);
					ScriptingObject->ProcessEvent(const_cast<UFunction*>(PillarsFn), &Name);
					}).detach();
			}
		}

		if (VersionInfo.FortniteVersion >= 16.00)
		{
			for (auto& UncastedPC : GameMode->AlivePlayers)
			{
				auto PlayerController = (AFortPlayerControllerAthena*)UncastedPC;

				auto PickaxeInstance = PlayerController->WorldInventory->Inventory.ReplicatedEntries.Search([&](FFortItemEntry& entry)
					{
						return entry.ItemDefinition->Cast<UFortWeaponMeleeItemDefinition>();
					}, FFortItemEntry::Size());

				if (PickaxeInstance)
					PlayerController->WorldInventory->Remove(PickaxeInstance->ItemGuid);

				auto EventModeActivator = FindObject<UFortItemDefinition>(L"/EventMode/Items/WID_EventMode_Activator.WID_EventMode_Activator");

				PlayerController->WorldInventory->GiveItem(EventModeActivator);
			}
		}

		printf("[Events] Started!\n");
		return;
	}

	printf("[Events] Build does not have an event.\n");
}