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
	//Load Dialogue for chosen NPC.
	
	//Check if File for Dialogue States exists
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*DialogueFilePath_States);

	//Load the file onto a FString
	FString JsonString;
	bool bLoadFileSuccess = FFileHelper::LoadFileToString(JsonString, *DialogueFilePath_States);
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	//Deserialize JsonString to JsonObject using the JsonReader
	bool bDeserialized = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if(!JsonObject.IsValid()) return;

	//Get the Objects array for later use
	TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField("Objects");
	TArray<TSharedPtr<FJsonValue>> DialogueOptions;
	//TArray<int32> OptionIDs;

	//Iterate over all the entries and find the one that matches with DialogueStateID
	for(int32 i = 0; i < JsonArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> Object = JsonArray[i]->AsObject();
		if(Object->GetIntegerField("DialogueStateID") == DialogueStateID)
		{
			//Store the Dialogue text and options
			NPCDialogue = Object->GetStringField("NPCDialogueText");
			DialogueOptions = Object->GetArrayField(FString("DialogueOptions"));
			PreviousDialogueStateID = CurrentDialogueStateID;
			CurrentDialogueStateID = DialogueStateID;
		}
	}
	if(DialogueOptions.IsEmpty()) return;
	OptionStrings.Empty();
	EmptyDialogueOptions();
	// Iterate through all the options and get the data from the JSON file
	for(const auto Option : DialogueOptions)
	{
		const auto OptionNumber = Option->AsNumber();
		//OptionIDs.Add(FMath::CeilToInt32(OptionNumber));
		GetJSON(DialogueFilePath_Options, FMath::CeilToInt32(OptionNumber));
	}
}

/**	Used for getting the dialogue options from the corresponding JSON file */
bool UDialogueManager::GetJSON(const FString& FilePath, int32 ID)
{
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
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
			FDialogueOption TempOption;
			FString ObjectName = Object->GetStringField("TextToDisplay");
			TempOption.TextToDisplay = FText::FromString(ObjectName);
			TempOption.NextDialogueStateID = Object->GetNumberField("NextDialogueStateID");
			OptionStrings.Add(ObjectName);
			CurrentDialogueOptions.Emplace(i, TempOption);
		}
	}
	
	return true;
	
}

void UDialogueManager::EmptyDialogueOptions()
{
	CurrentDialogueOptions.Empty();
}

TArray<int32> UDialogueManager::GetDialogueOptions(TArray<FDialogueOption>& OutOptions)
{
	TArray<FDialogueOption> TempOptions;
	TArray<int32> ReturnInts;
	for(const auto& Option : CurrentDialogueOptions)
	{
		ReturnInts.Add(Option.Key);
		OutOptions.Add(Option.Value);
	}
	 
	return ReturnInts;
}
