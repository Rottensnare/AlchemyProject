// Fill out your copyright notice in the Description page of Project Settings.

#include "Json.h"
#include "Managers/DialogueManager.h"


UDialogueManager::UDialogueManager()
{
	
}

void UDialogueManager::StartDialogue(const int32 NPCId)
{

	
	//Load Root Dialogue options for chosen NPC.
	//const FString DialogueFilePath = FPaths::ProjectContentDir()/TEXT("Dialogue/Dialogue_01.json");
	const FString DialogueFilePath = TEXT("C:\\Users\\Rotte\\Documents\\Unreal Projects\\AlchemyProject\\Content\\Assets\\JSON\\Dialogue\\Dialogue_02.json");
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*DialogueFilePath);
	UE_LOG(LogTemp, Display, TEXT("DialogueFilePath: %s"), *DialogueFilePath)
	UE_LOG(LogTemp, Display, TEXT("bFileExists: %d"), bFileExists)
	FString JsonString;
	bool bLoadFileSuccess = FFileHelper::LoadFileToString(JsonString, *DialogueFilePath);
	UE_LOG(LogTemp, Display, TEXT("bLoadFileSuccess: %d"), bLoadFileSuccess)
	TSharedPtr<FJsonObject> JsonObject;
	bool bDeserialized = FJsonSerializer::Deserialize(TJsonReaderFactory<char>::Create(JsonString), JsonObject);
	UE_LOG(LogTemp, Display, TEXT("bDeserialized: %d"), bDeserialized)
	if(!JsonObject.IsValid()) return;

	TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField("Objects");
	constexpr int32 ObjectID = 1;
	for(int32 i = 0; i < JsonArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> Object = JsonArray[i]->AsObject();
		if(Object->GetIntegerField("DialogueID") == ObjectID)
		{
			FString ObjectName = Object->GetStringField("TextToDisplay");
			UE_LOG(LogTemp, Warning, TEXT("String: %s"), *ObjectName)
		}
	}
}
