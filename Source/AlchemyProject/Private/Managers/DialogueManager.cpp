// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogueManager.h"
#include "Json.h"


UDialogueManager::UDialogueManager()
{
	DialogueFilePath_Options = TEXT("C:\\Users\\Rotte\\Documents\\Unreal Projects\\AlchemyProject\\Content\\Assets\\JSON\\Dialogue\\DialogueOptions_01.json");
	DialogueFilePath_States = TEXT("C:\\Users\\Rotte\\Documents\\Unreal Projects\\AlchemyProject\\Content\\Assets\\JSON\\Dialogue\\DialogueStates_01.json");
}

void UDialogueManager::StartDialogue(const int32 DialogueStateID)
{
	//Load Root Dialogue options for chosen NPC.
	
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*DialogueFilePath_States);
	//UE_LOG(LogTemp, Display, TEXT("DialogueFilePath: %s"), *DialogueFilePath_States)
	//UE_LOG(LogTemp, Display, TEXT("bFileExists: %d"), bFileExists)
	FString JsonString;
	bool bLoadFileSuccess = FFileHelper::LoadFileToString(JsonString, *DialogueFilePath_States);
	//UE_LOG(LogTemp, Display, TEXT("bLoadFileSuccess: %d"), bLoadFileSuccess)
	//UE_LOG(LogTemp, Display, TEXT("JsonString: %s"), *JsonString)
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	bool bDeserialized = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	//if(!bDeserialized) bDeserialized = FJsonSerializer::Deserialize(TJsonReaderFactory<char16_t>::Create(JsonString), JsonObject);
	//UE_LOG(LogTemp, Display, TEXT("bDeserialized: %d"), bDeserialized)
	if(!JsonObject.IsValid()) return;

	TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField("Objects");
	TArray<TSharedPtr<FJsonValue>> DialogueOptions;
	TArray<int32> OptionIDs;
	for(int32 i = 0; i < JsonArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> Object = JsonArray[i]->AsObject();
		if(Object->GetIntegerField("DialogueStateID") == DialogueStateID)
		{
			FString ObjectName = Object->GetStringField("NPCDialogueText");
			//Object->TryGetArrayField("DialogueOptions", DialogueOptions);
			DialogueOptions = Object->GetArrayField(FString("DialogueOptions"));
			//UE_LOG(LogTemp, Warning, TEXT("String: %s"), *ObjectName)
			PreviousDialogueStateID = CurrentDialogueStateID;
			CurrentDialogueStateID = DialogueStateID;
			NPCDialogue = ObjectName;
		}
	}
	if(DialogueOptions.IsEmpty()) return;
	OptionStrings.Empty();
	for(const auto Option : DialogueOptions)
	{
		const auto OptionNumber = Option->AsNumber();
		OptionIDs.Add(FMath::CeilToInt32(OptionNumber));
		GetJSON(DialogueFilePath_Options, FMath::CeilToInt32(OptionNumber));
	}
}

bool UDialogueManager::GetJSON(const FString& FilePath, int32 ID)
{
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	//UE_LOG(LogTemp, Display, TEXT("bFileExists: %d"), bFileExists)
	FString JsonString;
	bool bLoadFileSuccess = FFileHelper::LoadFileToString(JsonString, *FilePath);
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	bool bDeserialized = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if(!JsonObject.IsValid()) return false;
	TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField("Objects");
	for(int32 i = 0; i < JsonArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> Object = JsonArray[i]->AsObject();
		if(!Object.IsValid()) continue;
		if(Object->GetIntegerField("DialogueID") == ID)
		{
			FString ObjectName = Object->GetStringField("TextToDisplay");
			//UE_LOG(LogTemp, Warning, TEXT("String: %s"), *ObjectName)
			OptionStrings.Add(ObjectName);
		}
	}
	
	return true;
	
}
